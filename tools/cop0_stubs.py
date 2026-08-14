#!/usr/bin/env python3
"""List the functions that touch COP0, which is what `stubs` is actually for.

N64Recomp cannot translate COP0 instructions, and #32 showed that stubbing
whatever it happens to complain about is wrong — a third of those were branch
labels our own segmentation invented. This derives the list from the ROM
instead: a function belongs in `stubs` if and only if it contains a COP0
instruction.

Most of them turn out to be libultra OS primitives — Status-register interrupt
enable/disable, EntryHi TLB writes, Cause/EPC exception handling — which
N64ModernRuntime implements natively anyway. Stubbing those loses nothing; the
runtime supplies the behaviour. GoldenEye's port does the same for its TLB
functions.

Usage:
    python3 tools/cop0_stubs.py            # report
    python3 tools/cop0_stubs.py --toml     # emit a stubs = [...] block
"""

import argparse
import glob
import re
import struct
import sys
from collections import Counter
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
ROM = ROOT / "baserom.us.z64"
ASM = ROOT / "us" / "asm"

COP0_NAMES = {
    0: "Index", 2: "EntryLo0", 3: "EntryLo1", 4: "Context", 5: "PageMask",
    6: "Wired", 8: "BadVAddr", 9: "Count", 10: "EntryHi", 11: "Compare",
    12: "Status", 13: "Cause", 14: "EPC", 30: "ErrorEPC",
}
TLB_OPS = {0x42000001: "tlbr", 0x42000002: "tlbwi", 0x42000006: "tlbwr",
           0x42000008: "tlbp", 0x42000018: "eret"}


# The cache-control instruction. N64Recomp reports it as "Unhandled
# instruction: cache" and stops, for the same reason it cannot do COP0: there
# is no C for it. It belongs in `stubs` on the same grounds -- N64ModernRuntime
# keeps its own memory coherent, so a translated cache op would have nothing to
# operate on.
CACHE_OPCODE = 0x2F


def classify(word):
    if (word >> 26) == CACHE_OPCODE:
        return "cache"
    if word in TLB_OPS:
        return TLB_OPS[word]
    if (word >> 26) != 0x10:
        return None
    rs, rd = (word >> 21) & 0x1F, (word >> 11) & 0x1F
    name = COP0_NAMES.get(rd, f"c0r{rd}")
    if rs == 0:
        return f"mfc0 {name}"
    if rs == 4:
        return f"mtc0 {name}"
    return None


def load_segments():
    """(vram_lo, vram_hi, delta) for the code-bearing segments."""
    import yaml
    with (ROOT / "versions" / "turok2.us.yaml").open() as fh:
        doc = yaml.safe_load(fh)
    out = []
    tops = [s for s in doc["segments"] if isinstance(s, dict)]
    tops.sort(key=lambda s: s["start"])
    for i, seg in enumerate(tops):
        if "vram" not in seg or seg.get("type") != "code":
            continue
        end = tops[i + 1]["start"] if i + 1 < len(tops) else seg["start"]
        out.append((seg["vram"], seg["vram"] + (end - seg["start"]),
                    seg["start"] - seg["vram"]))
    return out


def to_rom(segments, addr):
    for lo, hi, delta in segments:
        if lo <= addr < hi:
            return addr + delta
    return None


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--toml", action="store_true", help="emit a stubs block")
    args = ap.parse_args()

    rom = ROM.read_bytes()
    segments = load_segments()

    # Only real functions. `stubs` is a list of things N64Recomp will replace,
    # and it only ever walks STT_FUNC symbols — naming anything else there is
    # rejected outright ("Function .L0029C170 is stubbed out in the config file
    # but does not exist!"), which aborts the whole run before a single
    # function is emitted.
    #
    # Dropping them is not just about satisfying the parser. A `.L` is a branch
    # target and a `D_` a data label; both sit *inside* a function, so a COP0
    # instruction found under one belongs to whichever function encloses it.
    # Filtering them out before the ranges are computed is what makes that
    # attribution happen — each range now runs from one function to the next
    # instead of being cut short at every interior label. #32 is the same
    # mistake in its earlier form: a third of the names stubbed back then were
    # branch labels our own segmentation had invented.
    labels = []
    for path in sorted(glob.glob(str(ASM / "*.s"))):
        for line in open(path, errors="replace"):
            m = re.match(r"^\s*\.globl (\S+)", line)
            if m:
                name = m.group(1)
                if name.startswith(".L") or name.startswith("D_"):
                    continue
                a = re.search(r"([0-9A-Fa-f]{8})$", name)
                if a:
                    labels.append((int(a.group(1), 16), name))
    labels.sort()

    found = {}
    for i, (addr, name) in enumerate(labels):
        end = labels[i + 1][0] if i + 1 < len(labels) else addr + 0x100
        off = to_rom(segments, addr)
        if off is None:
            continue
        for j in range(off, min(off + (end - addr), len(rom) - 3), 4):
            what = classify(struct.unpack_from(">I", rom, j)[0])
            if what:
                found.setdefault(name, set()).add(what)

    # Functions that cannot be translated for a reason other than COP0, listed
    # by hand with the reason. The only class so far is a routine duplicated
    # between two VM modules: both copies read the same jump table, whose
    # entries name the first copy's case bodies, so for the second copy every
    # entry falls outside its own body and N64Recomp cannot size the table.
    # Its twin is recompiled normally, so nothing is lost by stubbing it.
    manual = ROOT / "versions" / "manual_stubs.us.txt"
    if manual.exists():
        for line in manual.read_text(errors="replace").splitlines():
            line = line.split("#")[0].strip()
            if line:
                found.setdefault(line, set()).add("manual")

    if args.toml:
        print("stubs = [")
        for name in sorted(found):
            print(f'    "{name}",  # {", ".join(sorted(found[name]))}')
        print("]")
        return

    print(f"functions touching COP0: {len(found)}")
    ops = Counter(o for v in found.values() for o in v)
    print("\ninstructions involved:")
    for op, n in ops.most_common(12):
        print(f"  {op:<18} in {n} functions")
    kinds = Counter(n.split("_")[0] if "_" in n else n[:2] for n in found)
    print("\nby symbol kind:")
    for k, n in kinds.most_common():
        print(f"  {k:<8} {n}")


if __name__ == "__main__":
    main()
