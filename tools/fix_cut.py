#!/usr/bin/env python3
"""Put function boundaries splat got wrong where they really are.

join_cut.py handles one kind of wrong cut, a function split at an early
`jr ra`. This handles the rest, from a tracked list of fixes,
versions/cuts.us.txt, one per line:

    stray <from> <to>    // why
        The words [from, to) are never executed: no call, jump, branch or
        table reaches them, and the function that splat started at <from>
        really starts at <to>. They stay in the ROM as a hand-written stub,
        src/us/asm/stray_<from>.s, `.word` by `.word` from the ROM, wired
        into the yaml as a `hasm` subsegment. A stub is not a function: its
        symbol (func_<from>, kept because code may still name it, as
        func_00264C94 stores func_00264994 + 0xC) is an object, so no tool
        counts it as code and N64Recomp does not try to translate it.
        <to> gets a function symbol of its own.

    func <start> <size>  // why
        The function at <start> is <size> bytes: an end splat put too early
        or too late, a start splat missed (a cut holding several functions),
        or several of splat's functions that are one. Every function symbol
        strictly inside the range is dropped (as join_cut.py drops a tail),
        and C already wired for one of them goes back to asm with it.

Both are kept in versions/function_sizes.us.txt, in a block this tool owns,
so splat reads them; symbols_from_scan.us.txt loses the entries they
override. Applying is idempotent: run it after editing the list, then
`make setup` and `make verify`.

Usage (on the build box):
    tools/fix_cut.py            # report what would change
    tools/fix_cut.py --write
"""

import argparse
import re
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
CUTS = ROOT / "versions" / "cuts.us.txt"
SIZES = ROOT / "versions" / "function_sizes.us.txt"
SCAN = ROOT / "versions" / "symbols_from_scan.us.txt"
NAMES = ROOT / "versions" / "names.us.txt"
YAML = ROOT / "versions" / "turok2.us.yaml"
ROM = ROOT / "baserom.us.z64"
STUBS = ROOT / "src" / "us" / "asm"
SRC = ROOT / "src" / "us"
BEGIN = "// --- true extents from versions/cuts.us.txt (tools/fix_cut.py) ---"
END = "// --- end cuts ---"
ENTRY = re.compile(r"^(\s+)- \[(0x[0-9A-Fa-f]+),\s*([.\w]+)(?:,\s*([^\]\s]+))?\s*\](.*)$")


def read_cuts():
    out = []
    for n, line in enumerate(CUTS.read_text().splitlines(), 1):
        body, _, why = line.partition("//")
        f = body.split()
        if not f:
            continue
        if f[0] not in ("stray", "func") or len(f) != 3:
            sys.exit(f"{CUTS.name}:{n}: expected `stray <from> <to>` or `func <start> <size>`")
        out.append((f[0], int(f[1], 16), int(f[2], 16), why.strip()))
    return out


def code_segments(lines):
    """[(first line, end line, rom start, vram)] for the yaml's code segments."""
    heads = [i for i, l in enumerate(lines) if l.startswith("  - name: ")]
    out = []
    for k, i in enumerate(heads):
        stop = heads[k + 1] if k + 1 < len(heads) else len(lines)
        start = vram = None
        for l in lines[i:stop]:
            m = re.match(r"^\s+(start|vram):\s*(0x[0-9A-Fa-f]+)", l)
            if m and m.group(1) == "start" and start is None:
                start = int(m.group(2), 16)
            elif m and m.group(1) == "vram" and vram is None:
                vram = int(m.group(2), 16)
        if start is not None and vram is not None:
            out.append((i, stop, start, vram))
    return out


def subsegments(lines, seg):
    i, stop, _, _ = seg
    return [(j, ENTRY.match(lines[j])) for j in range(i, stop) if ENTRY.match(lines[j])]


def locate(lines, vram):
    """(segment, rom offset) of a code address: the segment whose subsegments
    cover it. Segments are ordered by ROM, and each one's vram window runs to
    the next segment's start."""
    segs = code_segments(lines)
    allstarts = sorted(s[2] for s in segs) + [1 << 32]
    for seg in segs:
        _, _, start, base = seg
        end = min(a for a in allstarts if a > start)
        rom = start + vram - base
        if base <= vram and start <= rom < end and subsegments(lines, seg):
            return seg, rom
    sys.exit(f"no code segment holds 0x{vram:08X}")


def names_by_addr():
    out = {}
    for n, a in re.findall(r"^(\w+)\s*=\s*0x([0-9A-Fa-f]+);", NAMES.read_text(), re.M):
        out[int(a, 16)] = n
    return out


def fname(addr, named):
    return named.get(addr, f"func_{addr:08X}")


def stub_text(a, b, rom_off, rom, why):
    words = struct.unpack(f">{(b - a) // 4}I", rom[rom_off:rom_off + b - a])
    name = f"func_{a:08X}"
    return (f"/* 0x{a:08X}-0x{b:08X}: {len(words)} words splat took for the start of a\n"
            f" * function. Never executed: {why or 'see versions/cuts.us.txt'}.\n"
            f" * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */\n\n"
            f".set noreorder\n.section .text, \"ax\"\n\n"
            f".globl {name}\n.type {name}, @object\n{name}:\n"
            + "".join(f"    .word 0x{w:08X}\n" for w in words)
            + f".size {name}, . - {name}\n")


def find_strays():
    """Candidates for `stray` lines, from the linked ELF and the asm.

    The stray words are the padding before an object that starts 16-aligned
    (SN's linker aligns each object's .text to 16 and fills the gap with
    whatever its buffer held, often a copy of code from elsewhere), so a
    candidate is a function at A, not 16-aligned, whose next 16-aligned
    address B is inside it, where
      - nothing reaches A or any word before B (jal, j, branch, a .word in
        the data), and
      - B is reached (jal, a pointer in the data) or opens with a prologue
        (addiu $sp, $sp, -N).
    Each one still wants a look: print it with the disassembly.
    """
    import bisect
    import subprocess
    from elftools.elf.elffile import ELFFile
    elf_path = ROOT / "build" / "turok2.us.elf"
    funcs = []
    with elf_path.open("rb") as fh:
        for s in ELFFile(fh).get_section_by_name(".symtab").iter_symbols():
            if s["st_info"]["type"] == "STT_FUNC" and s["st_size"]:
                funcs.append((s["st_value"], s["st_size"], s.name))
    funcs.sort()
    reached, called = set(), set()
    lo, hi = funcs[0][0], funcs[-1][0] + funcs[-1][1]
    dis = subprocess.run(["mips-linux-gnu-objdump", "-d", "--no-show-raw-insn", "-M", "reg-names=numeric",
                          str(elf_path)], capture_output=True, text=True).stdout
    words = {}
    for line in dis.splitlines():
        m = re.match(r"^\s*([0-9a-f]+):\s+(\S+)\s*(.*)$", line)
        if not m:
            continue
        pc, op, ops = int(m.group(1), 16), m.group(2), m.group(3)
        words[pc] = (op, ops)
        if op == "jal" or op == "j" or op.startswith("b"):
            t = re.match(r"\s*([0-9a-f]+)\b", ops.split(",")[-1])
            if t:
                (called if op == "jal" else reached).add(int(t.group(1), 16))
    for p in (ROOT / "us" / "asm" / "data").glob("*.s"):
        for v in re.findall(r"\.word\s+(?:0x|func_|D_|\.L)([0-9A-Fa-f]{8})\b", p.read_text()):
            called.add(int(v, 16))
    # Code that names the function (a pointer, %hi/%lo) other than its own file.
    asm = [p for p in (ROOT / "us" / "asm").glob("*.s")] + list((ROOT / "src").rglob("*.c"))
    texts = {p: re.sub(r"^\.globl.*\n|^\.ent.*\n|^\.end.*\n|^\w+:\n", "", p.read_text(), flags=re.M) for p in asm}
    for a, size, name in funcs:
        if a % 16 == 0:
            continue
        b = (a + 15) & ~15
        if b > a + size or b - a > 12:
            continue
        if b == a + size and not any(f[0] == b for f in funcs):
            continue
        if any(x in reached or x in called for x in range(a, b, 4)):
            continue
        op = words.get(b, ("", ""))
        prologue = op[0] == "addiu" and op[1].startswith("$29,$29,-")
        if not (b in called or prologue):
            continue
        why = "reached" if b in called else "a prologue"
        named = [p.name for p in asm if re.search(rf"\b{name}\b", texts[p])]
        print(f"stray 0x{a:08X} 0x{b:08X} // {name}: {(b - a) // 4} words; 0x{b:08X} is {why}"
              + (f"; {name} named in {', '.join(named[:3])}" if named else ""))


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--write", action="store_true")
    ap.add_argument("--find", action="store_true", help="list candidate stray lines (needs a build)")
    args = ap.parse_args()
    if args.find:
        return find_strays()
    cuts = read_cuts()
    named = names_by_addr()
    rom = ROM.read_bytes()
    lines = YAML.read_text().splitlines()
    sizes = {}                 # name -> (addr, size or None), the block's entries
    drop = set()               # function names that stop being functions
    ranges = []                # (start, end) every fix owns
    stubs = {}
    for kind, a, b, why in cuts:
        if kind == "func":
            sizes[fname(a, named)] = (a, b)
            ranges.append((a, a + b))
            continue
        if b <= a or (b - a) % 4:
            sys.exit(f"stray 0x{a:08X} 0x{b:08X}: not a run of words")
        ranges.append((a, b))
        seg, ra = locate(lines, a)
        _, rb = locate(lines, b)
        stubs[a] = (b, stub_text(a, b, ra, rom, why))
        drop.add(f"func_{a:08X}")
        if a in named:
            sys.exit(f"stray 0x{a:08X} has a real name ({named[a]}): move it to the real start first")
        # The real start needs a symbol; a `func` line may give its size.
        if fname(b, named) not in sizes:
            sizes[fname(b, named)] = (b, None)
        # yaml: [ra, hasm, stray] and [rb, asm] in the asm subsegment holding them.
        ents = subsegments(lines, seg)
        indent = ents[0][1].group(1)
        path = f"../src/us/asm/stray_{a:08X}"
        if any(m.group(4) == path for _, m in ents):
            continue
        starts = {int(m.group(2), 16): (j, m) for j, m in ents}
        host = max(s for s in starts if s <= ra)
        hj, hm = starts[host]
        if hm.group(3) != "asm":
            sys.exit(f"stray 0x{a:08X}: it is in a `{hm.group(3)}` subsegment at ROM 0x{host:X}, not asm")
        nxt = min((s for s in starts if s > ra), default=None)
        if nxt is not None and nxt < rb:
            sys.exit(f"stray 0x{a:08X}: crosses the subsegment at ROM 0x{nxt:X}")
        new = []
        if host == ra:
            lines[hj] = f"{indent}- [0x{ra:X}, hasm, {path}]"
        else:
            new.append(f"{indent}- [0x{ra:X}, hasm, {path}]")
        if nxt != rb:
            new.append(f"{indent}- [0x{rb:X}, asm]")
        lines[hj + 1:hj + 1] = new
        print(f"stray 0x{a:08X}-0x{b:08X}: yaml " + (", ".join(x.strip() for x in new) or "replaced in place"))

    # Function symbols strictly inside a fixed range stop being functions.
    scan_text = SCAN.read_text()
    size_text = SIZES.read_text()
    known = {}
    for text in (scan_text, size_text):
        for n, v in re.findall(r"^(\w+) = 0x([0-9A-Fa-f]+);", text, re.M):
            known[n] = int(v, 16)
    for n, v in known.items():
        if n in sizes:
            continue
        for s, e in ranges:
            if s < v < e or (v == s and any(k == "stray" and a == s for k, a, _, _ in cuts)):
                drop.add(n)
    for n, (v, _) in sizes.items():
        drop.discard(n)
    # Drop also the scan/size entries of names the block now defines (the block
    # is the one place they live), so no file disagrees with it.
    owned = set(sizes) | drop

    # A C file wired for a function inside a fixed range (named by address,
    # code/func_X) is a fragment too, even when no symbol file lists it.
    for l in lines:
        m = ENTRY.match(l)
        if m and m.group(3) == "c" and (f := re.match(r"code/func_([0-9A-F]{8})$", m.group(4) or "")):
            v = int(f.group(1), 16)
            if any(s < v < e for s, e in ranges) and f"func_{v:08X}" not in sizes:
                drop.add(f"func_{v:08X}")
    # Taking a fragment's C out leaves two asm subsegments in a row, and splat
    # would cut the function at the second: one asm entry each run.
    out, prev = [], None
    for l in lines:
        m = ENTRY.match(l)
        if m and m.group(3) in ("c", ".rodata") and re.match(rf"code/(?:{'|'.join(sorted(drop)) or '-'})$", m.group(4) or ""):
            print(f"  {m.group(4)[5:]}: its C goes back to asm (a fragment of a whole function)")
            if args.write:
                (SRC / "code" / f"{m.group(4)[5:]}.c").unlink(missing_ok=True)
            continue
        if m and m.group(3) == "asm" and prev == "asm":
            continue
        prev = m.group(3) if m else (prev if l.strip().startswith("#") or not l.strip() else None)
        out.append(l)
    lines = out
    yaml_text = "\n".join(lines) + "\n"
    for n in sorted(drop):
        # C wired for a fragment goes back to asm, inside the whole function.
        pat = rf"^\s+- \[0x[0-9A-F]+, (?:c|\.rodata), code/{n}\]\n"
        if re.search(pat, yaml_text, re.M):
            print(f"  {n}: its C goes back to asm (a fragment of a whole function)")
            yaml_text = re.sub(pat, "", yaml_text, flags=re.M)
            if args.write:
                (SRC / "code" / f"{n}.c").unlink(missing_ok=True)
    block = [BEGIN,
             "// Generated from versions/cuts.us.txt by tools/fix_cut.py; edit that."]
    for n, (v, sz) in sorted(sizes.items(), key=lambda x: x[1][0]):
        block.append(f"{n} = 0x{v:08X}; // type:func" + (f" size:0x{sz:X}" if sz else ""))
    block.append(END)
    size_text = re.sub(re.escape(BEGIN) + r".*?" + re.escape(END) + r"\n?", "", size_text, flags=re.S)
    size_text = "".join(l for l in size_text.splitlines(keepends=True)
                        if l.split(" =", 1)[0].strip() not in owned)
    size_text = size_text.rstrip("\n") + "\n\n" + "\n".join(block) + "\n"
    scan_new = "".join(l for l in scan_text.splitlines(keepends=True)
                       if l.split(" =", 1)[0].strip() not in owned)
    print(f"{len(cuts)} fixes: {len(stubs)} stray stubs, {len(sizes)} function extents, "
          f"{len(drop)} symbols dropped ({', '.join(sorted(drop)) or 'none'})")
    if not args.write:
        return 0
    for a, (b, text) in stubs.items():
        (STUBS / f"stray_{a:08X}.s").write_text(text)
    # A stub dropped from the list: its file goes too.
    for p in STUBS.glob("stray_*.s"):
        if int(p.stem[6:], 16) not in stubs:
            p.unlink()
    YAML.write_text(yaml_text)
    SIZES.write_text(size_text)
    SCAN.write_text(scan_new)
    return 0


if __name__ == "__main__":
    sys.exit(main())
