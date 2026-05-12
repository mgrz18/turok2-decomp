#!/usr/bin/env python3
"""
vmasm_decode.py — locate and pretty-print the VMASM overlay dispatch table.

Background
----------
Turok 2 (and the rest of the Acclaim Studios London engine family — Turok 3,
Armorines, Rage Wars) hot-loads three code overlays at runtime via a small
dispatcher named VMASM.S in the original sources. The dispatcher reads a
static table that pairs each overlay's ROM offset with a fixed VRAM load
address, then does an osPiStartDma() to bring it in.

Splat doesn't understand runtime-relocated code: by default it assigns one
sequential VRAM to the whole `virtual` segment. The unresolved-reference
spray that follows (~1820 refs in the 0x80210000-0x802A50C4 range) is the
symptom.

This script:
  * scans the ROM for plausible dispatch-table candidates,
  * prints the table if found, and
  * (with --yaml-snippet) emits the YAML segments matching the discovered
    load addresses.

If no table is auto-discovered, the script still prints the load addresses
we adopted by gap-analysis of the unresolved cluster (the Round-5 fallback,
documented in docs/VMASM.md), so the YAML snippet is always usable.

Usage
-----
    python3 tools/vmasm_decode.py
    python3 tools/vmasm_decode.py --yaml-snippet
    python3 tools/vmasm_decode.py --table-offset 0x17A600
    python3 tools/vmasm_decode.py --symbols   # emit symbol_addrs lines
"""
import argparse
import struct
import sys
from pathlib import Path

# Adopted Round-5 layout (see docs/VMASM.md "Pass 5"). Each entry:
#     (name, rom_start, rom_end, vram_load)
# rom_end is exclusive. Sizes come from versions/turok2.us.yaml.
ROUND5_MODULES = [
    ("virtual_text_0",   0x13B5F0, 0x17A600, 0x8013AAF0),
    ("virtual_rodata_0", 0x17A600, 0x186000, None),
    ("virtual_text_1",   0x186000, 0x1B3400, 0x80210000),
    ("virtual_rodata_1", 0x1B3400, 0x1BFA00, None),
    ("virtual_text_2",   0x1BFA00, 0x1DEE00, 0x80280000),
    ("virtual_rodata_2", 0x1DEE00, 0x20E334, None),
]


def find_table_candidates(rom: bytes):
    """Scan ROM for 4-uint32-BE rows that look like (rom, vram, size, entry).

    Heuristic: a row matches if its first word equals one of the known
    overlay rom_starts (0x186000 or 0x1BFA00) and its second word is in
    the kernel VRAM range 0x80000000..0x80800000.
    """
    candidates = []
    rom_starts = {0x186000, 0x1BFA00, 0x13B5F0}
    for off in range(0, len(rom) - 16, 4):
        a, b, c, d = struct.unpack(">4I", rom[off:off + 16])
        if a in rom_starts and 0x80000000 <= b < 0x80800000 and c < 0x100000:
            candidates.append((off, a, b, c, d))
    return candidates


def search_binary_targets(rom: bytes):
    """Locate raw BE encodings of expected rom-offset / vram literals."""
    targets = {
        b"\x00\x18\x60\x00": "rom_start 0x186000  (text_1)",
        b"\x00\x1B\xFA\x00": "rom_start 0x1BFA00  (text_2)",
        b"\x00\x13\xB5\xF0": "rom_start 0x13B5F0  (text_0)",
        b"\x80\x21\x00\x00": "vram_load 0x80210000 (text_1 adopted)",
        b"\x80\x28\x00\x00": "vram_load 0x80280000 (text_2 adopted)",
        b"\x80\x13\xAA\xF0": "vram_load 0x8013AAF0 (text_0)",
    }
    out = {}
    for be, label in targets.items():
        i, hits = 0, []
        while True:
            j = rom.find(be, i)
            if j < 0:
                break
            hits.append(j)
            i = j + 1
        out[label] = hits
    return out


def print_table():
    print("VMASM overlay dispatch (Round 5 adopted layout):")
    print(f"  {'module':<18} {'rom_start':>10} {'rom_end':>10} "
          f"{'size':>8} {'vram_load':>12}")
    for name, a, b, vram in ROUND5_MODULES:
        size = b - a
        vram_s = f"{vram:#x}" if vram is not None else "(in-place)"
        print(f"  {name:<18} {a:#010x} {b:#010x} {size:#8x} {vram_s:>12}")


def emit_yaml_snippet():
    lines = []
    for name, a, b, vram in ROUND5_MODULES:
        if vram is not None:
            lines.append(f"  - name: {name}")
            lines.append("    type: code")
            lines.append(f"    start: {a:#x}")
            lines.append(f"    vram: {vram:#x}")
            lines.append("    subsegments:")
            lines.append(f"      - [{a:#x}, asm]")
        else:
            lines.append(f"  - name: {name}")
            lines.append("    type: bin")
            lines.append(f"    start: {a:#x}")
        lines.append("")
    return "\n".join(lines)


def emit_symbol_addrs():
    """Stub: no symbols are produced by this pass; reserved for future use."""
    return "// vmasm_decode: no extra symbols emitted in Round 5 layout.\n"


def main():
    p = argparse.ArgumentParser(description=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument("--rom", default="baserom.us.z64",
                   help="path to baserom (default: baserom.us.z64)")
    p.add_argument("--table-offset", type=lambda s: int(s, 0), default=None,
                   help="override the dispatch-table location (hex ok)")
    p.add_argument("--scan", action="store_true",
                   help="run the candidate-table scan and print top matches")
    p.add_argument("--yaml-snippet", action="store_true",
                   help="emit YAML segments for the virtual overlays")
    p.add_argument("--symbols", action="store_true",
                   help="emit symbol_addrs.us.txt-compatible lines")
    args = p.parse_args()

    if args.yaml_snippet:
        print(emit_yaml_snippet())
        return 0
    if args.symbols:
        print(emit_symbol_addrs())
        return 0

    rom_path = Path(args.rom)
    if not rom_path.is_file():
        print(f"error: rom not found at {rom_path}", file=sys.stderr)
        return 1
    rom = rom_path.read_bytes()
    print(f"loaded {rom_path} ({len(rom):,} bytes)\n")

    if args.table_offset is not None:
        off = args.table_offset
        print(f"dumping 4 rows at {off:#x}:")
        for i in range(4):
            row = struct.unpack(">4I", rom[off + i*16: off + (i+1)*16])
            print(f"  row[{i}] = " + "  ".join(f"{w:#010x}" for w in row))
        print()

    if args.scan:
        cs = find_table_candidates(rom)
        print(f"plausible 4-word rows: {len(cs)}")
        for off, a, b, c, d in cs[:20]:
            print(f"  @ {off:#x}  rom={a:#x}  vram={b:#x}  size={c:#x}  "
                  f"entry={d:#x}")
        if not cs:
            print("(no candidates — table likely lives in a "
                  "data section we have not segmented yet, or is generated "
                  "at runtime from LNK records)")
        print()
        print("byte-pattern hits for known literals:")
        for label, hits in search_binary_targets(rom).items():
            head = ", ".join(f"{h:#x}" for h in hits[:6])
            more = f" (+{len(hits)-6} more)" if len(hits) > 6 else ""
            print(f"  {label}: {len(hits)} hit(s)  {head}{more}")
        print()

    print_table()
    return 0


if __name__ == "__main__":
    sys.exit(main())
