#!/usr/bin/env python3
"""Compare the compiled code of two source trees, with every symbol resolved.

tools/match_func.py masks the relocated fields of an unlinked object: a
`sym + 4` that should have been `sym + 8` (an array of the wrong element type,
a struct view one field off) masks equal there and only fails `make verify`.
This closes that gap for edits that should not change the code at all (names,
declarations, header moves): it compiles every C file of a base revision and of
the working tree to assembly (the Makefile's cpp and cc1, with the file's own
`/* cc1-flags: */`), replaces every `symbol +/- offset` operand by the absolute
address it links to (the symbol table of the linked ELF), and reports the files
whose normalized assembly differs. A whole-tree run takes about ten seconds.

Renaming a global, moving its declaration to a header, giving it its real type
or reaching it as `gScene.m_Field` instead of `gScene_m_Field` all compile to
the same addresses, so they compare equal; anything that moves an access, a
load order or a register shows as a diff. Symbols the ELF does not know
(a new alias, defined in versions/symbols_manual.us.txt, or a name
tools/apply_names.py just wrote to symbols_names*.us.txt) are looked up there
too; anything else stays a name, so rebuild the ELF once after adding names.

A merged file (a module in one C file, tools/merge_module.py) is new at a base
that still has the module's one-function files: it is compared function by
function with them (round 29). Every `.ent NAME` .. `.end NAME` of the merged
file is matched by name with the same function of the base files that are gone
from the working tree (the one in the same directory first), local labels are
numbered per function and every `.LCn` literal operand is replaced by the data
it labels; a differing function prints `DIFF file: function`. A whole-tree run
also prints `GONE` for a base function that no file defines any more. The data
of the merged object (its .rodata order included) is still checked only by
`make verify`.

Usage:
    python3 tools/asm_diff.py                     working tree vs HEAD, all of src/
    python3 tools/asm_diff.py --base ecba2b71     against another revision
    python3 tools/asm_diff.py src/us/code/CCamera/func_0027ECF0.c ...   only these files
    python3 tools/asm_diff.py -v                  show the differing lines (per function for a merged file)

Exit status: 0 when every file compiles and matches, 1 otherwise.
"""

import argparse
import difflib
import os
import re
import shutil
import subprocess
import sys
import tempfile
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

from elftools.elf.elffile import ELFFile

ROOT = Path(__file__).resolve().parent.parent
CC1 = ROOT / "tools" / "sn64" / "cc1"
CPP_FLAGS = ["-P", "-undef", "-Wall", "-lang-c", "-D_LANGUAGE_C", "-DF3DEX_GBI_2",
             "-D__GNUC__=2", '-DGAME_VERSION="us"', "-nostdinc"]
CC_FLAGS = "-quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mno-fix4300 -mhard-float -mfp64"
FLAGS_LINE = re.compile(r"^/\* cc1-flags: (.*) \*/$", re.M)
OPERAND = re.compile(r"\b([A-Za-z_][A-Za-z0-9_.]*)((?:\s*[+-]\s*(?:0x[0-9a-fA-F]+|\d+))*)")
ADDR_NAME = re.compile(r"D_[0-9A-F]{8}")
LOCAL_LABEL = re.compile(r"\.L\d+\b")
MANUAL = re.compile(r"^\s*(\w+)\s*=\s*0x([0-9A-Fa-f]+)\s*;", re.M)


def symbols(elf_path):
    out = {}
    with open(elf_path, "rb") as fh:
        for sym in ELFFile(fh).get_section_by_name(".symtab").iter_symbols():
            if sym.name and sym.name not in out:
                out[sym.name] = sym["st_value"]
    # Aliases the ELF does not know yet: manual symbols, and names just added to
    # the names tables (tools/apply_names.py writes them here; ld's file holds the
    # module-2 data at its link address).
    for f in ("symbols_manual.us.txt", "symbols_names_ld.us.txt", "symbols_names.us.txt"):
        for m in MANUAL.finditer((ROOT / "versions" / f).read_text()):
            a = int(m.group(2), 16)
            # a new name for an ELF func_/D_<a>: where that symbol links (a module-2
            # copy links 0x39000 below its VM address)
            name = m.group(1)
            if name in out:
                # the ELF has the name already: the base trees func_/D_<a> links there
                out.setdefault("func_%08X" % a, out[name])
                out.setdefault("D_%08X" % a, out[name])
                continue
            out[name] = out.get("func_%08X" % a, out.get("D_%08X" % a, a))
    return out


def compile_one(tree, rel, out_dir):
    """tree/rel -> out_dir/rel.s; returns None or the error text."""
    src = tree / rel
    out = out_dir / (rel + ".s")
    out.parent.mkdir(parents=True, exist_ok=True)
    inc = ["-I" + str(tree), "-I" + str(tree / "include"), "-I" + str(ROOT / "us" / "include")]
    with tempfile.TemporaryDirectory() as tmp:
        i = Path(tmp) / "f.i"
        r = subprocess.run(["cpp", *CPP_FLAGS, *inc, "-o", str(i), str(src)],
                           capture_output=True, text=True)
        if r.returncode:
            return "cpp: " + r.stderr.strip()
        m = FLAGS_LINE.search(src.read_text(errors="replace"))
        flags = (m.group(1) if m else CC_FLAGS).split()
        r = subprocess.run([str(CC1), *flags, "-o", str(out), str(i)], capture_output=True, text=True)
        if r.returncode:
            out.unlink(missing_ok=True)
            return "cc1: " + "\n".join(l for l in r.stderr.splitlines() if "warning" not in l)[:2000]
    return None


def read_lines(path):
    return [line.strip() for line in path.read_text(errors="replace").splitlines()
            if line.strip() and not line.strip().startswith((".file", ".ident", "#"))]


def resolve_line(s, syms):
    def resolve(m):
        name = m.group(1)
        if name in syms:
            base = syms[name]
        elif ADDR_NAME.fullmatch(name):
            # D_<address>: a base tree's name the rebuilt ELF has renamed
            base = int(name[2:], 16)
        else:
            return m.group(0)
        off = sum(int(t.replace(" ", ""), 0) for t in re.findall(r"[+-]\s*(?:0x[0-9a-fA-F]+|\d+)", m.group(2)))
        return "@%08X" % ((base + off) & 0xFFFFFFFF)
    return OPERAND.sub(resolve, s)


def relabel(lines):
    # Local labels (.L4) are numbered by cc1 in the order it makes them, so a
    # rewrite that drops a dead branch (a de-faked FAKEMATCH) renumbers them
    # without changing a byte: number them by first appearance instead.
    labels = {}

    def one(m):
        return labels.setdefault(m.group(0), ".L@%d" % len(labels))
    return [LOCAL_LABEL.sub(one, l) for l in lines]


def normalize(path, syms):
    return relabel([resolve_line(s, syms) for s in read_lines(path)])


ENT = re.compile(r"^\.ent\s+(\S+)$")
LITERAL = re.compile(r"\.LC\d+\b")
ADDR = re.compile(r"@[0-9A-F]{8}\b")
DATA = (".word", ".half", ".byte", ".ascii", ".asciz", ".string", ".float", ".double", ".space", ".dword")


def functions(path, syms):
    """{name: normalized text of the function} of one compiled file: the lines from
    `.ent NAME` to `.end NAME`, local labels numbered per function, and every
    `.LCn` literal operand replaced by the data the label holds (a merged file
    numbers its literals across the whole object)."""
    lines = read_lines(path)
    lits, cur = {}, None
    for l in lines:
        m = re.fullmatch(r"([A-Za-z_.][\w.]*):", l)
        if m and not LOCAL_LABEL.fullmatch(m.group(1)):
            cur = m.group(1)
            lits[cur] = []
        elif cur and l.startswith(DATA):
            # (a string keeps its text; a number drops cc1's `# value` comment)
            text = l if l.startswith((".ascii", ".asciz", ".string")) else l.split("#")[0].strip()
            lits[cur].append(resolve_line(text, syms))
        else:
            cur = None
    # A string the file defines under a name (an N64-only `const char x[] __asm__("D_..") = ".."`
    # the merge puts where the ROM has the literal) reads as the literal it replaces: its text.
    strs = {}
    for k, v in lits.items():
        if v and all(d.startswith((".ascii", ".string", ".asciz")) for d in v):
            t = "".join(d.split(None, 1)[1].strip()[1:-1] + ("" if d.startswith(".ascii") else "\\000")
                        for d in v)
            strs[k] = "LC[%s]" % t
    lits = {k: "LC[" + "; ".join(v) + "]" for k, v in lits.items()}
    out, name, body = {}, None, []
    for l in lines:
        m = ENT.match(l)
        if m:
            name, body = m.group(1), []
            continue
        if name is None:
            continue
        if l.split() == [".end", name]:
            out[name] = relabel(body)
            name = None
            continue
        l = LITERAL.sub(lambda m: strs.get(m.group(0)) or lits.get(m.group(0), m.group(0)), l)
        body.append(resolve_line(l, syms))
    # where those named strings link (the other tree reaches the same bytes as a literal)
    at = {"@%08X" % syms[k]: v for k, v in strs.items() if k in syms}
    return out, at


_SECTIONS = []


def elf_string(addr):
    """The NUL-terminated string the linked ELF holds at addr, written as cc1 writes a literal, or None."""
    for base, data in _SECTIONS:
        if base <= addr < base + len(data):
            end = data.find(b"\0", addr - base)
            raw = data[addr - base:end] if 0 <= end - (addr - base) <= 512 else None
            if raw is None or any((b < 0x20 and b not in (9, 10, 13)) or b > 0x7E for b in raw):
                return None
            return "".join(chr(b) if 0x20 <= b <= 0x7E and b not in (0x22, 0x5C) else
                           "\\" + chr(b) if b in (0x22, 0x5C) else "\\%03o" % b for b in raw) + "\\000"
    return None


def literal_view(text):
    """A function's text with every address that holds a string in the ELF read as that literal: a merged
    file's literal against the base's named copy of it (CScene__GetLevelName's "level name")."""
    def one(m):
        st = elf_string(int(m.group(0)[1:], 16))
        return "LC[%s]" % st if st is not None else m.group(0)
    return [ADDR.sub(one, l) for l in text]


def canon(text, at):
    """A function's text with every address that holds one of `at`'s strings read as that literal."""
    return [ADDR.sub(lambda m: at.get(m.group(0), m.group(0)), l) for l in text] if at else text


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("files", nargs="*", help="C files (default: every .c under src/)")
    ap.add_argument("--base", default="HEAD", help="revision to compare against (default HEAD)")
    ap.add_argument("--elf", default=str(ROOT / "build" / "turok2.us.elf"),
                    help="linked ELF whose symbol table resolves the operands")
    ap.add_argument("-j", type=int, default=os.cpu_count(), help="parallel compiles")
    ap.add_argument("-v", "--verbose", action="store_true", help="print the differing lines")
    args = ap.parse_args()

    files = [str(Path(f).resolve().relative_to(ROOT)) for f in args.files] or \
        sorted(str(p.relative_to(ROOT)) for p in (ROOT / "src").rglob("*.c"))
    syms = symbols(args.elf)
    with open(args.elf, "rb") as fh:
        _SECTIONS.extend((sec["sh_addr"], sec.data()) for sec in ELFFile(fh).iter_sections()
                         if sec["sh_type"] == "SHT_PROGBITS" and sec["sh_addr"] and sec["sh_flags"] & 2)
    work = Path(tempfile.mkdtemp(prefix="asm_diff."))
    try:
        base = work / "base"
        base.mkdir()
        archive = subprocess.run(["git", "-C", str(ROOT), "archive", args.base, "src", "include"],
                                 capture_output=True, check=True).stdout
        subprocess.run(["tar", "-x", "-C", str(base)], input=archive, check=True)
        # Files moved since the base (round 23 put every file in its module's directory)
        # are compared with the base file of the same name.
        moved = {}
        for p in (base / "src").rglob("*.c"):
            moved.setdefault(p.name, str(p.relative_to(base)))
        at_base = {f: f if (base / f).exists() else moved.get(Path(f).name) for f in files}
        # A merged file (a module's one C file) is new at the base: its functions are
        # compared one by one with those of the base files that are gone from the
        # working tree (the module's one-function files), matched by name.
        gone = []
        if any(not at_base[f] for f in files):
            here = {p.name for p in (ROOT / "src").rglob("*.c")}
            gone = sorted(str(p.relative_to(base)) for p in (base / "src").rglob("*.c")
                          if not (ROOT / p.relative_to(base)).exists() and p.name not in here)
        jobs = [(base, at_base[f], work / "a") for f in files if at_base[f]] + \
               [(base, g, work / "a") for g in gone] + \
               [(ROOT, f, work / "b") for f in files]
        with ThreadPoolExecutor(args.j) as pool:
            errs = list(pool.map(lambda j: compile_one(*j), jobs))
        failed = {(str(j[0]), j[1]): e for j, e in zip(jobs, errs) if e}
        pool_funcs = {}   # name -> [(base file, normalized text)]
        for g in gone:
            s = work / "a" / (g + ".s")
            if s.exists():
                funcs, at = functions(s, syms)
                for name, text in funcs.items():
                    pool_funcs.setdefault(name, []).append((g, text, at))
        claimed = set()
        bad = 0
        merged = 0
        for f in files:
            if (str(ROOT), f) in failed:
                print(f"FAIL {f}\n    " + failed[(str(ROOT), f)].replace("\n", "\n    "))
                bad += 1
                continue
            a, b = work / "a" / ((at_base[f] or f) + ".s"), work / "b" / (f + ".s")
            if not a.exists():
                if at_base[f]:
                    print(f"NEW  {f} (does not compile at the base)")
                    continue
                funcs, at_b = functions(b, syms)
                d = Path(f).parent
                same = new = 0
                for name, text in funcs.items():
                    cands = pool_funcs.get(name)
                    if not cands:
                        new += 1
                        continue
                    # the candidate in the same directory first (a vm1/vm2 twin has its own)
                    cands = sorted(cands, key=lambda c: Path(c[0]).parent != d)
                    claimed.add((name, cands[0][0]))
                    at = {**cands[0][2], **at_b}
                    ta, tb = canon(cands[0][1], at), canon(text, at)
                    if ta == tb or literal_view(ta) == literal_view(tb):
                        same += 1
                        continue
                    bad += 1
                    diff = [l for l in difflib.unified_diff(ta, tb, lineterm="", n=0)
                            if not l.startswith(("---", "+++"))]
                    print(f"DIFF {f}: {name} (vs {cands[0][0]}, {len(diff)} lines)")
                    if args.verbose:
                        print("\n".join("    " + l for l in diff[:40]))
                if same or len(funcs) - new:
                    merged += 1
                    print(f"MERGED {f}: {same} of {len(funcs)} functions equal to the base's"
                          + (f", {new} not at the base" if new else ""))
                else:
                    print(f"NEW  {f}")
                continue
            na, nb = normalize(a, syms), normalize(b, syms)
            if na != nb:
                bad += 1
                diff = [l for l in difflib.unified_diff(na, nb, lineterm="", n=0)
                        if not l.startswith(("---", "+++"))]
                print(f"DIFF {f} ({len(diff)} lines)")
                if args.verbose:
                    print("\n".join("    " + l for l in diff[:40]))
        # base functions no merged file defines any more
        # (whole-tree runs only: a file list compares only the merged files it names)
        names = {n for n, _ in claimed}
        for name, cands in sorted(pool_funcs.items()) if not args.files else ():
            if name not in names:
                print(f"GONE {name} ({cands[0][0]}): defined by no file of the working tree")
                bad += 1
        print(f"{len(files)} files against {args.base}"
              + (f" ({merged} merged, compared by function)" if merged else "")
              + f": {bad} differ or fail")
        return 1 if bad else 0
    finally:
        shutil.rmtree(work, ignore_errors=True)


if __name__ == "__main__":
    sys.exit(main())
