#!/usr/bin/env python3
"""Check a C file that emits its own .rodata against the ROM, linked.

tools/asm_diff.py proves an edit leaves the code alone, but turning an extern
`D_X` into the literal it names changes the assembly by design: the load now
goes through the file's own pool (`$LC0`, or the pool tools/sn_li.py writes
for a `li.s`). What has to hold then is the linked result: the file's .rodata
is the ROM's bytes at the slice the yaml gives it, and every word of its code,
with each relocation resolved (named symbols through the linked ELF, the
file's own .rodata through the slice's address), is the ROM's word.

This compiles each file the way the Makefile does (cpp, cc1 with the file's
own `/* cc1-flags: */`, tools/sn_as.sh), places its .rodata where

  - the yaml puts it (`[0xROM, .rodata, code/<file>]`), or
  - versions/shared_rodata.us.txt puts it (a second-module copy, NOLOAD over
    the first module's pool; its jump-table entries are held by their
    distance from the table's first entry, as `shared_rodata.py --check`
    does), or
  - `--at 0x800C0540` says (a draft, before the yaml has the slice),

and compares. No build is needed: functions are found in build/turok2.us.elf
(or MATCH_ELF), which links byte-exact.

Usage:
    python3 tools/rodata_check.py src/us/code/func_004077AC.c [...]
    python3 tools/rodata_check.py --at 0x800C0540 src/us/code/func_004077AC.c
    python3 tools/rodata_check.py -v ...      print the differing words

Exit status: 0 when every file matches.
"""

import argparse
import os
import re
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
from match_func import rom_symbols  # noqa: E402

ELF = Path(os.environ.get("MATCH_ELF", ROOT / "build" / "turok2.us.elf"))
ROM = ROOT / "baserom.us.z64"
YAML = ROOT / "versions" / "turok2.us.yaml"
SHARED = ROOT / "versions" / "shared_rodata.us.txt"
CC1 = ROOT / "tools" / "sn64" / "cc1"
CPP_FLAGS = ["-P", "-undef", "-Wall", "-lang-c", "-D_LANGUAGE_C", "-DF3DEX_GBI_2",
             "-D__GNUC__=2", '-DGAME_VERSION="us"', "-nostdinc"]
CC_FLAGS = "-quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mno-fix4300 -mhard-float -mfp64"
FLAGS_LINE = re.compile(r"^/\* cc1-flags: (.*) \*/$", re.M)
KSEG0_ROM = 0x7FFFF400          # vram - ROM offset of both .rodata pools
R_MIPS_32, R_MIPS_26, R_MIPS_HI16, R_MIPS_LO16 = 2, 4, 5, 6
SYMDEF = re.compile(r"^\s*(\.?\w+)\s*=\s*0x([0-9A-Fa-f]+)\s*;", re.M)


def compile_c(src, out):
    inc = ["-I" + str(ROOT), "-I" + str(ROOT / "include"), "-I" + str(ROOT / "us" / "include")]
    i, s = out.with_suffix(".i"), out.with_suffix(".s")
    subprocess.run(["cpp", *CPP_FLAGS, *inc, "-o", str(i), str(src)], check=True)
    m = FLAGS_LINE.search(src.read_text(errors="replace"))
    flags = (m.group(1) if m else CC_FLAGS).split()
    subprocess.run([str(CC1), *flags, "-o", str(s), str(i)], check=True,
                   stderr=subprocess.DEVNULL)
    # gas warns about $at and odd float registers on every file; keep quiet
    subprocess.run([str(ROOT / "tools" / "sn_as.sh"), str(s), str(out)], check=True,
                   stderr=subprocess.DEVNULL, env={**os.environ, "INC_DIR": str(ROOT / "us" / "include")})


def symbol_table():
    out = {}
    for p in ("undefined_syms_auto.us.txt", "undefined_funcs_extra.us.txt",
              "symbols_manual.us.txt", "names.us.txt"):
        f = ROOT / "versions" / p
        if f.exists():
            for name, addr in SYMDEF.findall(f.read_text()):
                out.setdefault(name, int(addr, 16))
    with ELF.open("rb") as fh:
        for sym in ELFFile(fh).get_section_by_name(".symtab").iter_symbols():
            if sym.name and sym["st_shndx"] != "SHN_UNDEF":
                out[sym.name] = sym["st_value"]
    return out


def placement(stem, at):
    """(vram of the file's .rodata, shared?) or None."""
    if at is not None:
        return at, False
    m = re.search(rf"^\s+- \[(0x[0-9A-Fa-f]+),\s*\.rodata,\s*code/{stem}\]", YAML.read_text(), re.M)
    if m:
        return int(m.group(1), 16) + KSEG0_ROM, False
    for line in SHARED.read_text().splitlines():
        f = line.split("#")[0].split()
        if len(f) == 2 and f[0] == stem:
            return int(f[1], 16), True
    return None


def word(b, i):
    return struct.unpack(">I", b[i:i + 4])[0]


def check(path, at, syms, rsyms, rom, verbose):
    stem = path.stem
    bad = []
    with tempfile.TemporaryDirectory(dir=os.environ.get("TMPDIR")) as tmp:
        obj = Path(tmp) / "f.o"
        try:
            compile_c(path, obj)
        except subprocess.CalledProcessError as e:
            print(f"FAIL {path}: does not compile ({e.cmd[0]})")
            return False
        with obj.open("rb") as fh:
            elf = ELFFile(fh)
            secs = list(elf.iter_sections())
            idx = {s.name: i for i, s in enumerate(secs)}
            symtab = elf.get_section_by_name(".symtab")
            text = secs[idx[".text"]].data()
            ro = secs[idx[".rodata"]].data() if ".rodata" in idx else b""
            for extra in (".data", ".bss", ".sdata", ".sbss"):
                if extra in idx and secs[idx[extra]]["sh_size"]:
                    bad.append(f"the object has {extra}")
            funcs = sorted((s["st_value"], s.name) for s in symtab.iter_symbols()
                           if s["st_shndx"] == idx[".text"] and s["st_info"]["type"] == "STT_FUNC")
            # The file's .text vram, from its first function's linked address.
            text_vram = None
            for off, name in funcs:
                if name in syms:
                    text_vram = syms[name] - off
                    break
            if text_vram is None:
                print(f"FAIL {path}: none of its functions is in {ELF.name}")
                return False
            ro_vram, shared = (None, False)
            if ro:
                pl = placement(stem, at)
                if pl is None:
                    print(f"FAIL {path}: it has .rodata and no slice (yaml, shared_rodata, or --at)")
                    return False
                ro_vram, shared = pl

            def sym_value(r):
                s = symtab.get_symbol(r["r_info_sym"])
                if s["st_info"]["type"] == "STT_SECTION" or s["st_shndx"] != "SHN_UNDEF":
                    sec = secs[s["st_shndx"]].name
                    base = {".text": text_vram, ".rodata": ro_vram}.get(sec)
                    if base is None:
                        raise KeyError(sec)
                    return base + (s["st_value"] if s["st_info"]["type"] != "STT_SECTION" else 0)
                if s.name not in syms:
                    raise KeyError(s.name)
                return syms[s.name]

            def relocs(target):
                out = []
                for s in secs:
                    if isinstance(s, RelocationSection) and s["sh_info"] == idx.get(target):
                        out += list(s.iter_relocations())
                return sorted(out, key=lambda r: r["r_offset"])

            # .rodata against the ROM.
            if ro:
                buf = bytearray(ro)
                rel32 = {}
                for r in relocs(".rodata"):
                    if r["r_info_type"] != R_MIPS_32:
                        bad.append(f".rodata reloc type {r['r_info_type']}")
                        continue
                    o = r["r_offset"]
                    rel32[o] = (word(buf, o) + sym_value(r)) & 0xFFFFFFFF
                    buf[o:o + 4] = struct.pack(">I", rel32[o])
                rom_off = ro_vram - KSEG0_ROM
                first = None
                for o in range(0, len(buf), 4):
                    want = rom[rom_off + o:rom_off + o + 4]
                    got = bytes(buf[o:o + 4])
                    if o in rel32 and shared:
                        if first is None:
                            first = o
                        elif word(rom, rom_off + o) - word(rom, rom_off + first) != rel32[o] - rel32[first]:
                            bad.append(f".rodata+0x{o:X}: jump-table distance")
                        continue
                    first = None
                    if len(got) < 4:
                        got = got + bytes(4 - len(got))   # the linker pads to SUBALIGN 4 with zeros
                    if got != want:
                        bad.append(f".rodata 0x{ro_vram + o:08X}: ROM {want.hex()} C {got.hex()}")

            # .text against the ROM, every relocation resolved.
            buf = bytearray(text)
            rl = relocs(".text")
            for k, r in enumerate(rl):
                o, t = r["r_offset"], r["r_info_type"]
                w = word(buf, o)
                try:
                    S = sym_value(r)
                except KeyError as e:
                    bad.append(f".text+0x{o:X}: unknown symbol {e}")
                    continue
                if t == R_MIPS_26:
                    v = (w & 0xFC000000) | ((((w & 0x03FFFFFF) << 2) + S) >> 2 & 0x03FFFFFF)
                elif t == R_MIPS_LO16:
                    v = (w & 0xFFFF0000) | ((S + (w & 0xFFFF)) & 0xFFFF)
                elif t == R_MIPS_HI16:
                    lo = next((q for q in rl[k + 1:] if q["r_info_type"] == R_MIPS_LO16
                               and q["r_info_sym"] == r["r_info_sym"]), None)
                    lo_imm = word(buf, lo["r_offset"]) & 0xFFFF if lo else 0
                    lo_imm -= 0x10000 if lo_imm & 0x8000 else 0
                    a = ((w & 0xFFFF) << 16) + lo_imm
                    v = (w & 0xFFFF0000) | (((S + a + 0x8000) >> 16) & 0xFFFF)
                elif t == R_MIPS_32:
                    v = (w + S) & 0xFFFFFFFF
                else:
                    bad.append(f".text+0x{o:X}: reloc type {t}")
                    continue
                buf[o:o + 4] = struct.pack(">I", v)
            for n, (off, name) in enumerate(funcs):
                end = funcs[n + 1][0] if n + 1 < len(funcs) else len(buf)
                key = name if name in rsyms else None
                if key is None:
                    bad.append(f"{name}: not in the ROM symbol table")
                    continue
                rom_off, size = rsyms[key]
                if end - off != size:
                    bad.append(f"{name}: size 0x{end - off:X}, ROM 0x{size:X}")
                for i in range(0, min(size, end - off), 4):
                    if buf[off + i:off + i + 4] != rom[rom_off + i:rom_off + i + 4]:
                        bad.append(f"{name}+0x{i:X}: ROM {rom[rom_off + i:rom_off + i + 4].hex()} "
                                   f"C {bytes(buf[off + i:off + i + 4]).hex()}")
    where = "" if not ro else f" (.rodata 0x{len(ro):X} bytes at 0x{ro_vram:08X}{', shared' if shared else ''})"
    if bad:
        print(f"DIFF {path}{where}: {len(bad)} differences")
        for b in bad[:40 if verbose else 3]:
            print("    " + b)
        return False
    print(f"OK   {path}{where}")
    return True


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("files", nargs="+", type=Path)
    ap.add_argument("--at", type=lambda s: int(s, 16), help="vram of the .rodata (a draft)")
    ap.add_argument("-v", "--verbose", action="store_true")
    args = ap.parse_args()
    syms = symbol_table()
    rsyms = rom_symbols()
    rom = ROM.read_bytes()
    ok = all([check(f.resolve(), args.at, syms, rsyms, rom, args.verbose) for f in args.files])
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())
