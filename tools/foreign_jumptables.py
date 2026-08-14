#!/usr/bin/env python3
"""Find functions whose jump table belongs to a different copy of them.

The two VM modules share their `switch` tables. A table holds absolute code
addresses, so it can only name one module's case bodies -- the copy in
`virtual` -- and the copy in `virtual_1` reads the same table and finds every
entry outside its own body. N64Recomp cannot size a table like that and stops:

    Failed to determine size of jump table at 0x800A6020 for instruction
    at 0x00465B88

See #40 for why that is coherent rather than a mapping error: the VM loader
relocates these tables when it pages a module in, which is exactly what a
static recompiler cannot follow.

Detection reads the ELF disassembly and tracks what each register holds, since
the table base reaches the `jr` in more than one shape:

    lui at, 0x800c            lui  s3, 0x800a
    addu at, at, v0           addiu s3, s3, 24608
    lw   v0, 0x1148(at)       ...
    jr   v0                   addu v0, v0, s3
                              lw   v0, 0(v0)
                              jr   v0

An earlier pass matched only the first and missed func_00465B04, which is the
second. Tracking `lui`/`addiu` pairs covers both without caring about the shape.

A table is foreign when its entries fall outside the function that reads it.
That is the property N64Recomp actually objects to, and it is what makes this a
test rather than a guess about which module a function belongs to.

Usage:
    python3 tools/foreign_jumptables.py           # report
    python3 tools/foreign_jumptables.py --write   # append to manual_stubs
"""

import argparse
import re
import struct
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
ELF = ROOT / "build" / "turok2.us.elf"
DIS = ROOT / "build" / "dis.txt"
STUBS = ROOT / "versions" / "manual_stubs.us.txt"
IMAGE = "turok2-build"

INSN = re.compile(r"^\s*([0-9a-f]+):\s+([0-9a-f]{8})\s+(\S+)\s*(.*)$")
FUNC = re.compile(r"^([0-9a-f]+) <(\S+)>:")


def disassembly():
    if not DIS.exists():
        subprocess.run(
            ["docker", "run", "--platform=linux/amd64", "--rm", "-v", f"{ROOT}:/work",
             IMAGE, "bash", "-c",
             f"mips-linux-gnu-objdump -d {ELF.relative_to(ROOT)} > /work/build/dis.txt"],
            check=True)
    return DIS.read_text(errors="replace").splitlines()


def rodata_reader():
    """A function mapping a rodata vram to its ROM bytes, via the yaml."""
    import yaml
    rom = (ROOT / "baserom.us.z64").read_bytes()
    with (ROOT / "versions" / "turok2.us.yaml").open() as fh:
        doc = yaml.safe_load(fh)
    tops = sorted((s for s in doc["segments"] if isinstance(s, dict)),
                  key=lambda s: s["start"])
    spans = []
    for i, seg in enumerate(tops):
        if "vram" not in seg:
            continue
        end = tops[i + 1]["start"] if i + 1 < len(tops) else seg["start"]
        spans.append((seg["vram"], seg["vram"] + (end - seg["start"]), seg["start"]))

    def read(vram):
        for lo, hi, rom_start in spans:
            if lo <= vram < hi:
                off = rom_start + (vram - lo)
                if off + 4 <= len(rom):
                    return struct.unpack_from(">I", rom, off)[0]
        return None
    return read


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--write", action="store_true", help="append to manual_stubs")
    opts = ap.parse_args()

    read_word = rodata_reader()
    lines = disassembly()

    current = None
    func_start = 0
    regs = {}
    prev = None
    foreign = {}

    for line in lines:
        m = FUNC.match(line)
        if m:
            func_start, current = int(m.group(1), 16), m.group(2)
            regs = {}
            prev = None
            continue

        m = INSN.match(line)
        if not m:
            continue
        addr, op, operands = int(m.group(1), 16), m.group(3), m.group(4)
        operands = operands.split("<")[0].strip()

        # Track the two halves of an address literal.
        lui = re.match(r"^(\S+),(0x[0-9a-f]+)$", operands.replace(" ", ""))
        if op == "lui" and lui:
            regs[lui.group(1)] = int(lui.group(2), 16) << 16
        elif op in ("addiu", "ori"):
            p = re.match(r"^(\S+),(\S+),(-?\S+)$", operands.replace(" ", ""))
            if p and p.group(2) in regs:
                regs[p.group(1)] = regs[p.group(2)] + int(p.group(3), 0)
        elif op in ("addu", "add"):
            # The index is added to the base at runtime, so the sum still
            # carries the base. This is where the base moves into another
            # register, and not following it is what hid func_00465B04:
            # `addu v0, v0, s3` put 0x800A6020 in v0 and the scan lost it.
            p = re.match(r"^(\S+),(\S+),(\S+)$", operands.replace(" ", ""))
            if p:
                known = [r for r in (p.group(2), p.group(3)) if r in regs]
                if len(known) == 1:
                    regs[p.group(1)] = regs[known[0]]
        elif op == "lw":
            prev = (addr, operands)
            continue
        elif op == "jr" and prev is not None:
            dest = operands.strip()
            p = re.match(r"^(\S+),(-?\d+)\((\S+)\)$", prev[1].replace(" ", ""))
            if p and p.group(1) == dest:
                base = regs.get(p.group(3))
                if base is not None:
                    table = base + int(p.group(2))
                    first = read_word(table)
                    # Foreign when the entries name code outside this function.
                    # Only trust it when the table really holds code, or a
                    # stray `lui` picked up as a base invents tables full of
                    # things like 0x20004000 and 0xFFFFFFFF.
                    plausible = first is not None and (
                        0x00200500 <= first < 0x002A5200
                        or 0x00400000 <= first < 0x004C4340)
                    if plausible and not (func_start <= first < addr + 0x2000):
                        foreign.setdefault(current, (table, first, prev[0]))
        prev = None

    already = {l.strip() for l in STUBS.read_text().splitlines()
               if l.strip().startswith("func_")}
    new = sorted(f for f in foreign if f not in already)

    print(f"jump tables whose entries lie outside the reader: {len(foreign):,}")
    print(f"  not stubbed yet                              : {len(new):,}")
    for name in new[:8]:
        table, first, at = foreign[name]
        print(f"  {name:<20} table 0x{table:08X} -> 0x{first:08X}  (jr at 0x{at:08X})")

    if opts.write and new:
        with STUBS.open("a") as fh:
            fh.write("\n# Same class, found by tracking the table base through a register\n"
                     "# rather than only the `at` form. Their tables' entries name the\n"
                     "# `virtual` copy's case bodies, so nothing in them falls inside these.\n")
            fh.write("\n".join(new) + "\n")
        print(f"\nappended: {len(new):,}")


if __name__ == "__main__":
    main()
