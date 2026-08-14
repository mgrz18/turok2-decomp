#!/usr/bin/env python3
"""Report decode + symbol coverage for the splat output.

The project targets a N64Recomp-consumable ELF, so progress is measured by how
much of the ROM splat turns into delimited functions -- not by how many
functions match byte-exact. This script is the yardstick for that.

Denominator for each asm region is the number of MIPS function prologues
(`addiu sp, sp, -X`) findable in the ROM slice, which is a lower bound on the
real function count (leaf functions that never touch sp are missed).

Usage:
    python3 tools/metrics.py                    # human-readable table
    python3 tools/metrics.py --json             # machine-readable
    python3 tools/metrics.py --baseline b.json  # diff against a saved run
"""

import argparse
import json
import re
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
ROM = ROOT / "baserom.us.z64"
YAML = ROOT / "versions" / "turok2.us.yaml"
ASM_DIR = ROOT / "us" / "asm"
SYMBOL_ADDRS = ROOT / "versions" / "symbol_addrs.us.txt"

# Lines splat emits for instructions are indented; directives sit at column 0.
INSTRUCTION_RE = re.compile(r"^\s{2,}[a-z]")
LABEL_RE = re.compile(r"^\s*\.globl\b|^glabel\b")


def load_segments():
    """Flatten the splat yaml into (start, end, kind, name) tuples, sorted."""
    import yaml

    with YAML.open() as fh:
        doc = yaml.safe_load(fh)

    entries = []
    for seg in doc["segments"]:
        if isinstance(seg, list):  # the trailing [0x2000000] end marker
            entries.append((seg[0], None, None))
            continue
        subs = seg.get("subsegments")
        if not subs:
            entries.append((seg["start"], seg.get("type"), seg.get("name")))
            continue
        for sub in subs:
            if isinstance(sub, list):
                start = sub[0]
                kind = sub[1] if len(sub) > 1 else seg.get("type")
                name = sub[2] if len(sub) > 2 else f"{start:X}"
                entries.append((start, kind, name))

    entries.sort(key=lambda e: e[0])
    out = []
    for i, (start, kind, name) in enumerate(entries):
        end = entries[i + 1][0] if i + 1 < len(entries) else start
        if kind is not None:
            out.append((start, end, kind, name))
    return out


def count_prologues(rom: bytes, start: int, end: int) -> int:
    """Count `addiu sp, sp, -X` words in a ROM slice."""
    n = 0
    for off in range(start, min(end, len(rom)) - 3, 4):
        word = struct.unpack_from(">I", rom, off)[0]
        if (word >> 16) == 0x27BD and (word & 0xFFFF) >= 0x8000:
            n += 1
    return n


def scan_asm_file(path: Path):
    total = words = instructions = labels = 0
    with path.open(errors="replace") as fh:
        for line in fh:
            total += 1
            stripped = line.strip()
            if stripped.startswith(".word"):
                words += 1
            elif INSTRUCTION_RE.match(line):
                instructions += 1
            if LABEL_RE.match(line):
                labels += 1
    return {
        "lines": total,
        "words": words,
        "instructions": instructions,
        "labels": labels,
    }


def collect():
    rom = ROM.read_bytes() if ROM.exists() else b""
    prologues = {}
    if rom:
        for start, end, kind, name in load_segments():
            if kind in ("asm", "hasm", "c"):
                prologues[name] = count_prologues(rom, start, end)

    files = {}
    for path in sorted(ASM_DIR.rglob("*.s")):
        stats = scan_asm_file(path)
        # splat suffixes data subsegment files (`foo.rodata.s`). Those carry
        # data labels, which must not be counted as delimited functions.
        stats["is_code"] = not path.stem.endswith((".data", ".rodata", ".bss"))
        stats["prologues"] = prologues.get(path.stem, 0)
        files[path.stem] = stats

    symbols = 0
    if SYMBOL_ADDRS.exists():
        symbols = sum(
            1 for line in SYMBOL_ADDRS.read_text(errors="replace").splitlines()
            if "=" in line and not line.strip().startswith("//")
        )

    code = [f for f in files.values() if f["is_code"]]
    totals = {
        key: sum(f[key] for f in code)
        for key in ("lines", "words", "instructions", "labels", "prologues")
    }
    totals["symbols"] = symbols
    totals["data_lines"] = sum(
        f["lines"] for f in files.values() if not f["is_code"]
    )
    return {"files": files, "totals": totals}


def pct(num, den):
    return (100.0 * num / den) if den else 0.0


def render(data, baseline=None):
    files, totals = data["files"], data["totals"]

    print("Segmentos de codigo (los de datos se excluyen del total):")
    print()
    print(f"{'archivo':<26}{'lineas':>9}{'.word':>9}{'instr':>9}"
          f"{'etiq':>7}{'prologos':>10}{'decod':>8}")
    print("-" * 78)
    for name in sorted(files, key=lambda n: -files[n]["words"]):
        f = files[name]
        if f["lines"] < 20 or not f["is_code"]:
            continue
        decoded = pct(f["instructions"], f["instructions"] + f["words"])
        print(f"{name:<26}{f['lines']:>9,}{f['words']:>9,}{f['instructions']:>9,}"
              f"{f['labels']:>7}{f['prologues']:>10}{decoded:>7.1f}%")

    print("-" * 78)
    decoded = pct(totals["instructions"], totals["instructions"] + totals["words"])
    print(f"{'TOTAL':<26}{totals['lines']:>9,}{totals['words']:>9,}"
          f"{totals['instructions']:>9,}{totals['labels']:>7}"
          f"{totals['prologues']:>10}{decoded:>7.1f}%")
    print()
    print(f"  decodificado      : {decoded:.1f}%  "
          f"({totals['words']:,} lineas .word de {totals['lines']:,})")
    print(f"  funciones etiquet.: {totals['labels']:,} "
          f"de {totals['prologues']:,} prologos detectables "
          f"({pct(totals['labels'], totals['prologues']):.1f}%)")
    print(f"  simbolos          : {totals['symbols']:,}")
    print(f"  (datos, aparte)   : {totals['data_lines']:,} lineas en segmentos data/rodata")

    if baseline:
        b = baseline["totals"]
        print()
        print("  delta vs baseline:")
        for key, label in (("words", "lineas .word"),
                           ("instructions", "instrucciones"),
                           ("labels", "funciones etiquetadas"),
                           ("symbols", "simbolos")):
            delta = totals[key] - b.get(key, 0)
            sign = "+" if delta >= 0 else ""
            print(f"    {label:<24}{sign}{delta:,}")


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--json", action="store_true", help="emitir JSON")
    ap.add_argument("--baseline", type=Path, help="JSON de una corrida previa")
    args = ap.parse_args()

    if not ASM_DIR.exists():
        sys.exit(f"no existe {ASM_DIR} — corre `make setup` primero")

    data = collect()
    if args.json:
        json.dump(data, sys.stdout, indent=2)
        print()
        return

    baseline = json.loads(args.baseline.read_text()) if args.baseline else None
    render(data, baseline)


if __name__ == "__main__":
    main()
