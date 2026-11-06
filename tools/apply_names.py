#!/usr/bin/env python3
"""Give functions and data their real names: the names files into the build.

Function names: each `Name = 0xVRAM;` line in versions/names.us.txt or
versions/names/<part>.us.txt. Data names: the same lines in
versions/names/data_<part>.us.txt. Every line can carry `// evidence`.

Functions: a splat symbol (versions/symbols_names.us.txt, listed in the yaml's
symbol_addrs_path, so the regenerated asm calls it by name), and func_VRAM
becomes the name in every C file and header, in versions/function_sizes.us.txt
and in versions/manual_stubs.us.txt. The C files keep their file names.

Data: a splat symbol too (no type and no size: see docs/NAMES.md), and
D_VRAM becomes the name in every C file, header and hand-written asm file
under src/ and include/, and in the tracked symbol files that spell D_ names
(symbols_manual, foreign_labels, undefined_funcs_extra, symbol_addrs) and
tools/ghidra/annotations.json. The generated ones (undefined_syms_auto,
undefined_funcs_auto, the linker script, us/asm) come back from `make setup`
with the names. Comments in cuts.us.txt and the yaml keep their D_ addresses.

Second VM module: a function name at 0x0043CC14 or above (the module's named
address) goes to splat with can_be_referenced:False. A data name there goes to
ld instead (versions/symbols_names_ld.us.txt, at the link address). Its data
is linked 0x39000 lower, and code and C reach it there: D_<link address>
becomes the `_2` name when the link address has no name of its own, and, in a
module-2 file that declares the symbol itself, even when it has one (a header
declaration is shared with the first module and keeps the first's name).

Then make setup and make verify.

Usage:
    python3 tools/apply_names.py            apply
    python3 tools/apply_names.py --check    report problems, write nothing
"""

import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
VERSIONS = ROOT / "versions"
NAMES = VERSIONS / "names.us.txt"
NAMES_DIR = VERSIONS / "names"
SYMS = VERSIONS / "symbols_names.us.txt"
SYMS_LD = VERSIONS / "symbols_names_ld.us.txt"
OLD = VERSIONS / "symbol_addrs.us.txt"
YAML = VERSIONS / "turok2.us.yaml"
VM2_NAMED = 0x0043CC14   # virtual_1's vram in the yaml (Makefile VIRTUAL_1_NAMED)
VM2_END = 0x00475A00     # ROM 0x1BFA00 (vm_dup_engine) in virtual_1's naming
VM2_SHIFT = 0x39000      # named - linked (Makefile VIRTUAL_1_NAMED - VIRTUAL_1_LINKED)
BEGIN, END = "// --- names.us.txt (tools/apply_names.py) ---", "// --- end names ---"

LINE = re.compile(r"^\s*(\w+)\s*=\s*0x([0-9A-Fa-f]+)\s*;")
IDENT = re.compile(r"^[A-Za-z_]\w*$")
# Tracked files that spell data symbols by name, besides src/ and include/.
DATA_TEXT_FILES = [
    VERSIONS / "function_sizes.us.txt",
    VERSIONS / "manual_stubs.us.txt",
    VERSIONS / "symbols_manual.us.txt",
    VERSIONS / "foreign_labels.us.txt",
    VERSIONS / "undefined_funcs_extra.us.txt",
    VERSIONS / "symbol_addrs.us.txt",
    ROOT / "tools" / "ghidra" / "annotations.json",
]
C_KEYWORDS = set("""auto break case char const continue default do double else enum extern
float for goto if inline int long register restrict return short signed sizeof static
struct switch typedef union unsigned void volatile while asm typeof __typeof__ __asm__
__attribute__ __inline__ __extension__ __restrict NULL TRUE FALSE""".split())


def in_vm2(a):
    return VM2_NAMED <= a < 0x80000000


def read_names(files, kind):
    """[(name, addr, file, lineno)] plus syntax problems."""
    out, bad = [], []
    for f in files:
        for i, line in enumerate(f.read_text().splitlines(), 1):
            s = line.strip()
            if not s or s.startswith("//") or s.startswith("#"):
                continue
            m = LINE.match(line)
            if not m:
                bad.append(f"{f.name}:{i}: not `Name = 0xVRAM;`: {s[:60]}")
                continue
            out.append((m.group(1), int(m.group(2), 16), f.name, i, kind))
    return out, bad


def collect(entries):
    """Duplicate checks over functions and data together."""
    by_addr = {}   # (kind, addr) -> name
    by_name = {}   # name -> (kind, addr, where)
    any_addr = {}  # addr -> (kind, name)
    bad = []
    for name, addr, fname, ln, kind in entries:
        where = f"{fname}:{ln}"
        if not IDENT.match(name):
            bad.append(f"{where}: {name} is not a C identifier")
        if name.startswith("func_") or re.match(r"^(D|jtbl|STR)_[0-9A-Fa-f]{8}$", name):
            bad.append(f"{where}: {name} is an automatic name, not a real one")
        if name in C_KEYWORDS:
            bad.append(f"{where}: {name} is a C keyword")
        k = (kind, addr)
        if k in by_addr and by_addr[k] != name:
            bad.append(f"0x{addr:08X} named twice: {by_addr[k]}, {name} ({where})")
        if addr in any_addr and any_addr[addr][0] != kind:
            bad.append(f"0x{addr:08X} is both a function ({any_addr[addr][1] if any_addr[addr][0] == 'func' else name})"
                       f" and data ({name if kind == 'data' else any_addr[addr][1]}) ({where})")
        if name in by_name:
            okind, oaddr, owhere = by_name[name]
            if okind != kind:
                bad.append(f"{name} is both a function and data ({owhere}, {where})")
            elif oaddr != addr:
                bad.append(f"{name} on two addresses: 0x{oaddr:08X}, 0x{addr:08X} ({owhere}, {where})")
        by_addr[k] = name
        any_addr.setdefault(addr, (kind, name))
        by_name.setdefault(name, (kind, addr, where))
    funcs = {f"func_{a:08X}": (n, a) for (k, a), n in by_addr.items() if k == "func"}
    data = {f"D_{a:08X}": (n, a) for (k, a), n in by_addr.items() if k == "data"}
    return funcs, data, by_name, bad


def vm2_sources():
    """src/ files linked in the second VM module (yaml segment virtual_1)."""
    files, seg = set(), None
    for line in YAML.read_text().splitlines():
        m = re.match(r"^  - name:\s*(\S+)", line)
        if m:
            seg = m.group(1)
            continue
        if seg != "virtual_1":
            continue
        m = re.match(r"^\s*- \[0x[0-9A-Fa-f]+,\s*(c|hasm|asm),\s*([^\],]+)", line)
        if not m:
            continue
        kind, p = m.group(1), m.group(2).strip()
        if kind == "c":
            files.add((ROOT / "src" / "us" / (p + ".c")).resolve())
        elif kind == "hasm":
            files.add((ROOT / "us" / (p + ".s")).resolve())
    return files


def source_files():
    src = ROOT / "src"
    return (list(src.rglob("*.c")) + list(src.rglob("*.h")) + list(src.rglob("*.s"))
            + list((ROOT / "include").rglob("*.h")))


# ---- --check: collisions with identifiers already in use ------------------

def _strip_comments(t):
    t = re.sub(r"/\*.*?\*/", " ", t, flags=re.S)
    return re.sub(r"//[^\n]*", " ", t)


def known_identifiers():
    """Identifiers the new names must not take: {name: kind}."""
    kinds = {}
    headers = list((ROOT / "include").rglob("*.h")) + list((ROOT / "src").rglob("*.h"))
    for extra in (ROOT / "references" / "ultralib" / "include", ROOT / "us" / "include"):
        if extra.exists():
            headers += list(extra.rglob("*.h"))
    cfiles = list((ROOT / "src").rglob("*.c"))

    def add(name, kind):
        if not name.startswith(("D_", "func_")):
            kinds.setdefault(name, kind)

    for p in headers + cfiles:
        t = _strip_comments(p.read_text(errors="replace"))
        lib = "ultralib" in str(p)
        for m in re.finditer(r"^\s*#\s*define\s+(\w+)", t, re.M):
            add(m.group(1), "libultra macro" if lib else "macro")
        for m in re.finditer(r"\b(?:struct|union|enum)\s+(\w+)", t):
            add(m.group(1), "struct/union/enum tag")
        for m in re.finditer(r"\btypedef\b", t):
            # the typedef's name: the last identifier before its `;` at depth 0
            depth, j = 0, m.end()
            while j < len(t):
                c = t[j]
                if c in "{(":
                    depth += 1
                elif c in "})":
                    depth -= 1
                elif c == ";" and depth == 0:
                    break
                j += 1
            body = t[m.end():j]
            fp = re.search(r"\(\s*\*\s*(\w+)\s*\)", body)
            tail = re.search(r"(\w+)\s*(?:\[[^\]]*\]\s*)*$", re.sub(r"\{.*\}", " ", body, flags=re.S))
            if fp and "{" not in body:
                add(fp.group(1), "typedef")
            elif tail:
                add(tail.group(1), "typedef")
        for m in re.finditer(r"\bextern\b[^;(]*?\b(\w+)\s*(?:\[[^\]]*\]\s*)*;", t):
            add(m.group(1), "libultra global" if lib else "global")
        if p.suffix == ".h":
            for m in re.finditer(r"\b(\w+)\s*\([^;{}()]*(?:\([^()]*\)[^;{}()]*)*\)\s*;", t):
                if m.group(1) not in C_KEYWORDS:
                    add(m.group(1), "libultra function" if lib else "function prototype")
        if p.suffix == ".c":
            for m in re.finditer(r"^[A-Za-z_][\w \*]*?\b(\w+)\s*\([^;{}]*\)\s*\{", t, re.M):
                add(m.group(1), "C function")
    return kinds


def check(funcs, data, by_name):
    """Collisions with identifiers in use; addresses nothing refers to."""
    applied = set()
    if SYMS.exists():
        applied = {m.group(1) for m in map(LINE.match, SYMS.read_text().splitlines()) if m}
    known = known_identifiers()
    errs, warns = [], []
    for name, (kind, addr, where) in sorted(by_name.items()):
        if name in applied:
            continue                        # already in the build under this name
        k = known.get(name)
        if k:
            (errs if kind == "data" or k not in ("function prototype", "C function") else warns).append(
                f"{where}: {name} (0x{addr:08X}) collides with a {k} of that name")
    # Every token in the sources, to see what the data names would replace.
    tokens = set()
    texts = []
    for p in source_files():
        t = p.read_text(errors="replace")
        texts.append(t)
        tokens.update(re.findall(r"\bD_[0-9A-F]{8}\b", t))
    for f in DATA_TEXT_FILES + [VERSIONS / "undefined_syms_auto.us.txt"]:
        if f.exists():
            tokens.update(re.findall(r"\bD_[0-9A-F]{8}\b", f.read_text(errors="replace")))
    words = set()
    for t in texts:
        words.update(re.findall(r"\b[A-Za-z_]\w*\b", _strip_comments(t)))
    for name, (kind, addr, where) in sorted(by_name.items()):
        if name in applied or kind != "data":
            continue
        if name in words and name not in known:
            warns.append(f"{where}: {name} is already an identifier in the sources (a field or a local?)")
        tok = f"D_{addr:08X}"
        linked = f"D_{addr - VM2_SHIFT:08X}"
        if tok not in tokens and not (in_vm2(addr) and linked in tokens):
            warns.append(f"{where}: {name}: no {tok} in the sources or the symbol files"
                         " (splat may still label it; check the address)")
        if in_vm2(addr) and not name.endswith("_2"):
            warns.append(f"{where}: {name} at 0x{addr:08X} is in the second VM module but does not end in _2")
        if name.endswith("_2") and not in_vm2(addr):
            warns.append(f"{where}: {name} ends in _2 but 0x{addr:08X} is not in the second VM module")
    vm2 = vm2_sources()
    link = {f"D_{a - VM2_SHIFT:08X}": n for n, (k, a, w) in by_name.items()
            if k == "data" and VM2_NAMED <= a < VM2_END and n not in applied}
    link = {t: n for t, n in link.items() if t not in data}
    if link:
        lpat = re.compile(r"\b(" + "|".join(link) + r")\b")
        for p in source_files():
            if p.resolve() in vm2 or p.suffix == ".h":
                continue
            for tok in sorted(set(lpat.findall(p.read_text(errors="replace")))):
                warns.append(f"{p.relative_to(ROOT)}: first-module file uses {tok}, which becomes"
                             f" {link[tok]} (the second module's data linked there)")
    return errs, warns


# ---- apply -----------------------------------------------------------------

def main():
    check_only = "--check" in sys.argv[1:]
    func_files = [NAMES] + sorted(p for p in NAMES_DIR.glob("*.us.txt") if not p.name.startswith("data_"))
    data_files = sorted(NAMES_DIR.glob("data_*.us.txt"))
    fe, bad1 = read_names(func_files, "func")
    de, bad2 = read_names(data_files, "data")
    funcs, data, by_name, bad = collect(fe + de)
    bad = bad1 + bad2 + bad
    stray = [p.name for p in NAMES_DIR.glob("data_*") if not p.name.endswith(".us.txt")]
    if stray:
        bad.append("not read (name them data_<part>.us.txt): " + ", ".join(stray))
    if check_only:
        errs, warns = check(funcs, data, by_name)
        for w in warns:
            print("warning:", w)
        for e in bad + errs:
            print("error:", e)
        print(f"{len(funcs)} function names, {len(data)} data names; "
              f"{len(bad) + len(errs)} errors, {len(warns)} warnings")
        return 1 if bad or errs else 0
    if bad:
        print("\n".join(bad), file=sys.stderr)
        return 1

    # The second VM module (virtual_1) is named from 0x43CC14 but linked
    # 0x39000 lower, so its C functions are defined at the link address. A
    # name splat may use as a reference target resolves there: data words that
    # only look like 0x44xxxx pointers would become the link address, and
    # splat lists such names in undefined_funcs_auto at the named address,
    # which overrides the C definition. Nothing in the ROM points at the named
    # addresses, so these names are labels only. The same holds for its data.
    def attrs(a):
        return " can_be_referenced:False" if in_vm2(a) else ""
    # Data symbols get no type: splat accepts only its own and spimdisasm's
    # (`f32`, `asciz`, ...), which change how the bytes are disassembled. And no
    # size: with a size spimdisasm writes every reference inside the object as
    # `name + 0xN`, and the D_ labels inside it, which C still uses, vanish.
    #
    # The second module's data names do not go to splat at all. A symbol inside
    # virtual_rodata_1 changes how spimdisasm splits that pool, and with it
    # which words elsewhere it takes for pointers: gMotherSpawnPoints_2 made a
    # word 0x00470100 in libultra's data a reference to a new label there,
    # which links 0x39000 lower (can_be_referenced:False does not stop that;
    # the reference is to the new label). They go to ld instead, at the link
    # address (versions/symbols_names_ld.us.txt), unless an object defines them.
    def vm2_data(a):
        return VM2_NAMED <= a < VM2_END
    SYMS.write_text(
        "// Generated by tools/apply_names.py from versions/names.us.txt and versions/names/; edit those.\n"
        + "".join(f"{n} = 0x{a:08X}; // type:func{attrs(a)}\n" for n, a in funcs.values())
        + "// data (versions/names/data_*.us.txt)\n"
        + "".join(f"{n} = 0x{a:08X};" + (f" //{attrs(a)}" if attrs(a) else "") + "\n"
                  for n, a in sorted(data.values(), key=lambda x: x[1]) if not vm2_data(a)))
    SYMS_LD.write_text(
        "/* Generated by tools/apply_names.py: the second VM module's data names, at\n"
        " * the link address (named - 0x39000), for ld. Not given to splat (see the\n"
        " * tool). tools/filter_defined_syms.py drops those an object defines. */\n"
        + "".join(f"{n} = 0x{a - VM2_SHIFT:08X}; /* named 0x{a:08X} */\n"
                  for n, a in sorted(data.values(), key=lambda x: x[1]) if vm2_data(a)))
    # An earlier version wrote a block into symbol_addrs.us.txt.
    if OLD.exists():
        text = OLD.read_text()
        OLD.write_text(re.sub(re.escape(BEGIN) + r".*?" + re.escape(END) + r"\n?", "", text, flags=re.S))
    # The address scan names every function func_VRAM; a renamed one would
    # then have two names at one address. Drop the scan's entry.
    scan = VERSIONS / "symbols_from_scan.us.txt"
    gone = set(funcs) | set(data)
    lines = scan.read_text().splitlines()
    kept = [l for l in lines if not (LINE.match(l) and LINE.match(l).group(1) in gone)]
    scan.write_text("\n".join(kept) + "\n")

    ren = {k: v[0] for k, v in funcs.items()}
    ren.update({k: v[0] for k, v in data.items()})
    dpat = re.compile(r"\b(D_[0-9A-F]{8})\b")
    both = re.compile(r"\b((?:func|D)_[0-9A-F]{8})\b")
    # Module-2 files: D_<link address> -> the _2 name at link + 0x39000, where
    # the file declares the symbol itself (a header's declaration is shared
    # with the first module, so those keep the first module's name).
    vm2_link = {f"D_{a - VM2_SHIFT:08X}": n for n, a in data.values() if VM2_NAMED <= a < VM2_END}
    # Everywhere else, a link address with no name of its own takes the _2
    # name: the module-2 label is linked there, so it is the same value (the
    # first module's data overlaps that range, so a first-module file doing
    # this is worth a look; --check lists them).
    for tok, n in vm2_link.items():
        ren.setdefault(tok, n)
    vm2 = vm2_sources()
    changed = 0
    for path in source_files():
        src = path.read_text()
        local = {}
        if path.resolve() in vm2 and vm2_link:
            for tok in set(dpat.findall(src)) & set(vm2_link):
                if re.search(r"\bextern\b[^;]*\b" + tok + r"\b", src):
                    local[tok] = vm2_link[tok]
        # Function names in C and headers only: hand-written asm keeps func_
        # labels (its strays sit inside functions splat cut wrong).
        sub = dpat if path.suffix == ".s" else both
        new = sub.sub(lambda m: local.get(m.group(1)) or ren.get(m.group(1), m.group(1)), src)
        if new != src:
            path.write_text(new)
            changed += 1
    # function_sizes.us.txt gives some functions a size under their func_
    # name; left as is, splat sees two names at one address and the data
    # (jump tables, callback tables) keeps calling them func_VRAM.
    for f in DATA_TEXT_FILES:
        if not f.exists():
            continue
        text = f.read_text()
        sub = both if f.name in ("function_sizes.us.txt", "manual_stubs.us.txt") else dpat
        new = sub.sub(lambda m: ren.get(m.group(1), m.group(1)), text)
        if f.name == "function_sizes.us.txt":
            # A second size line for a second-module name must say the same,
            # or splat takes the reference back.
            new = re.sub(r"^(\w+ = 0x([0-9A-Fa-f]+); // type:func[^\n]*?)$",
                         lambda m: m.group(1) if "can_be_referenced" in m.group(1)
                         or m.group(1).split()[0].startswith("func_")
                         else m.group(1) + attrs(int(m.group(2), 16)),
                         new, flags=re.M)
        if new != text:
            f.write_text(new)
            changed += 1
    print(f"{len(funcs)} function names, {len(data)} data names; {changed} files renamed in")
    return 0


if __name__ == "__main__":
    sys.exit(main())
