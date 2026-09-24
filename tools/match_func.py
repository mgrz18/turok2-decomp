#!/usr/bin/env python3
"""Compile a C file and check each function in it against the original ROM.

The matching loop needs one question answered fast: does this C, through this
compiler, produce the bytes the game shipped? This compiles the file once per
requested compiler (in the turok2-build container, via tools/cc_func.sh) and
compares every function the object defines with the same function in the ROM.

The reference comes from the ROM, not from the object's neighbours: the
function is looked up by name in build/turok2.us.elf, which links byte-exact,
and its program header gives the ROM offset.

Relocated fields are masked before comparing. The object is unlinked, so a
`jal` target or a %hi/%lo pair is zero in it and filled in by the linker; what
has to match is everything else. A match here means the function will match in
the linked ROM provided its symbols resolve to the same addresses, which they
will once the C replaces the asm at the same place.

Usage:
    python3 tools/match_func.py src/test/foo.c
    python3 tools/match_func.py src/test/foo.c --cc cc1 cc1n64.exe --opt -O2
"""

import argparse
import re
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection

ROOT = Path(__file__).resolve().parent.parent
ELF = ROOT / "build" / "turok2.us.elf"
ROM = ROOT / "baserom.us.z64"
IMAGE = "turok2-build"

R_MIPS_26, R_MIPS_HI16, R_MIPS_LO16 = 4, 5, 6

REGS = ("zero at v0 v1 a0 a1 a2 a3 t0 t1 t2 t3 t4 t5 t6 t7 "
        "s0 s1 s2 s3 s4 s5 s6 s7 t8 t9 k0 k1 gp sp fp ra").split()


def rom_symbols():
    """name -> (rom offset, size) for every sized function in the linked ELF."""
    out = {}
    with ELF.open("rb") as fh:
        elf = ELFFile(fh)
        loads = [s for s in elf.iter_segments() if s["p_type"] == "PT_LOAD"]
        for sym in elf.get_section_by_name(".symtab").iter_symbols():
            if sym["st_info"]["type"] != "STT_FUNC" or not sym["st_size"]:
                continue
            v = sym["st_value"]
            for seg in loads:
                if seg["p_vaddr"] <= v < seg["p_vaddr"] + seg["p_filesz"]:
                    out[sym.name] = (seg["p_paddr"] + v - seg["p_vaddr"], sym["st_size"])
                    break
    return out


def object_functions(path):
    """name -> (words, {word index: (reloc type, symbol name)}) per function in .text."""
    with open(path, "rb") as fh:
        elf = ELFFile(fh)
        text_idx = None
        for i, sec in enumerate(elf.iter_sections()):
            if sec.name == ".text":
                text_idx, text = i, sec.data()
        if text_idx is None:
            return {}
        relocs = {}
        symtab = elf.get_section_by_name(".symtab")
        for sec in elf.iter_sections():
            if isinstance(sec, RelocationSection) and sec["sh_info"] == text_idx:
                for r in sec.iter_relocations():
                    sym = symtab.get_symbol(r["r_info_sym"])
                    relocs[r["r_offset"] // 4] = (r["r_info_type"], sym.name if sym else "")
        funcs = {}
        syms = [s for s in elf.get_section_by_name(".symtab").iter_symbols()
                if s["st_shndx"] == text_idx and s.name and not s.name.startswith(".")]
        syms.sort(key=lambda s: s["st_value"])
        for i, s in enumerate(syms):
            start = s["st_value"]
            end = s["st_value"] + s["st_size"] if s["st_size"] else (
                syms[i + 1]["st_value"] if i + 1 < len(syms) else len(text))
            words = struct.unpack(f">{(end - start) // 4}I", text[start:end])
            funcs[s.name] = (words, {k - start // 4: t for k, t in relocs.items()
                                     if start // 4 <= k < end // 4})
    return funcs


def mask(word, rel):
    rtype = rel[0] if isinstance(rel, tuple) else rel
    if rtype == R_MIPS_26:
        return word & 0xFC000000
    if rtype in (R_MIPS_HI16, R_MIPS_LO16):
        return word & 0xFFFF0000
    return word


ADDR_NAME = re.compile(r"^(?:D|func|jtbl|entry)_([0-9A-F]{8})$")


def reloc_target_ok(got, want, rel):
    """Check what a relocation resolves to, not just the rest of the word.

    Masking the whole field hides the addend: m2c writes `&D_800A5858 + 4`
    meaning four bytes, C scales it by the pointee size, and the word still
    masks equal. The symbol names here encode their address, so the linked
    value can be computed and held against the ROM. Names that do not encode
    an address (a static label, a section symbol) are left to `make verify`.
    """
    if not rel:
        return True
    rtype, name = rel
    m = ADDR_NAME.match(name or "")
    if not m:
        return True
    addr = int(m.group(1), 16)
    if rtype == R_MIPS_LO16:
        imm = got & 0xFFFF
        imm -= 0x10000 if imm & 0x8000 else 0
        return ((addr + imm) & 0xFFFF) == (want & 0xFFFF)
    if rtype == R_MIPS_26:
        target = (addr + ((got & 0x03FFFFFF) << 2)) & 0x0FFFFFFF
        return (target >> 2) == (want & 0x03FFFFFF)
    return True


def disasm(word):
    try:
        import rabbitizer
        return rabbitizer.Instruction(word).disassemble()
    except Exception:
        return f".word 0x{word:08X}"


def compare(name, got, relocs, rom, rom_off, rom_size, verbose):
    want = struct.unpack(f">{rom_size // 4}I", rom[rom_off:rom_off + rom_size])
    n = max(len(got), len(want))
    bad = []
    for i in range(n):
        g = got[i] if i < len(got) else None
        w = want[i] if i < len(want) else None
        rt = relocs.get(i)
        if g is None or w is None or mask(g, rt) != mask(w, rt) or not reloc_target_ok(g, w, rt):
            bad.append(i)
    status = "MATCH" if not bad else f"{len(bad)} of {n} words differ"
    size_note = "" if len(got) * 4 == rom_size else f" (size {len(got) * 4:#x} vs {rom_size:#x})"
    print(f"  {name:28s} {status}{size_note}")
    if bad and verbose:
        for i in range(n):
            g = got[i] if i < len(got) else None
            w = want[i] if i < len(want) else None
            mark = "!" if i in bad else " "
            gs = disasm(g) if g is not None else "-"
            ws = disasm(w) if w is not None else "-"
            print(f"    {mark} {i * 4:04x}  {ws:40s} | {gs}")
    return not bad


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("c_file", type=Path)
    ap.add_argument("--cc", nargs="+", default=["cc1"],
                    help="compilers in tools/sn64 to try")
    ap.add_argument("--opt", default="-O2")
    ap.add_argument("--assembler", choices=["asn64", "gas"], default="gas",
                    help="gas is the fast path (tools/sn_as.sh); asn64 is the original")
    ap.add_argument("-v", "--verbose", action="store_true", help="show word diffs")
    args = ap.parse_args()

    rel = args.c_file.resolve().relative_to(ROOT)
    syms = rom_symbols()
    rom = ROM.read_bytes()
    all_ok = True
    with tempfile.TemporaryDirectory(dir=ROOT / "build") as tmp:
        tmp_rel = Path(tmp).relative_to(ROOT)
        script = " && ".join(
            f"tools/cc_func.sh {rel} {tmp_rel}/{cc}.o {cc} {args.opt}" for cc in args.cc)
        res = subprocess.run(
            ["docker", "run", "--platform=linux/amd64", "--rm", "-v", f"{ROOT}:/work",
             "-w", "/work",
             # A fresh wine prefix per container crashes asn64 while wine is
             # still initialising it; keep one prefix in build/ across runs.
             "-e", "WINEPREFIX=/work/build/.wineprefix", "-e", "WINEDEBUG=-all",
             "-e", f"ASSEMBLER={args.assembler}",
             IMAGE, "bash", "-c", script],
            capture_output=True, text=True)
        if res.returncode:
            print(res.stdout, res.stderr, file=sys.stderr)
            return 1
        for cc in args.cc:
            print(f"{cc} {args.opt}:")
            for name, (words, relocs) in object_functions(Path(tmp) / f"{cc}.o").items():
                if name not in syms:
                    print(f"  {name:28s} not in the ROM symbol table")
                    continue
                off, size = syms[name]
                all_ok &= compare(name, words, relocs, rom, off, size, args.verbose)
    return 0 if all_ok else 2


if __name__ == "__main__":
    sys.exit(main())
