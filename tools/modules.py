#!/usr/bin/env python3
"""Recover Turok 2's original object (source-file) boundaries: the module map.

Every function of build/ref.elf (one function per file today) is put in a
module: a run of functions that came from one original object file, or from a
few neighbouring ones when nothing in the ROM tells them apart. The evidence,
strongest first (docs/MODULES.md):

  pad       SN64's linker starts every object at a 16-byte boundary and fills
            the gap with whatever was in its buffer (stale strings, not nops):
            a gap between two functions is an object boundary, always.
  ultralib  in libultra/libaudio/libc, the ultralib file each function is
            (versions/ultralib.us.txt, tools/ultralib_version.py --map).
  names     inside a pad-delimited run, a change of class/module prefix
            (CHeap__* -> CHashTable__*) is taken as a boundary the padding
            cannot show (an object that ends on a 16-byte boundary).
  known     a few blocks named by outside evidence (KNOWN below).

The paged VM module is in the ROM twice; the second copy (.virtual_1) is
mapped onto the first (same module, `_2`), not analysed on its own.

Usage (from the repo root, needs build/ref.elf and baserom.us.z64):
    tools/modules.py                 # check, print a summary
    tools/modules.py --write         # also write versions/modules.us.txt
    tools/modules.py --md            # print the docs/MODULES.md module table
"""

import argparse
import os
import re
import struct
import sys
from collections import Counter, OrderedDict
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
os.environ.setdefault("MATCH_ELF", str(ROOT / "build" / "ref.elf"))
import match_func  # noqa: E402

OUT = ROOT / "versions" / "modules.us.txt"
UL_MAP = ROOT / "versions" / "ultralib.us.txt"
# Regions by ROM offset (the two VM copies are linked over the same addresses).
ENGINE = (0x00001100, 0x000C3074)       # .code (engine, libultra, libgcc) and its tail
VM1 = (0x0014A000, 0x00186C14)          # first paged VM module
VM2 = (0x00186C14, 0x001BFA00)          # its stale second copy, 0x39000 further in the ROM

# Blocks named by outside evidence: first vram -> (module, evidence). A block
# runs to the next boundary of any kind.
KNOWN = {
    0x00200C40: ("rnc", "known: RNC ProPack unpacker, hand-written asm (docs/ASM_ORIGIN.md)"),
    0x0028EE40: ("stvm_tlb", "known: STVM TLB helpers, asm (docs/ASM_ORIGIN.md)"),
    0x0029BFE0: ("libultra/os/exceptasm.s", "known: ultralib exceptasm.s (docs/ASM_ORIGIN.md)"),
    0x002A3088: ("exception_stubs", "known: exception-vector stubs, asm (docs/ASM_ORIGIN.md)"),
    0x002A3AE0: ("libgcc/_divdi3", "known: GCC 2.8.1 libgcc2.c L_divdi3 (file header)"),
    0x002A40D0: ("libgcc/_moddi3", "known: GCC 2.8.1 libgcc2.c L_moddi3"),
    0x002A4650: ("libgcc/_udivdi3", "known: GCC 2.8.1 libgcc2.c L_udivdi3"),
    0x002A4BE0: ("libgcc/_umoddi3", "known: GCC 2.8.1 libgcc2.c L_umoddi3"),
}


def functions():
    """[(vram, size, name, rom)] sorted; aliases (two names, one address) dropped."""
    from elftools.elf.elffile import ELFFile
    seen = {}
    with match_func.ELF.open("rb") as fh:
        elf = ELFFile(fh)
        for s in elf.get_section_by_name(".symtab").iter_symbols():
            if s["st_info"]["type"] != "STT_FUNC" or not s["st_size"]:
                continue
            r = match_func.sym_rom(elf, s)
            if r is None:
                continue
            old = seen.get(r)
            if old and not old[2].startswith("func_"):
                continue
            seen[r] = (s["st_value"], s["st_size"], s.name, r)
    return sorted(seen.values(), key=lambda f: f[3])


def prefix(name):
    """The class/module a function name says it belongs to, or None."""
    if re.match(r"func_[0-9A-F]{8}$", name):
        return None
    name = re.sub(r"_2$", "", name)
    if "__" in name.strip("_"):
        return name.strip("_").split("__")[0]
    m = re.match(r"([A-Za-z0-9]+)_[A-Za-z0-9]", name)
    if m:
        return m.group(1)
    return name


def ultralib_files():
    out = {}
    if UL_MAP.exists():
        for line in UL_MAP.read_text().splitlines():
            m = re.match(r"0x([0-9A-F]{8})\s+\d+\s+(\S+) = (\S+?)(?::\S+)?; (\S+)", line)
            if m and m.group(3) != "-":
                out[m.group(2)] = "libultra/" + m.group(3).removeprefix("src/")
    return out


def build(rom):
    fs = functions()
    ul = ultralib_files()                         # by name
    region = lambda f: "engine" if ENGINE[0] <= f[3] < ENGINE[1] else "vm1" if VM1[0] <= f[3] < VM1[1] \
        else "vm2" if VM2[0] <= f[3] < VM2[1] else None
    fs = [f for f in fs if region(f)]
    # hard boundaries: index i means "a new object starts at fs[i]"
    starts = {}                                   # index -> set(evidence)
    for i, f in enumerate(fs):
        prev = fs[i - 1] if i else None
        if prev is None or region(prev) != region(f):
            starts.setdefault(i, set()).add("region")
        elif f[3] > prev[3] + prev[1]:
            starts.setdefault(i, set()).add("pad" if f[3] % 16 == 0 else "gap")
        a, b = (ul.get(prev[2]) if prev else None), ul.get(f[2])
        if prev and region(prev) == region(f) and (a or b) and a != b:
            starts.setdefault(i, set()).add("ultralib")
        if f[0] in KNOWN and region(f) == "engine":
            starts.setdefault(i, set()).add("known")
    # The two VM copies come from two links of the same objects: a padding boundary
    # between two functions in one copy is one between their twins in the other.
    base = lambda n: n[:-2] if n.endswith("_2") else n
    where = {}                                    # (region, base name) -> index
    for i, f in enumerate(fs):
        if region(f) in ("vm1", "vm2") and not re.match(r"func_[0-9A-F]{8}$", f[2]):
            where[(region(f), base(f[2]))] = i
    for i in sorted(starts):
        rg = region(fs[i])
        if rg not in ("vm1", "vm2") or not ({"pad", "gap"} & starts[i]) or i == 0:
            continue
        other = "vm2" if rg == "vm1" else "vm1"
        j = where.get((other, base(fs[i][2])))
        k = where.get((other, base(fs[i - 1][2])))
        if j is not None and k == j - 1 and j not in starts:
            starts[j] = {"pad(other copy)"}
    idx = sorted(starts) + [len(fs)]
    mods = []
    for a, b in zip(idx, idx[1:]):
        run = fs[a:b]
        ev = sorted(starts[a])
        # soft boundaries: runs of one class/module prefix (func_ joins the run before it)
        parts, cur = [], None
        for f in run:
            p = prefix(f[2])
            if not parts or (p is not None and p != cur and cur is not None):
                parts.append([p, []])
            if p is not None:
                if parts[-1][0] is None:
                    parts[-1][0] = p
                cur = p
            parts[-1][1].append(f)
        if run[0][0] in KNOWN and region(run[0]) == "engine":
            name, why = KNOWN[run[0][0]]
            ev.append(why)
        elif run[0][2] in ul:
            name = ul[run[0][2]]
        else:
            c = Counter()
            for f in run:
                if prefix(f[2]):
                    c[prefix(f[2])] += f[1]
            name = c.most_common(1)[0][0] if c else f"unk_{run[0][0]:08X}"
        mods.append({"region": region(run[0]), "funcs": run, "name": name, "evidence": ev,
                     "parts": [(p or "?", len(fl)) for p, fl in parts]})
    # module 2: named as the module-1 module its functions' twins (same name less _2) are in
    by_name = {f[2]: m for m in mods if m["region"] == "vm1" for f in m["funcs"]}
    for m in mods:
        if m["region"] == "vm2":
            twins = Counter(by_name[base(f[2])]["name"] for f in m["funcs"]
                            if f[2].endswith("_2") and base(f[2]) in by_name)
            m["name"] = (twins.most_common(1)[0][0] if twins else m["name"]) + "_2"
    # unique names: a module name seen again gets #2, #3...
    count = Counter()
    for m in mods:
        count[m["name"]] += 1
        if count[m["name"]] > 1:
            m["name"] = f"{m['name']}#{count[m['name']]}"
    return fs, mods


def rodata_order(mods):
    """Each object's .rodata is linked in the same order as its .text: a module whose
    .rodata lies below the previous module's is out of order (a wrong boundary, or
    rodata shared across objects). Uses build/turok2.us.map (a `make` build)."""
    mp = ROOT / "build" / "turok2.us.map"
    if not mp.exists():
        return None
    text, ro = {}, {}
    for m in re.finditer(r"^ \.(text|rodata)\s+0x([0-9a-f]+)\s+0x([0-9a-f]+) (build/src/\S+\.c\.o)$",
                         mp.read_text(), re.M):
        sec, addr, size, obj = m.group(1), int(m.group(2), 16), int(m.group(3), 16), m.group(4)
        if size:
            (text if sec == "text" else ro)[obj] = addr
    ro_at = {text[o]: a for o, a in ro.items() if o in text}       # function vram -> its .rodata
    bad, last = [], None
    for m in mods:
        if m["region"] == "vm2":
            continue
        rs = [ro_at[f[0]] for f in m["funcs"] if f[0] in ro_at]
        if not rs:
            continue
        sect = 0 if min(rs) < 0x800BFD50 else 1      # .code_rodata or .code_rodata_tail
        if last and min(rs) < last[1] and (m["region"], sect) == last[2]:
            bad.append(f"{last[0]} -> {m['name']}")
        # (inside one module the order is not checked: the per-function files put a literal
        # shared by two functions with either, so even ultralib's synthesizer.c is "out of order")
        last = (m["name"], max(rs), (m["region"], sect))
    return bad


def check(fs, mods):
    errs = []
    flat = [f for m in mods for f in m["funcs"]]
    if flat != fs:
        errs.append("functions not assigned exactly once, in order")
    names = [m["name"] for m in mods]
    if len(set(names)) != len(names):
        errs.append("duplicate module names")
    for m in mods:
        fsm = m["funcs"]
        for x, y in zip(fsm, fsm[1:]):
            if y[3] - (x[3] + x[1]) > 0 and y[3] % 16 == 0:
                errs.append(f"{m['name']}: padding inside the module at {y[2]}")
    return errs


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--write", action="store_true", help=f"write {OUT.relative_to(ROOT)}")
    ap.add_argument("--md", action="store_true", help="print the markdown module table")
    args = ap.parse_args()
    rom = match_func.ROM.read_bytes()
    fs, mods = build(rom)
    errs = check(fs, mods)
    ev = Counter(e for m in mods for e in m["evidence"] if ":" not in e)
    multi = sum(1 for m in mods if len(m["parts"]) > 1 and m["region"] != "vm2")
    print(f"{len(fs)} functions, {len(mods)} modules "
          f"({sum(1 for m in mods if m['region'] == 'engine')} engine, "
          f"{sum(1 for m in mods if m['region'] == 'vm1')} VM module, "
          f"{sum(1 for m in mods if m['region'] == 'vm2')} module-2 copies); "
          f"boundaries by evidence: {dict(sorted(ev.items()))}; "
          f"{multi} modules hold more than one name prefix", file=sys.stderr)
    for e in errs:
        print("ERROR:", e, file=sys.stderr)
    ro = rodata_order(mods)
    if ro is None:
        print("(no build/turok2.us.map: .rodata order not checked)", file=sys.stderr)
    else:
        print(f".rodata order: {len(ro)} exceptions", file=sys.stderr)
        for b in ro:
            print("  rodata:", b, file=sys.stderr)
    if args.write:
        lines = ["// Turok 2's functions by original module (object file); written by tools/modules.py --write.",
                 "// module: first..last vram, functions, evidence for its start; then one function per line.",
                 "// docs/MODULES.md explains the evidence; module names are inferred, not original file names."]
        for m in mods:
            f0, f1 = m["funcs"][0], m["funcs"][-1]
            lines.append(f"\nmodule {m['name']}: 0x{f0[0]:08X}..0x{f1[0] + f1[1]:08X}, "
                         f"{len(m['funcs'])} functions; {' + '.join(m['evidence'])}")
            if len(m["parts"]) > 1:
                lines.append("  // name runs (possible objects the padding cannot show): "
                             + ", ".join(f"{p} {n}" for p, n in m["parts"]))
            lines += [f"  0x{f[0]:08X} {f[2]}" for f in m["funcs"]]
        OUT.write_text("\n".join(lines) + "\n")
        print(f"wrote {OUT.relative_to(ROOT)}", file=sys.stderr)
    if args.md:
        print("| Module | Region | First function | Last function | vram | Functions | Bytes | Start evidence |")
        print("|---|---|---|---|---|---:|---:|---|")
        for m in mods:
            f0, f1 = m["funcs"][0], m["funcs"][-1]
            print(f"| {m['name']} | {m['region']} | {f0[2]} | {f1[2]} | 0x{f0[0]:08X} | {len(m['funcs'])} | "
                  f"{f1[0] + f1[1] - f0[0]} | {', '.join(m['evidence'])} |")
    return 1 if errs else 0


if __name__ == "__main__":
    sys.exit(main())
