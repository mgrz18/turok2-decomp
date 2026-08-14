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
ABSOLUTE = ROOT / "versions" / "undefined_funcs_extra.us.txt"
EXISTING = ROOT / "versions" / "symbol_addrs.us.txt"

JAL_OPCODE = 0x03          # 000011
JR_RA = 0x03E00008
KSEG0 = 0x80000000  # solo para datos; el codigo corre en useg


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
        # Overlay members share a VRAM window, so a bare address does not say
        # which one a symbol belongs to. splat resolves that with a
        # `segment:<name>` attribute, and the owning segment is knowable here
        # because the seed came from a specific ROM range.
        owner = parent.get("name") if parent.get("exclusive_ram_id") else None
        out.append((start, end, vram, name, owner))
    return out


def rom_to_vram(segments, off):
    for start, end, vram, _name, _owner in segments:
        if start <= off < end:
            return vram + (off - start)
    return None


def vram_to_segment(segments, addr):
    for start, end, vram, name, _owner in segments:
        if vram <= addr < vram + (end - start):
            return name
    return None


def _is_virtual(addr):
    """The provisionally-mapped VM window — see the note in scan()."""
    return 0x00400000 <= addr < 0x004C4340


def in_overlay(segments, addr):
    """True when the address falls inside a segment that shares a VRAM window."""
    for start, end, vram, _name, owner in segments:
        if owner and vram <= addr < vram + (end - start):
            return True
    return False


def seed_name(addr, owner):
    """Overlay members need module-unique names.

    Three modules share the window at 0x80400000, so the same address is a
    different function in each. A single `func_804xxxxx` per address makes the
    linker see 91 duplicate definitions; prefixing with the owning segment
    keeps them distinct.
    """
    return f"{owner}_func_{addr:08X}" if owner else f"func_{addr:08X}"


def harvest_overlay_asm(segments):
    """Give module-unique names to overlay functions splat found on its own.

    splat discovers functions our scan misses and auto-names them `func_<addr>`.
    In the shared window that produces the same name in more than one module,
    which the linker rejects. Re-seeding each of them under the owning segment
    makes splat use our name instead.
    """
    import re

    globl = re.compile(r"^\s*\.globl\s+(func_[0-9A-Fa-f]{8})\s*$")
    found = set()
    for start, _end, _vram, name, owner in segments:
        if not owner:
            continue
        path = ROOT / "us" / "asm" / f"{name}.s"
        if not path.exists():
            continue
        with path.open(errors="replace") as fh:
            for line in fh:
                m = globl.match(line)
                if m:
                    found.add((int(m.group(1)[5:], 16), owner))
    return found


def scan(rom, segments):
    # Sets of (address, owner): the same address is a distinct function in each
    # overlay, so it can legitimately appear more than once.
    jal_targets = set()
    prologues = set()
    outside = Counter()
    outside_targets = set()

    for start, end, vram, _name, owner in segments:
        end = min(end, len(rom) - 3)
        for off in range(start, end, 4):
            word = struct.unpack_from(">I", rom, off)[0]

            # The `virtual` segment's mapping is still provisional — delta
            # voting gave it 46.7% against ~1% chance, which is well above
            # noise but far from the 65.6% the engine scored (#23). At that
            # confidence a good share of what reads as `jal` there is data,
            # and the targets it produces are noise.
            #
            # Measured: of the cross-section calls landing at an address with
            # no boundary evidence at all, 1,680 come from `virtual` and 3
            # from `code`. Seeding those fragments real engine functions, and
            # that is what has been making boundary fixes oscillate — each
            # pass corrected one class and the noise re-broke another.
            #
            # So `virtual` contributes prologues, which are local evidence,
            # but not jal targets, which depend on its mapping being right.
            if (word >> 26) == JAL_OPCODE and not _is_virtual(vram):
                # A `jal` carries only bits [27:2]; the top nibble comes from
                # the delay slot's PC. Hardcoding KSEG0 here was wrong: the
                # engine runs TLB-mapped in useg, so its calls resolve to
                # 0x002xxxxx, not 0x802xxxxx. libultra and the data segments
                # do live in KSEG0, and taking the nibble from the PC gets
                # both right. See docs/VRAM-LAYOUT.md.
                pc = vram + (off - start)
                target = (pc & 0xF0000000) | ((word & 0x03FFFFFF) << 2)
                if vram_to_segment(segments, target):
                    # Seed a real boundary, or a call that crosses sections.
                    #
                    # N64Recomp's resolve_jal creates a static function itself
                    # when a call lands mid-function *within the same section*
                    # (JalResolutionResult::CreateStatic). Seeding those fights
                    # it: the split leaves the parent too small and its own jump
                    # tables then point outside it. func_0029C270 came out 0x28
                    # bytes against a 0x658 body that way.
                    #
                    # That path is only taken for in-section targets. A call
                    # from another section — the `virtual` modules reaching into
                    # the engine — falls through to NoMatch, so those do need a
                    # symbol even when they land mid-function.
                    # Cross-section calls were also seeded here for a while,
                    # on the reasoning that CreateStatic only fires in-section.
                    # It measured worse: the extra symbols re-broke boundaries
                    # faster than they resolved calls. Left out until there is
                    # a way to add them without splitting their parent.
                    if is_function_start(rom, segments, target):
                        # Attribute to an overlay only when the target itself
                        # lands in the shared window: an overlay calling the
                        # engine must not tag the engine's address.
                        jal_targets.add(
                            (target, owner if in_overlay(segments, target) else None))
                else:
                    outside[target >> 16] += 1
                    outside_targets.add(target)

            if (word >> 16) == 0x27BD and (word & 0xFFFF) >= 0x8000:
                # function start only if the previous instruction pair is a
                # return: `jr ra` at off-8, its delay slot at off-4.
                if off - 8 >= start:
                    prev = struct.unpack_from(">I", rom, off - 8)[0]
                    if prev == JR_RA:
                        prologues.add((vram + (off - start), owner))

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


def harvest_link_log(path, segments, rom):
    """Split `ld` undefined references into seedable vs absolute-only.

    The linker names every address the disassembly reaches that we failed to
    define -- including function-pointer and jump-table targets, which a `jal`
    scan cannot see. Two different fixes apply:

    * address inside a known segment -> seed it, so splat emits a real label
      there. This is the case worth having: it adds a function boundary, which
      is the actual deliverable for N64Recomp.
    * address outside every segment -> nothing can place it yet, so define it
      absolutely for the linker. A `jal` to an absolute symbol still assembles
      back to the original word, so byte-exactness is unaffected.
    """
    import re

    names = set()
    with open(path, errors="replace") as fh:
        for line in fh:
            m = re.search(r"undefined reference to `([^']+)'", line)
            if m:
                names.add(m.group(1))
                continue
            # N64Recomp reports its own gaps differently, and they are the same
            # problem: an address the code calls that no function covers.
            m = re.search(r"No function found for jal target: 0x([0-9A-Fa-f]+)", line)
            if m:
                names.add(f"func_{int(m.group(1), 16):08X}")

    inside, outside = {}, {}
    for name in names:
        # Match the trailing hex of `func_00202110` / `.L002206DC`, not a
        # leading 8: code now lives in useg, so most addresses start at 0 and
        # an 0x8-anchored pattern silently stopped matching anything.
        m = re.search(r"([0-9A-Fa-f]{6,8})$", name)
        if not m:
            continue
        addr = int(m.group(1), 16)
        # GNU as keeps `.L`-prefixed symbols local, so they never reach the
        # symbol table and cross-object references to them cannot resolve no
        # matter where we seed them. Those always go the absolute route.
        # An address inside the shared overlay window cannot be attributed to a
        # module from the linker's message alone, and seeding it unqualified
        # defines the same name in every module that covers it. Route those to
        # the absolute file instead.
        if (name.startswith(".L")
                or not vram_to_segment(segments, addr)
                or in_overlay(segments, addr)):
            outside[name] = addr
        elif is_function_start(rom, segments, addr):
            inside[name] = addr
        else:
            # Inside a segment but not at a function boundary: a jump-table or
            # branch target in the middle of a function. Seeding it makes splat
            # cut the function there, and N64Recomp then rejects the piece with
            # "Unhandled branch" because the body jumps past its own end.
            outside[name] = addr
    return inside, outside


def is_function_start(rom, segments, addr):
    """True when `addr` sits right after a `jr ra` + delay slot in the ROM."""
    off = vram_to_rom(segments, addr)
    if off is None or off < 8 or off >= len(rom) - 3:
        return False
    return struct.unpack_from(">I", rom, off - 8)[0] == JR_RA


def vram_to_rom(segments, addr):
    for start, end, vram, _name, _owner in segments:
        if vram <= addr < vram + (end - start):
            return start + (addr - vram)
    return None


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--write", action="store_true", help="escribir el archivo de seeds")
    ap.add_argument("--from-link-log", type=Path,
                    help="cosechar simbolos sin definir de un log de ld")
    ap.add_argument("--from-overlay-asm", action="store_true",
                    help="renombrar por modulo los func_ que splat auto-nombro en overlays")
    args = ap.parse_args()

    if args.from_overlay_asm:
        segments = load_code_segments()
        found = harvest_overlay_asm(segments)
        print(f"funciones de overlay auto-nombradas por splat: {len(found):,}")
        by = Counter(o for _a, o in found)
        for o, n in sorted(by.items()):
            print(f"  {o}: {n:,}")
        if args.write and found:
            existing = OUT.read_text() if OUT.exists() else ""
            new = [f"{seed_name(a, o)} = 0x{a:08X}; // type:func segment:{o}"
                   for a, o in sorted(found, key=lambda t: (t[0], t[1]))
                   if seed_name(a, o) not in existing]
            if new:
                with OUT.open("a") as fh:
                    fh.write("\n// Overlay functions splat auto-named; re-seeded per module.\n")
                    fh.write("\n".join(new) + "\n")
            print(f"\nagregados: {len(new):,} -> {OUT.relative_to(ROOT)}")
        return

    if args.from_link_log:
        segments = load_code_segments()
        inside, outside = harvest_link_log(args.from_link_log, segments,
                                           ROM.read_bytes())
        print(f"simbolos sin definir en el log : {len(inside) + len(outside):,}")
        print(f"  dentro de segmento (sembrar) : {len(inside):,}")
        print(f"  fuera  de segmento (absoluto): {len(outside):,}")
        if args.write:
            existing = OUT.read_text() if OUT.exists() else ""
            extra = [f"{n} = 0x{a:08X}; // type:func"
                     for n, a in sorted(inside.items(), key=lambda kv: kv[1])
                     if n not in existing]
            if extra:
                with OUT.open("a") as fh:
                    fh.write("\n// Cosechados de referencias sin definir del linker.\n")
                    fh.write("\n".join(extra) + "\n")
            # Merge, never replace: each link pass only reports what still
            # fails, so overwriting would drop everything a previous pass
            # already fixed and the link would regress.
            merged = dict(outside)
            if ABSOLUTE.exists():
                for line in ABSOLUTE.read_text(errors="replace").splitlines():
                    line = line.strip()
                    if not line or line.startswith(("/*", "*", "//")) or "=" not in line:
                        continue
                    name, _, value = line.partition("=")
                    try:
                        merged.setdefault(name.strip(),
                                          int(value.strip().rstrip(";"), 16))
                    except ValueError:
                        continue
            ABSOLUTE.write_text(
                "/* Generado por tools/function_seed.py --from-link-log.\n"
                " * Direcciones que ningun segmento ubica todavia; se definen\n"
                " * absolutas para que el link cierre. Un jal a un simbolo\n"
                " * absoluto reensambla al word original. */\n"
                + "\n".join(f"{n} = 0x{a:08X};"
                            for n, a in sorted(merged.items(), key=lambda kv: kv[1]))
                + "\n")
            print(f"\nsembrados nuevos : {len(extra):,} -> {OUT.relative_to(ROOT)}")
            print(f"absolutos        : {len(merged):,} "
                  f"({len(outside):,} de este log) -> {ABSOLUTE.relative_to(ROOT)}")
        return

    if not ROM.exists():
        sys.exit(f"falta {ROM}")

    rom = ROM.read_bytes()
    segments = load_code_segments()
    jal_targets, prologues, outside, outside_targets = scan(rom, segments)

    known = load_existing()
    pairs = sorted(prologues | jal_targets, key=lambda t: (t[0], t[1] or ""))
    seeds = [(a, o) for a, o in pairs if a not in known]

    print(f"segmentos de codigo      : {len(segments)}")
    print(f"targets de jal           : {len(jal_targets):,}")
    print(f"prologos tras `jr ra`    : {len(prologues):,}")
    print(f"union                    : {len(jal_targets | prologues):,}")
    print(f"ya conocidos (symbol_addrs): {len(known):,}")
    print(f"seeds nuevos             : {len(seeds):,}")
    print()

    per_segment = Counter()
    for addr, _o in seeds:
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
        lines += [f"{seed_name(a, o)} = 0x{a:08X}; // type:func"
                  + (f" segment:{o}" if o else "")
                  for a, o in seeds]
        OUT.write_text("\n".join(lines) + "\n")
        print()
        print(f"escrito: {OUT.relative_to(ROOT)} ({len(seeds):,} simbolos)")


if __name__ == "__main__":
    main()
