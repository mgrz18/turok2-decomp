#!/usr/bin/env python3
"""Link libultra objects from lib/ultralib (decompals/ultralib, a pinned submodule) in place of
the game's own per-function C.

A module of versions/modules.us.txt named `libultra/<file>.c` whose every function is identical
to ultralib's 2.0I `libgultra_rom` C (versions/ultralib.us.txt) is a candidate. This tool compiles
lib/ultralib/src/<file>.c with the flags the Makefile gives it (UL_CPP_FLAGS / UL_CC_FLAGS), then
checks the whole object against the ROM, not only its functions:

  .text    every word equal to the ROM from the module's first function (relocations masked),
           and its size ends where the module's last function ends;
  .rodata  found in the engine's rodata (byte-equal, relocated words masked; where the game C
           has .rodata of its own, the one place that covers it, else the only place), over
           nothing but the module's own C .rodata and splat's unnamed `rodata` pieces (split
           where the object starts or ends inside one: assert's file names, float pools);
  .data/.bss  empty (a libultra object with data of its own would move the data segments).

  tools/ultralib_build.py            report every candidate and why it is (not) taken
  tools/ultralib_build.py --write    rewrite versions/turok2.us.yaml for the new ones and write
                                     versions/ultralib_objects.us.txt (then `make setup`)

Modules already linked from lib/ultralib are recognised and kept in the list. docs/LIBULTRA.md
explains the build; `make verify` is the arbiter.
"""
import argparse
import os
import re
import shlex
import struct
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
from elftools.elf.elffile import ELFFile  # noqa: E402
from elftools.elf.relocation import RelocationSection  # noqa: E402

UL = ROOT / "lib" / "ultralib"
YAML = ROOT / "versions" / "turok2.us.yaml"
MODULES = ROOT / "versions" / "modules.us.txt"
MAP = ROOT / "versions" / "ultralib.us.txt"
LIST = ROOT / "versions" / "ultralib_objects.us.txt"
ROM = ROOT / "baserom.us.z64"
OUT = ROOT / "build" / "ultralib_check"
LIB_VRAM = (0x0028EF90, 0x0029F190)
# The Controller Pak code in the first paged VM module (its copy in the second one is _2).
VM_PFS = (0x00427000, 0x0042A000)
# The engine's rodata (segment code_rodata, KSEG0): where the live library's .rodata is.
RODATA_ROM = (0xA5DF8, 0xC3074)
R_MIPS_32, R_MIPS_26, R_MIPS_HI16, R_MIPS_LO16 = 2, 4, 5, 6
UL_PREFIX = "../../lib/ultralib/src/"
# The second paged VM module's copies: a wrapper per file that includes ultralib's and renames
# its globals to the game's _2 names (vm2_names.h), both written by --write.
VM2 = ROOT / "lib" / "ultralib_vm2"
VM2_PREFIX = "../../lib/ultralib_vm2/"
NAMES = ROOT / "versions" / "symbols_names.us.txt"


def game_names():
    out = {}
    for line in NAMES.read_text().splitlines():
        m = re.match(r"(\w+) = 0x([0-9A-Fa-f]+);", line)
        if m:
            out[m.group(1)] = int(m.group(2), 16)
    return out


def object_globals(obj):
    with open(obj, "rb") as fh:
        st = ELFFile(fh).get_section_by_name(".symtab")
        return {x.name for x in st.iter_symbols() if x["st_info"]["bind"] == "STB_GLOBAL" and x.name}


def vm2_wrapper(rel):
    return (f"/* The second paged VM module's copy of ultralib's {rel}.c (docs/LIBULTRA.md): the same\n"
            f" * code under the game's _2 names. Written by tools/ultralib_build.py --write. */\n"
            f"#include \"vm2_names.h\"\n#include \"{rel}.c\"\n")


def make_var(name):
    """A variable of the Makefile, expanded by make itself (one source of truth for the flags)."""
    r = subprocess.run(["make", "-s", "--no-print-directory", f"print-{name}"], cwd=ROOT,
                       capture_output=True, text=True, check=True)
    return r.stdout.strip()


def modules():
    """[(name, first vram, end vram, [function vram])] of the libultra modules in .code."""
    out, cur = [], None
    for line in MODULES.read_text().splitlines():
        m = re.match(r"module (\S+): 0x([0-9A-F]+)\.\.0x([0-9A-F]+),", line)
        if m:
            cur = (m.group(1), int(m.group(2), 16), int(m.group(3), 16), [])
            out.append(cur)
            continue
        m = re.match(r"  0x([0-9A-F]{8}) (\w+)", line)
        if m and cur:
            cur[3].append(int(m.group(1), 16))
    return [m for m in out if m[0].startswith("libultra/") and (LIB_VRAM[0] <= m[1] < LIB_VRAM[1] or VM_PFS[0] <= m[1] < VM_PFS[1])]


def ulmap():
    """vram -> (size, ultralib file, status, the game's name)."""
    out = {}
    for line in MAP.read_text().splitlines():
        m = re.match(r"0x([0-9A-F]{8})\s+(\d+) (\S+) = (\S+?):(\S+); (.*)", line)
        if m:
            out[int(m.group(1), 16)] = (int(m.group(2)), m.group(4), m.group(6), m.group(3))
    return out


ENTRY = re.compile(r"^(\s*)- \[0x([0-9A-F]+), (\.?\w+)(?:, (\S+?))?\]")


def compile_obj(src, cpp_flags, cc_flags, inc=None):
    o = OUT / src.relative_to(ROOT).with_suffix(".c.o")
    o.parent.mkdir(parents=True, exist_ok=True)
    i, s = o.with_suffix(".i"), o.with_suffix(".s_c")
    # from lib/ultralib/src, like the Makefile: __FILE__ is "audio/csplayer.c" in the ROM
    arg = str(src.relative_to(UL / "src")) if src.is_relative_to(UL) else str(src)
    extra = [f"-I{inc}"] if inc else []
    subprocess.run(["cpp", *shlex.split(cpp_flags), *extra, f"-I{UL / 'src'}", "-o", str(i), arg],
                   check=True, cwd=UL / "src")
    subprocess.run(["unix2dos", "-q", str(i)], check=True)
    subprocess.run(["tools/sn64/cc1", *shlex.split(cc_flags), "-o", str(s), str(i)], check=True,
                   cwd=ROOT, capture_output=True)
    subprocess.run(["tools/sn_as.sh", str(s), str(o)], check=True, cwd=ROOT, capture_output=True,
                   env={**os.environ, "INC_DIR": "us/include"})
    return o


def sections(obj):
    """name -> (bytes, {offset: reloc type}, alignment) for .text/.rodata/.data/.bss."""
    out = {}
    with open(obj, "rb") as fh:
        elf = ELFFile(fh)
        idx = {}
        for n, sec in enumerate(elf.iter_sections()):
            if sec.name in (".text", ".rodata", ".data", ".bss", ".sdata", ".sbss"):
                data = b"\0" * sec["sh_size"] if sec["sh_type"] == "SHT_NOBITS" else sec.data()
                out[sec.name] = (data, {}, sec["sh_addralign"])
                idx[n] = sec.name
        for sec in elf.iter_sections():
            if isinstance(sec, RelocationSection) and sec["sh_info"] in idx:
                for r in sec.iter_relocations():
                    out[idx[sec["sh_info"]]][1][r["r_offset"]] = r["r_info_type"]
    return out


def same(got, rom, relocs):
    """None if equal (relocated fields masked), else the first differing offset."""
    if len(rom) < len(got):
        return 0
    for off in range(0, len(got) & ~3, 4):
        g, w = struct.unpack(">I", got[off:off + 4])[0], struct.unpack(">I", rom[off:off + 4])[0]
        t = relocs.get(off)
        if t == R_MIPS_26:
            g, w = g & 0xFC000000, w & 0xFC000000
        elif t in (R_MIPS_HI16, R_MIPS_LO16):
            g, w = g & 0xFFFF0000, w & 0xFFFF0000
        elif t is not None:          # R_MIPS_32: a pointer (jump table) in .rodata
            continue
        if g != w:
            return off
    tail = len(got) & ~3
    if got[tail:] != rom[tail:len(got)]:
        return tail
    return None


class Yaml:
    def __init__(self):
        self.lines = YAML.read_text().splitlines(keepends=True)
        self.text_at, self.ro_at, self.entries, self.c_named = {}, {}, [], {}
        for n, line in enumerate(self.lines):
            m = ENTRY.match(line)
            if not m:
                continue
            self.entries.append(n)
            f = re.fullmatch(r"code/func_([0-9A-F]{8})", m.group(4) or "")
            if m.group(3) == "c" and (m.group(4) or "").startswith("code/"):
                self.c_named[m.group(4)[5:]] = n
            if f and m.group(3) == "c":
                self.text_at[int(f.group(1), 16)] = n
            elif f and m.group(3) == ".rodata":
                self.ro_at[int(f.group(1), 16)] = n

    def rom(self, n):
        return int(ENTRY.match(self.lines[n]).group(2), 16)

    def covering(self, off):
        """index into entries of the subsegment that holds ROM offset off."""
        lo = 0
        for k, n in enumerate(self.entries):
            if self.rom(n) <= off:
                lo = k
            else:
                break
        return lo


def check_rodata(y, rom, ro, rrel, align, funcs):
    """-> (start rom, end rom, first entry index, last entry index) or an error string."""
    own = {y.ro_at[v] for v in funcs if v in y.ro_at}
    size = len(ro)
    cands = [a for a in range(RODATA_ROM[0], RODATA_ROM[1] - size, max(align, 4))
             if rom[a:a + 4] == ro[:4] or 0 in rrel]
    cands = [a for a in cands if same(ro, rom[a:a + size], rrel) is None]
    if own:
        lo, hi = min(y.rom(n) for n in own), max(y.rom(n) for n in own)
        cands = [a for a in cands if a <= lo and hi < a + size]
    if len(cands) != 1:
        return f".rodata ({size:#x} bytes) found at {len(cands)} places"
    start = cands[0]
    end = (start + size + 3) & ~3
    if any(rom[start + size:end]):
        # the original linker left stale bytes in the last word's padding; an object's section
        # is padded with zeros, and the next piece cannot start inside a word (subalign 4)
        return f".rodata ends at +{size:#x}, and the ROM has stale bytes after it (not zeros)"
    k0, k1 = y.covering(start), y.covering(end - 1)
    for k in range(k0, k1 + 1):
        n = y.entries[k]
        m = ENTRY.match(y.lines[n])
        if n in own:
            continue
        if m.group(3) != "rodata" or m.group(4):
            return f".rodata would cover {y.lines[n].strip()}"
    if any(v in y.ro_at and y.ro_at[v] not in [y.entries[k] for k in range(k0, k1 + 1)] for v in funcs):
        return "a function's .rodata lies outside the object's"
    return (start, end, k0, k1)


def plan():
    rom = ROM.read_bytes()
    cpp_flags, cc_flags = make_var("UL_CPP_FLAGS"), make_var("UL_CC_FLAGS")
    y = Yaml()
    yaml_text = "".join(y.lines)
    um = ulmap()
    taken, report = [], []
    gn = game_names()
    vm2_defs = set()
    for name, start, end, funcs in modules():
        st = [um.get(v) for v in funcs]
        files = {s[1] for s in st if s}
        rel = next(iter(files)).removeprefix("src/").removesuffix(".c") if len(files) == 1 else None
        vm2 = name.endswith("_2")
        if rel and vm2 and f"c, {VM2_PREFIX}{rel}_2]" in yaml_text:
            taken.append((name, rel + "_2", None, None, funcs))
            report.append((name, "linked from lib/ultralib (the _2 copy)"))
            continue
        if rel and not vm2 and f"c, {UL_PREFIX}{rel}]" in yaml_text:
            taken.append((name, rel, None, None, funcs))
            report.append((name, "linked from lib/ultralib"))
            continue
        if any(s is None or not re.search(r"identical .*O3fp64", s[2]) for s in st):
            report.append((name, "not every function identical (O3fp64)"))
            continue
        if not rel:
            report.append((name, f"functions from {len(files)} ultralib files"))
            continue
        for v in funcs:          # a few game files carry the function's name, not its address
            if v not in y.text_at and um[v][3] in y.c_named:
                y.text_at[v] = y.c_named[um[v][3]]
            # the second VM module's files are named at the address splat gives it
            named = gn.get(um[v][3])
            if v not in y.text_at and named is not None and f"func_{named:08X}" in y.c_named:
                y.text_at[v] = y.c_named[f"func_{named:08X}"]
        if any(v not in y.text_at for v in funcs):
            report.append((name, "a function is not a `c, code/func_` subsegment"))
            continue
        try:
            obj = compile_obj(UL / "src" / f"{rel}.c", cpp_flags, cc_flags)
            if vm2:
                ren = sorted(g for g in object_globals(obj) if g + "_2" in gn)
                vm2_defs.update(ren)
                w = OUT / "vm2" / f"{rel}_2.c"
                w.parent.mkdir(parents=True, exist_ok=True)
                w.write_text(vm2_wrapper(rel))
                hdr = OUT / "vm2" / "vm2_names.h"
                hdr.write_text("".join(f"#define {g} {g}_2\n" for g in ren))
                obj = compile_obj(w, cpp_flags, cc_flags, OUT / "vm2")
                rel = rel + "_2"
        except subprocess.CalledProcessError as e:
            report.append((name, f"does not compile: {e}"))
            continue
        secs = sections(obj)
        text, trel, _ = secs.get(".text", (b"", {}, 4))
        t0, t1 = y.text_at[funcs[0]], y.text_at[funcs[-1]]
        rom0 = y.rom(t0)
        want_len = funcs[-1] + um[funcs[-1]][0] - start
        if len(text) != want_len:
            report.append((name, f".text {len(text):#x} bytes, the module {want_len:#x}"))
            continue
        bad = same(text, rom[rom0:rom0 + len(text)], trel)
        if bad is not None:
            report.append((name, f".text differs at +{bad:#x}"))
            continue
        extra = [k for k in (".data", ".bss", ".sdata", ".sbss") if secs.get(k, (b"",))[0]]
        if extra:
            report.append((name, f"has {', '.join(f'{k} {len(secs[k][0]):#x}' for k in extra)}"))
            continue
        between = [y.lines[i] for i in range(t0, t1 + 1) if ENTRY.match(y.lines[i])]
        if any(", c, code/" not in b for b in between):
            report.append((name, "something other than its own functions between them in the yaml"))
            continue
        ro, rrel, align = secs.get(".rodata", (b"", {}, 4))
        ro_span = None
        if ro:
            r = check_rodata(y, rom, ro, rrel, align, funcs)
            if isinstance(r, str):
                report.append((name, r))
                continue
            ro_span = r
        elif any(v in y.ro_at for v in funcs):
            report.append((name, "the game's C has .rodata, ultralib's none"))
            continue
        taken.append((name, rel, (t0, t1), ro_span, funcs))
        report.append((name, "taken"))
    return taken, report, y, vm2_defs


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--write", action="store_true")
    args = ap.parse_args()
    taken, report, y, vm2_defs = plan()
    for name, why in report:
        print(f"{name:40s} {why}")
    print(f"{sum(t[2] is not None for t in taken)} modules taken now, "
          f"{len(taken)} of {len(report)} linked from lib/ultralib in all")
    if not args.write:
        return
    lines = y.lines
    edits = []          # (first line, last line, [replacement lines])
    for name, rel, tspan, ro_span, funcs in taken:
        if tspan is None:
            continue
        t0, t1 = tspan
        ind = ENTRY.match(lines[t0]).group(1)
        prefix = VM2_PREFIX if name.endswith("_2") else UL_PREFIX
        edits.append((t0, t1, [f"{ind}- [0x{y.rom(t0):X}, c, {prefix}{rel}]\n"]))
        if ro_span:
            start, end, k0, k1 = ro_span
            n0, n1 = y.entries[k0], y.entries[k1]
            ind = ENTRY.match(lines[n0]).group(1)
            rep = []
            if y.rom(n0) < start:                  # keep the head of a split piece
                rep.append(lines[n0])
            rep.append(f"{ind}- [0x{start:X}, .rodata, {prefix}{rel}]\n")
            nxt = y.entries[k1 + 1]
            if y.rom(nxt) > end:                   # the tail of a split piece
                rep.append(f"{ind}- [0x{end:X}, rodata]\n")
            edits.append((n0, n1, rep))
    for a, b, rep in sorted(edits, reverse=True):
        lines[a:b + 1] = rep
    YAML.write_text("".join(lines))
    vm2 = [t for t in taken if t[0].endswith("_2")]
    if vm2:
        hdr = VM2 / "vm2_names.h"
        old = set(re.findall(r"#define (\w+) ", hdr.read_text())) if hdr.exists() else set()
        names = sorted(old | vm2_defs)
        hdr.parent.mkdir(parents=True, exist_ok=True)
        hdr.write_text("/* The globals of ultralib's Controller Pak files under the names the game gives their\n"
                       " * copies in the second paged VM module (docs/LIBULTRA.md). Written by\n"
                       " * tools/ultralib_build.py --write. */\n"
                       + "".join(f"#define {g} {g}_2\n" for g in names))
        for name, rel, *_ in vm2:
            w = VM2 / f"{rel}.c"
            w.parent.mkdir(parents=True, exist_ok=True)
            w.write_text(vm2_wrapper(rel.removesuffix("_2")))
    out = ["// libultra modules linked from lib/ultralib (decompals/ultralib, 2.0I libgultra_rom)",
           "// instead of the game's C; written by tools/ultralib_build.py --write (docs/LIBULTRA.md).",
           "// module = the source the N64 build compiles: the game files (src/us/code) it no longer links"]
    um, gn = ulmap(), game_names()
    code = ROOT / "src" / "us" / "code"

    def game_file(v):
        # by address; a few by name; the second VM module's at the address splat names it
        for stem in (f"func_{v:08X}", um[v][3], f"func_{gn.get(um[v][3], 0):08X}"):
            if (code / f"{stem}.c").exists():
                return f"{stem}.c"
        return f"?{um[v][3]}"
    for name, rel, _, _, funcs in taken:
        src = f"lib/ultralib_vm2/{rel}.c" if name.endswith("_2") else f"lib/ultralib/src/{rel}.c"
        out.append(f"{name} = {src}: " + " ".join(game_file(v) for v in funcs))
    LIST.write_text("\n".join(out) + "\n")
    print(f"wrote {YAML.relative_to(ROOT)} and {LIST.relative_to(ROOT)}")


if __name__ == "__main__":
    main()
