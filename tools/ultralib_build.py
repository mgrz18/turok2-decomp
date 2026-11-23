#!/usr/bin/env python3
"""Link libultra objects from lib/ultralib (decompals/ultralib, a pinned submodule) in place of
the game's own per-function C.

A module of versions/modules.us.txt named `libultra/<file>.c` whose every function is identical
to ultralib's 2.0I `libgultra_rom` C (versions/ultralib.us.txt) is a candidate. This tool compiles
lib/ultralib/src/<file>.c with the flags the Makefile gives it (UL_CPP_FLAGS / UL_CC_FLAGS), then
checks the whole object against the ROM, not only its functions:

  .text    every word equal to the ROM from the module's first function (relocations masked),
           and its size ends where the module's last function ends;
  .rodata  the yaml's .rodata subsegments of the module's functions (and the splat data files
           between them) cover exactly the object's .rodata, byte-equal (relocated words masked);
  .data/.bss  empty (a libultra object with data of its own would move the data segments).

  tools/ultralib_build.py            report every candidate and why it is (not) taken
  tools/ultralib_build.py --write    rewrite versions/turok2.us.yaml for the ones taken and write
                                     versions/ultralib_objects.us.txt (then `make setup`)

docs/LIBULTRA.md explains the build; `make verify` is the arbiter.
"""
import argparse
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
R_MIPS_26, R_MIPS_HI16, R_MIPS_LO16 = 4, 5, 6


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
    return [m for m in out if m[0].startswith("libultra/") and LIB_VRAM[0] <= m[1] < LIB_VRAM[1]]


def ulmap():
    """vram -> (size, ultralib file, status line)."""
    out = {}
    for line in MAP.read_text().splitlines():
        m = re.match(r"0x([0-9A-F]{8})\s+(\d+) (\S+) = (\S+?):(\S+); (.*)", line)
        if m:
            out[int(m.group(1), 16)] = (int(m.group(2)), m.group(4), m.group(6))
    return out


def yaml_lines():
    return YAML.read_text().splitlines(keepends=True)


ENTRY = re.compile(r"^(\s*)- \[0x([0-9A-F]+), (\.?\w+)(?:, (\S+?))?\]")


def compile_obj(src, cpp_flags, cc_flags):
    rel = src.relative_to(ROOT)
    o = OUT / rel.with_suffix(".c.o")
    o.parent.mkdir(parents=True, exist_ok=True)
    i, s = o.with_suffix(".i"), o.with_suffix(".s_c")
    subprocess.run(["cpp", *shlex.split(cpp_flags), "-o", str(i), str(src)], check=True, cwd=ROOT)
    subprocess.run(["unix2dos", "-q", str(i)], check=True)
    subprocess.run(["tools/sn64/cc1", *shlex.split(cc_flags), "-o", str(s), str(i)], check=True, cwd=ROOT)
    subprocess.run(["tools/sn_as.sh", str(s), str(o)], check=True, cwd=ROOT,
                   env={**__import__("os").environ, "INC_DIR": "us/include"})
    return o


def sections(obj):
    """name -> (bytes, {offset: reloc type}) for .text/.rodata/.data/.bss."""
    out = {}
    with open(obj, "rb") as fh:
        elf = ELFFile(fh)
        idx = {}
        for n, sec in enumerate(elf.iter_sections()):
            if sec.name in (".text", ".rodata", ".data", ".bss", ".sdata", ".sbss"):
                data = b"\0" * sec["sh_size"] if sec["sh_type"] == "SHT_NOBITS" else sec.data()
                out[sec.name] = (data, {})
                idx[n] = sec.name
        for sec in elf.iter_sections():
            if isinstance(sec, RelocationSection) and sec["sh_info"] in idx:
                for r in sec.iter_relocations():
                    out[idx[sec["sh_info"]]][1][r["r_offset"]] = r["r_info_type"]
    return out


def same(got, rom, relocs):
    for off in range(0, len(got), 4):
        g, w = struct.unpack(">I", got[off:off + 4])[0], struct.unpack(">I", rom[off:off + 4])[0]
        t = relocs.get(off)
        if t == R_MIPS_26:
            g, w = g & 0xFC000000, w & 0xFC000000
        elif t in (R_MIPS_HI16, R_MIPS_LO16):
            g, w = g & 0xFFFF0000, w & 0xFFFF0000
        elif t is not None:          # R_MIPS_32 (a pointer in .rodata: a jump table)
            continue
        if g != w:
            return off
    return None


def plan():
    rom = ROM.read_bytes()
    cpp_flags, cc_flags = make_var("UL_CPP_FLAGS"), make_var("UL_CC_FLAGS")
    lines = yaml_lines()
    text_at, ro_at, entries = {}, {}, []      # func vram -> line index
    for n, line in enumerate(lines):
        m = ENTRY.match(line)
        if not m:
            continue
        entries.append(n)
        path = m.group(4) or ""
        f = re.fullmatch(r"code/func_([0-9A-F]{8})", path)
        if f and m.group(3) == "c":
            text_at[int(f.group(1), 16)] = n
        elif f and m.group(3) == ".rodata":
            ro_at[int(f.group(1), 16)] = n
    um = ulmap()
    taken, report = [], []
    for name, start, end, funcs in modules():
        st = [um.get(v) for v in funcs]
        if any(s is None or not re.search(r"identical .*O3fp64", s[2]) for s in st):
            report.append((name, "not every function identical (O3fp64)"))
            continue
        files = {s[1] for s in st}
        if len(files) != 1:
            report.append((name, f"functions from {len(files)} ultralib files"))
            continue
        src = UL / files.pop()
        if any(v not in text_at for v in funcs):
            report.append((name, "a function is not a `c, code/func_` subsegment"))
            continue
        try:
            obj = compile_obj(src, cpp_flags, cc_flags)
        except subprocess.CalledProcessError as e:
            report.append((name, f"does not compile: {e}"))
            continue
        secs = sections(obj)
        text, trel = secs.get(".text", (b"", {}))
        first = lines[text_at[funcs[0]]]
        rom0 = int(ENTRY.match(first).group(2), 16)
        last_v = funcs[-1]
        want_len = last_v + um[last_v][0] - start
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
        # the text lines to replace: from the first function's line to the last function's line
        t0, t1 = text_at[funcs[0]], text_at[funcs[-1]]
        between = [lines[i] for i in range(t0, t1 + 1) if ENTRY.match(lines[i])]
        if any("stray_" in b or ", asm" in b or ", c, code/func_" not in b for b in between):
            report.append((name, "something other than its own functions between them in the yaml"))
            continue
        ro, rrel = secs.get(".rodata", (b"", {}))
        ro_span = None
        has_ro = [v for v in funcs if v in ro_at]
        if ro:
            if not has_ro:
                report.append((name, f".rodata {len(ro):#x} bytes but no .rodata subsegment"))
                continue
            r0 = ro_at[has_ro[0]]
            ro_rom = int(ENTRY.match(lines[r0]).group(2), 16)
            # the subsegments from r0 that cover exactly len(ro) bytes
            k = entries.index(r0)
            end_rom = ro_rom + len(ro)
            j = k
            while int(ENTRY.match(lines[entries[j]]).group(2), 16) < end_rom:
                j += 1
            if int(ENTRY.match(lines[entries[j]]).group(2), 16) != end_rom:
                report.append((name, ".rodata ends inside a yaml subsegment"))
                continue
            span = entries[k:j]
            own = {ro_at[v] for v in has_ro}
            others = [lines[i] for i in span if i not in own]
            if any(", .rodata, code/func_" in o or ", c, " in o for o in others):
                report.append((name, ".rodata covers another file's C .rodata"))
                continue
            if any(v in ro_at and ro_at[v] not in span for v in funcs):
                report.append((name, "a function's .rodata lies outside the object's"))
                continue
            bad = same(ro, rom[ro_rom:end_rom], rrel)
            if bad is not None:
                report.append((name, f".rodata differs at +{bad:#x}"))
                continue
            ro_span = (span[0], span[-1])
        elif has_ro:
            report.append((name, "the game's C has .rodata, ultralib's none"))
            continue
        rel = src.relative_to(UL / "src").with_suffix("").as_posix()
        taken.append((name, rel, (t0, t1), ro_span, funcs))
        report.append((name, "taken"))
    return taken, report, lines


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--write", action="store_true")
    args = ap.parse_args()
    taken, report, lines = plan()
    for name, why in report:
        print(f"{name:40s} {why}")
    print(f"{len(taken)} of {len(report)} modules taken")
    if not args.write:
        return
    edits = []          # (first line, last line, replacement)
    for name, rel, (t0, t1), ro_span, funcs in taken:
        ind = ENTRY.match(lines[t0]).group(1)
        rom0 = ENTRY.match(lines[t0]).group(2)
        edits.append((t0, t1, f"{ind}- [0x{rom0}, c, ../../lib/ultralib/src/{rel}]\n"))
        if ro_span:
            r0 = ENTRY.match(lines[ro_span[0]])
            edits.append((ro_span[0], ro_span[1],
                          f"{r0.group(1)}- [0x{r0.group(2)}, .rodata, ../../lib/ultralib/src/{rel}]\n"))
    for a, b, rep in sorted(edits, reverse=True):
        lines[a:b + 1] = [rep]
    YAML.write_text("".join(lines))
    out = ["// libultra modules linked from lib/ultralib (decompals/ultralib, 2.0I libgultra_rom)",
           "// instead of the game's C; written by tools/ultralib_build.py --write (docs/LIBULTRA.md).",
           "// module = ultralib file: the game files it replaces in the N64 build"]
    for name, rel, _, _, funcs in taken:
        out.append(f"{name} = src/{rel}.c: " + " ".join(f"func_{v:08X}.c" for v in funcs))
    LIST.write_text("\n".join(out) + "\n")
    print(f"wrote {YAML.relative_to(ROOT)} and {LIST.relative_to(ROOT)}")


if __name__ == "__main__":
    main()
