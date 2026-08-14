#!/usr/bin/env python3
"""Recover function entry points from the ROM and emit splat symbol seeds.

splat only disassembles a region as code where it knows functions start. Where
it doesn't, it bails to `.word` -- which is why `virtual_text_1` came out with
zero instructions despite being 181 KB of confirmed MIPS.

Two signals, strongest first:

1. `jal` targets. A `jal` encodes its destination absolutely, so every one of
   them names a real function entry. This is the same signal N64Recomp's own
   function finder uses, and it reaches into regions we have not decoded yet.

2. Prologues after a return. A word matching `addiu sp, sp, -X` whose
   instruction two slots back is `jr ra` sits right after a function epilogue,
   so it starts the next function. This catches functions nothing calls
   directly (jump-table and function-pointer targets).

Targets landing outside any known segment VRAM are reported separately: they
are the evidence for where the runtime overlay banks actually live.

Usage:
    python3 tools/function_seed.py                 # report only
    python3 tools/function_seed.py --write         # write the seed file
"""

import argparse
import struct
import sys
from collections import Counter
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
ROM = ROOT / "baserom.us.z64"
YAML = ROOT / "versions" / "turok2.us.yaml"
OUT = ROOT / "versions" / "symbols_from_scan.us.txt"
EXISTING = ROOT / "versions" / "symbol_addrs.us.txt"

JAL_OPCODE = 0x03          # 000011
JR_RA = 0x03E00008
KSEG0 = 0x80000000


def load_code_segments():
    """Return [(rom_start, rom_end, vram, name)] for *code* subsegments only.

    Scanning whole top-level segments is wrong: a `code` segment also holds
    rodata subsegments, and random data words masquerade as `jal` often enough
    to bury the real signal (an early run produced ~200 bogus banks up in
    0x8F______ that way).
    """
    import yaml

    with YAML.open() as fh:
        doc = yaml.safe_load(fh)

    tops = []
    for seg in doc["segments"]:
        tops.append({"start": seg[0], "type": None} if isinstance(seg, list) else seg)
    tops.sort(key=lambda s: s["start"])

    # Flatten every subsegment so each one's end is the next one's start.
    flat = []
    for i, seg in enumerate(tops):
        seg_end = tops[i + 1]["start"] if i + 1 < len(tops) else seg["start"]
        subs = seg.get("subsegments") if isinstance(seg, dict) else None
        if not subs:
            flat.append((seg["start"], seg_end, seg.get("type"), seg, seg.get("name", "?")))
            continue
        starts = [s[0] if isinstance(s, list) else s["start"] for s in subs]
        for j, sub in enumerate(subs):
            if not isinstance(sub, list):
                continue
            start = sub[0]
            kind = sub[1] if len(sub) > 1 else seg.get("type")
            name = sub[2] if len(sub) > 2 else f"{start:X}"
            end = starts[j + 1] if j + 1 < len(starts) else seg_end
            flat.append((start, end, kind, seg, name))

    out = []
    for start, end, kind, parent, name in flat:
        if kind not in ("asm", "hasm") or "vram" not in parent:
            continue
        vram = parent["vram"] + (start - parent["start"])
        out.append((start, end, vram, name))
    return out


def rom_to_vram(segments, off):
    for start, end, vram, _ in segments:
        if start <= off < end:
            return vram + (off - start)
    return None


def vram_to_segment(segments, addr):
    for start, end, vram, name in segments:
        if vram <= addr < vram + (end - start):
            return name
    return None


def scan(rom, segments):
    jal_targets = set()
    prologues = set()
    outside = Counter()
    outside_targets = set()

    for start, end, vram, _name in segments:
        end = min(end, len(rom) - 3)
        for off in range(start, end, 4):
            word = struct.unpack_from(">I", rom, off)[0]

            if (word >> 26) == JAL_OPCODE:
                target = KSEG0 | ((word & 0x03FFFFFF) << 2)
                if vram_to_segment(segments, target):
                    jal_targets.add(target)
                else:
                    outside[target >> 16] += 1
                    outside_targets.add(target)

            if (word >> 16) == 0x27BD and (word & 0xFFFF) >= 0x8000:
                # function start only if the previous instruction pair is a
                # return: `jr ra` at off-8, its delay slot at off-4.
                if off - 8 >= start:
                    prev = struct.unpack_from(">I", rom, off - 8)[0]
                    if prev == JR_RA:
                        prologues.add(vram + (off - start))

    return jal_targets, prologues, outside, outside_targets


def load_existing():
    known = set()
    if not EXISTING.exists():
        return known
    for line in EXISTING.read_text(errors="replace").splitlines():
        line = line.strip()
        if line.startswith("//") or "=" not in line:
            continue
        try:
            known.add(int(line.split("=")[1].split(";")[0].strip(), 16))
        except ValueError:
            continue
    return known


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--write", action="store_true", help="escribir el archivo de seeds")
    args = ap.parse_args()

    if not ROM.exists():
        sys.exit(f"falta {ROM}")

    rom = ROM.read_bytes()
    segments = load_code_segments()
    jal_targets, prologues, outside, outside_targets = scan(rom, segments)

    known = load_existing()
    seeds = sorted((jal_targets | prologues) - known)

    print(f"segmentos de codigo      : {len(segments)}")
    print(f"targets de jal           : {len(jal_targets):,}")
    print(f"prologos tras `jr ra`    : {len(prologues):,}")
    print(f"union                    : {len(jal_targets | prologues):,}")
    print(f"ya conocidos (symbol_addrs): {len(known):,}")
    print(f"seeds nuevos             : {len(seeds):,}")
    print()

    per_segment = Counter()
    for addr in seeds:
        per_segment[vram_to_segment(segments, addr) or "?"] += 1
    print("seeds por segmento:")
    for name, n in per_segment.most_common():
        print(f"  {name:<22}{n:>8,}")

    if outside_targets:
        print()
        print("Funciones FUERA de todo segmento conocido (bancos de overlay).")
        print("Son targets de jal, o sea direcciones absolutas ciertas.")
        print()
        # Agrupar en bancos contiguos: un hueco > 64 KB corta el banco.
        ordered = sorted(outside_targets)
        banks, cur = [], [ordered[0], ordered[0]]
        for addr in ordered[1:]:
            if addr - cur[1] > 0x10000:
                banks.append(tuple(cur))
                cur = [addr, addr]
            else:
                cur[1] = addr
        banks.append(tuple(cur))

        print(f"{'banco':<28}{'funciones':>11}{'refs':>10}{'tamano':>10}")
        for lo, hi in banks:
            n = sum(1 for a in ordered if lo <= a <= hi)
            refs = sum(v for k, v in outside.items() if (lo >> 16) <= k <= (hi >> 16))
            print(f"  0x{lo:08X}-0x{hi:08X}{n:>11,}{refs:>10,}{hi - lo:>10,}")
        print()
        print(f"  total funciones en overlay: {len(outside_targets):,}")

    if args.write:
        lines = ["// Generado por tools/function_seed.py — no editar a mano.",
                 "// Entradas de funcion recuperadas por targets de jal + prologos tras `jr ra`.",
                 ""]
        lines += [f"func_{a:08X} = 0x{a:08X}; // type:func" for a in seeds]
        OUT.write_text("\n".join(lines) + "\n")
        print()
        print(f"escrito: {OUT.relative_to(ROOT)} ({len(seeds):,} simbolos)")


if __name__ == "__main__":
    main()
