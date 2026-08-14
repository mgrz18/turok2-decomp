#!/usr/bin/env python3
"""Declare the addresses that are a function's *second* entry point.

Some calls land inside a function rather than at its start. `0x00426898` is
called from five separate sites in `virtual_1` and is also the delay slot of
the `jal` at `0x00426894`, inside `func_00426814`, which reaches past it to
`0x004268BC`. That is legal MIPS and ordinary in hand-written or heavily
optimised code: one body, two ways in.

A symbol table has one name per address, so seeding these splits the function
that contains them and the pieces then branch out of themselves. Measured, that
took the emitted C from 2.0 MB to 228 KB.

N64Recomp solves this itself when caller and callee share a section --
`resolve_jal` returns CreateStatic and emits a static function at the address.
Across sections it declines, because with overlays one vram can belong to
several sections, and the static would also be registered under the caller's
section index. That matters more than it sounds: `main.cpp` computes a static's
ROM address as `addr - section.ram_addr + section.rom_addr` from whichever
section it was filed under, so filing it under the wrong one produces a
function built from the wrong bytes. It would compile and misbehave.

`manual_funcs` avoids all of it. Each entry names its own section, so the ROM
address is computed from the right one, and the function is registered in
`functions_by_vram`, which makes the call resolve as an ordinary match. The
enclosing function is left alone -- the new one simply overlaps it, which the
recompiler is fine with, and its jump tables still size against its unchanged
extent.

Sizes come from the ELF symbol table: a second entry runs to the end of the
function it sits in.

Usage:
    python3 tools/second_entry_points.py           # report
    python3 tools/second_entry_points.py --toml    # emit a manual_funcs block
"""

import argparse
import bisect
import re
import struct
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
ELF = ROOT / "build" / "turok2.us.elf"
IMAGE = "turok2-build"


def elf_functions():
    """[(vram, size, name)] for every sized function symbol, sorted."""
    out = subprocess.run(
        ["docker", "run", "--platform=linux/amd64", "--rm", "-v", f"{ROOT}:/work",
         IMAGE, "mips-linux-gnu-nm", "-S", "--defined-only", str(ELF.relative_to(ROOT))],
        capture_output=True, text=True).stdout
    funcs = []
    for line in out.splitlines():
        parts = line.split()
        if len(parts) == 4 and parts[2] in ("t", "T"):
            funcs.append((int(parts[0], 16), int(parts[1], 16), parts[3]))
    funcs.sort()
    return funcs


def elf_sizeless():
    """Function symbols the ELF carries with no size, as a set of addresses.

    `nm -S` prints four fields for a sized symbol and three for an unsized one,
    so the field count is the test.
    """
    out = subprocess.run(
        ["docker", "run", "--platform=linux/amd64", "--rm", "-v", f"{ROOT}:/work",
         IMAGE, "mips-linux-gnu-nm", "-S", "--defined-only", str(ELF.relative_to(ROOT))],
        capture_output=True, text=True).stdout
    addrs = set()
    for line in out.splitlines():
        parts = line.split()
        if len(parts) == 3 and parts[1] in ("t", "T"):
            addrs.add(int(parts[0], 16))
    return addrs


def elf_sections():
    """[(name, vram, size)] for the loadable code sections."""
    out = subprocess.run(
        ["docker", "run", "--platform=linux/amd64", "--rm", "-v", f"{ROOT}:/work",
         IMAGE, "mips-linux-gnu-readelf", "-S", str(ELF.relative_to(ROOT))],
        capture_output=True, text=True).stdout
    secs = []
    for line in out.splitlines():
        m = re.match(r"\s*\[\s*\d+\]\s+(\S+)\s+PROGBITS\s+([0-9a-f]+)\s+[0-9a-f]+\s+([0-9a-f]+)", line)
        if m:
            secs.append((m.group(1), int(m.group(2), 16), int(m.group(3), 16)))
    return secs


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--toml", action="store_true", help="emit a manual_funcs block")
    args = ap.parse_args()

    sys.path.insert(0, str(ROOT / "tools"))
    from call_target_seeds import branch_target, load_addresses
    from function_seed import load_code_segments, vram_to_segment, vram_to_rom, ROM

    rom = ROM.read_bytes()
    segments = load_code_segments()
    seeded = load_addresses(ROOT / "versions" / "symbols_from_scan.us.txt")
    ordered = sorted(seeded)

    # Every call target, by `jal` or by a `j` that leaves its own function.
    targets = set()
    for start, end, vram, _name, _owner in segments:
        end = min(end, len(rom) - 3)
        for off in range(start, end, 4):
            word = struct.unpack_from(">I", rom, off)[0]
            op = word >> 26
            if op not in (0x02, 0x03):
                continue
            pc = vram + (off - start)
            target = (pc & 0xF0000000) | ((word & 0x03FFFFFF) << 2)
            if not vram_to_segment(segments, target):
                continue
            if op == 0x02:
                i = bisect.bisect_right(ordered, pc) - 1
                if i < 0:
                    continue
                home = ordered[i]
                j = bisect.bisect_right(ordered, home)
                nxt = ordered[j] if j < len(ordered) else 1 << 32
                if home <= target < nxt:
                    continue          # a jump inside its own function
            targets.add(target)

    funcs = elf_functions()
    starts = [f[0] for f in funcs]
    # Sizeless function symbols are invisible to `nm -S`'s size column but they
    # still occupy space, so they bound the ones around them. Both sets are
    # needed: `funcs` to know what is already usable, `all_starts` to know
    # where anything at all begins.
    sizeless = elf_sizeless()
    all_starts = sorted(starts + sorted(sizeless))
    sections = elf_sections()
    # Addresses objdump rendered as `.word` rather than an instruction.
    data_words = []
    dis = ROOT / "build" / "dis.txt"
    if dis.exists():
        for line in dis.read_text(errors="replace").splitlines():
            m = re.match(r"^\s*([0-9a-f]+):\s+[0-9a-f]{8}\s+\.word\b", line)
            if m:
                data_words.append(int(m.group(1), 16))
    data_words.sort()
    by_name = {name for _v, _s, name in funcs}

    # Addresses already stubbed. Declaring one again puts a second, unstubbed
    # function at the same vram, and the recompiler then tries to translate the
    # very thing the stub exists to avoid: entry_002A31FC failed on
    # `mfc0 $10` while func_002A31FC, the same address, was stubbed for exactly
    # that COP0 access.
    stubbed = set()
    toml = ROOT / "turok2.us.toml"
    if toml.exists():
        block = re.search(r"stubs = \[(.*?)\n\]", toml.read_text(), re.S)
        if block:
            for m in re.finditer(r'"(\S*?([0-9A-Fa-f]{8}))"', block.group(1)):
                stubbed.add(int(m.group(2), 16))

    entries = []
    unplaced = 0
    # Not `targets - seeded`. Being seeded is not the same as being usable: a
    # seed at a second entry point comes out of gas with no size, because
    # fix_asm.py rightly declines to promote a label whose preceding
    # instructions are not a return, and the symbol stays NOTYPE with no
    # `.end`. N64Recomp skips a zero-sized symbol when resolving a call, so
    # `func_00241860` was seeded, emitted, and still reported as "No function
    # found for jal target". The ELF is what decides -- a target only needs no
    # declaring if a *sized* function starts exactly there.
    for target in sorted(targets):
        # Two different things end up needing a declaration, and they want
        # different sizes.
        i = bisect.bisect_right(starts, target) - 1
        if i < 0:
            continue
        vram, size, _name = funcs[i]

        if vram == target:
            continue                      # a sized function starts here already

        if target in stubbed:
            continue                      # the stub already covers this address

        if target < vram + size:
            # A second entry into a sized function. It runs to that function's
            # end, since that is where the shared body stops.
            length = vram + size - target
        elif target in sizeless:
            # A genuine function start that lost its size. splat emitted it as
            # an indented `.globl` with no `.ent`, fix_asm declined to promote
            # it because the instructions before it are not a return, and gas
            # recorded nothing. N64Recomp then skips it when resolving a call.
            # func_00214F68 is one: the sized function before it ends at
            # exactly 0x00214F68, so it begins where that one stops.
            #
            # Its extent is whatever comes next, sized or not.
            j = bisect.bisect_right(all_starts, target)
            if j >= len(all_starts):
                unplaced += 1
                continue
            length = all_starts[j] - target

        else:
            unplaced += 1                 # inside no function at all
            continue

        # Truncate where objdump stops seeing instructions. "Runs to whatever
        # begins next" overshoots when what sits between is data rather than
        # another symbol: entry_002010C0 came out 1812 bytes and swept up
        # `.word 0x794c6179` at 0x002017AC -- the tail of the string
        # "meSkyLay" -- which N64Recomp reports as "Unhandled instruction:
        # INVALID".
        #
        # The last `jr ra` in the span is not the anchor. There is one at
        # 0x002017CC, past the data, because a random word in a string pool can
        # equal 0x03E00008. objdump has already made the judgement and written
        # `.word`, so that is what to trust -- the same signal
        # word_bodied_funcs.py uses.
        first_data = None
        for bad in data_words:
            if target < bad < target + length:
                first_data = bad
                break

        # objdump's `.word` is not the only tell. A string can decode as a
        # perfectly ordinary-looking instruction: 0x74656D44 is "temD" and
        # renders as `jalx`, which N64Recomp still rejects as INVALID. Text is
        # the more reliable signal -- four printable bytes in a row do not
        # happen by accident in compiled MIPS.
        base = vram_to_rom(segments, target)
        if base is not None:
            for k in range(0, length, 4):
                if base + k + 4 > len(rom):
                    break
                if target + k >= (first_data or 1 << 32):
                    break
                chunk = rom[base + k:base + k + 4]
                if all(0x20 <= b < 0x7F for b in chunk):
                    first_data = target + k
                    break

        # Back up to the last return before the data, so the function ends at
        # its own epilogue rather than in the middle of one.
        if first_data is not None and base is not None:
            end = first_data - target
            last = None
            for k in range(0, end, 4):
                if struct.unpack_from(">I", rom, base + k)[0] == 0x03E00008:
                    last = k
            length = (last + 8) if last is not None else end
        # Stretch to cover the function's own branches. A branch cannot leave
        # its function, so if one inside the declared range lands past the end,
        # the range is short -- which the recompiler reports as "Unhandled
        # branch in entry_00418CA4". Bounded by whatever data was found above,
        # so growing it can never swallow the string pool the trim just avoided.
        if base is not None:
            limit = (first_data - target) if first_data is not None else length * 4
            grew = True
            while grew and length < limit:
                grew = False
                for k in range(0, length, 4):
                    if base + k + 4 > len(rom):
                        break
                    dest = branch_target(
                        struct.unpack_from(">I", rom, base + k)[0], target + k)
                    op = struct.unpack_from(">I", rom, base + k)[0] >> 26
                    if dest is None or op == 3:
                        continue          # a `jal` leaves and comes back
                    if op == 2 and dest in targets:
                        continue          # a `j` to an entry is a tail call
                    # Everything else stays inside: a conditional branch cannot
                    # leave its function, and a `j` to an address that is nobody's
                    # entry is the jump to a shared epilogue. entry_00418CA4 has
                    # one at 0x00418CB0 reaching 0x00418D44, 0xA0 past a body
                    # declared 104 bytes long.
                    if target < dest and dest - target >= length:
                        length = min(dest - target + 4, limit)
                        grew = True

        section = next((n for n, sv, ss in sections if sv <= target < sv + ss), None)
        if section is None or length <= 0:
            unplaced += 1
            continue

        name = f"entry_{target:08X}"
        if name in by_name:
            continue
        entries.append((name, section, target, length))

    if args.toml:
        print("manual_funcs = [")
        for name, section, vram, size in entries:
            print(f'    {{ name = "{name}", section = "{section}", '
                  f'vram = 0x{vram:08X}, size = {size} }},')
        print("]")
        return

    print(f"call targets                       : {len(targets):,}")
    print(f"  already a function start         : {len(targets & seeded):,}")
    print(f"  second entry into a function     : {len(entries):,}")
    print(f"  in no function or section        : {unplaced:,}")
    if entries:
        print("\nfirst few:")
        for name, section, vram, size in entries[:6]:
            print(f"  {name}  {section:<18} 0x{vram:08X}  {size} bytes")


if __name__ == "__main__":
    main()
