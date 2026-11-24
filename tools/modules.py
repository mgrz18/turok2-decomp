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

Each module's C lives in its own directory (round 23): src/us/code/<module>/ for
.code, src/us/vm1/<module>/ and src/us/vm2/<module>/ for the two copies of the
paged VM module (the second copy under its first-copy twin's name), and the
host-only libultra C the N64 build takes from lib/ultralib in
src/port_libultra/<file>/ (src_dir below; versions/modules.us.txt lists each
module's directory).

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
    0x002A3708: ("exception_context", "known: exception context save/return, asm (docs/ASM_ORIGIN.md)"),
    0x002C22D8: ("code_tail_fragments", "known: dead copies of older code (docs/ASM_ORIGIN.md)"),
    0x002A50E0: ("libgcc/_cmpdi2", "known: GCC 2.8.1 libgcc2.c L_cmpdi2"),
    0x002A5130: ("libgcc/_floatdisf", "known: GCC 2.8.1 libgcc2.c L_floatdisf"),
    0x002A3AE0: ("libgcc/_divdi3", "known: GCC 2.8.1 libgcc2.c L_divdi3 (file header)"),
    0x002A40D0: ("libgcc/_moddi3", "known: GCC 2.8.1 libgcc2.c L_moddi3"),
    0x002A4650: ("libgcc/_udivdi3", "known: GCC 2.8.1 libgcc2.c L_udivdi3"),
    0x002A4BE0: ("libgcc/_umoddi3", "known: GCC 2.8.1 libgcc2.c L_umoddi3"),
}

# Linker padding in .rodata (docs/MODULES.md, round 25 part n2 and round 26 part o2;
# tools/rodata_gaps.py lists them): stale non-zero bytes, shorter than 16, no symbol, before a
# .rodata slice on a 16-byte boundary. The slice after is the first of another object, which
# starts at a 16-aligned function between the function owning the slice before and the one
# owning the slice after. One candidate: hard; several: the one where the name prefix changes
# (soft, noted). (region, first vram) -> evidence. A VM boundary holds in the second copy too.
RODATA_PAD = {
    ("engine", 0x002427E0): "rodata pad: at 0xA7478 (of func_00241E30, func_002427E0: where Platform_ gives way to AI_)",
    ("engine", 0x0026F760): "rodata pad: at 0xA8C94 (of 3 candidates: where Plane_ gives way to am*)",
    ("engine", 0x00270240): "rodata pad: at 0xA8CB8 (of 5 candidates: where __am* gives way to CAudioWorld__)",
    ("engine", 0x00271E20): "rodata pad: at 0xA8CEC (of func_00271B20, func_00271E20: where CAudioWorld__ gives way to Sound_)",
    ("engine", 0x002333C0): "rodata pad: at 0xA7034 (one candidate)",
    ("engine", 0x00236EA0): "rodata pad: at 0xA7144 (of 3 candidates: where Swoosh gives way to ImpactEvent_)",
    ("engine", 0x002393D0): "rodata pad: at 0xA71FC (of 2 candidates: where ImpactEvent_ gives way to Path_)",
    ("engine", 0x00239F00): "rodata pad: at 0xA7208 (one candidate)",
    ("engine", 0x0023D050): "rodata pad: at 0xA72B4 (one candidate)",
    ("engine", 0x0023E820): "rodata pad: at 0xA72D8 (of 2 candidates: where Sweep_ gives way to CPlayer__)",
    ("engine", 0x0026DFB0): "rodata pad: at 0xA8C3C (one candidate)",
    # round 27: stale bytes rodata_gaps.py does not class as PADDING (a string tail, an older
    # build's code words) end two slices on 16 inside the old CInstancePool (0xA6C70, 0xA6CA0)
    ("engine", 0x00227C10): "rodata pad: at 0xA6C6C (stale 'e\\en'; of func_00227AE0, func_00227C10: where CLevelActorPool__ gives way to CInstancePool__)",
    ("engine", 0x002284E0): "rodata pad: at 0xA6C96 (stale words after 'anim object info'; of func_002280B0, func_002284E0: where CInstancePool__ gives way to DLights_)",
    # round 29 (r4): stale bytes after "Lit Vertices" end a slice on 16 inside the old CMtxF
    ("engine", 0x002141F0): "rodata pad: at 0xA63C9 (stale words after Lit Vertices; where Lighting_ gives way to CStaticObject__)",
    ("vm1", 0x00424B90): "rodata pad: at 0xC2368 (zeros after the string \" \" up to the next 16; one candidate)",
    ("vm1", 0x00419560): "rodata pad: at 0xC1C28 (of 10 candidates: where the multiplayer menus give way to Single/Title/Pause)",
    ("vm1", 0x00423E10): "rodata pad: at 0xC2314 (of 3 candidates: where PakConfirmMenu_ gives way to Video_)",
    ("vm1", 0x00401E30): "rodata pad: at 0xC0F14 (of 4 candidates: where Automap_ gives way to MantidQueen_)",
    ("vm1", 0x004037B0): "rodata pad: at 0xC0F64 (of 6 candidates: where MantidQueen_ gives way to Primagen_)",
}

# Readable names where the dominant prefix is taken or says less than the functions do:
# (region, first vram) -> name. Everything else keeps the dominant prefix.
RENAMES = {
    # Collision_SweepRegions alone, padded off Collision's region/map wall sweeps
    ("engine", 0x00241010): "CollisionRegions",
    # GameObject_ApplyCurrent, SlideTo, TestMove, CanSee, CanMoveTo, PlayLandSound: moving an object
    ("engine", 0x00269050): "GameObjectMove",
    # DList/NodePool/WeightedTable helpers and GameObject_EmitHotPoints (round 26: padded off the
    # level events in .rodata; the 31 were one module before)
    ("engine", 0x0026D230): "DList",
    # the level events and their triggers (Event_*, LevelEvent_*: 17 of its 19 functions)
    ("engine", 0x0026DFB0): "LevelEvent",
    # round 26, modules found by .rodata padding (RODATA_PAD) whose dominant prefix is taken:
    # Sweep_Hit*, the Quad_/Plane_ shape tests (Collision_MoveInstance first)
    ("engine", 0x00239F00): "SweepShapes",
    # Collision_TestModels, ModelSweep_*, Collision_TestInstances
    ("engine", 0x0023D050): "ModelSweep",
    # Platform_CollideGround/Box/Cylinder, HitFace: an actor against a platform
    ("engine", 0x00241960): "PlatformCollide",
    # amCreateAudioMgr, __amMain, the audio DMA callbacks
    ("engine", 0x0026F760): "AudioMgr",
    # round 27: DLights_Load, DLights_SwapBuffers, padded off CInstancePool in .rodata and off
    # the DLights_GetAtPoint .. CDLightSet module in .text
    ("engine", 0x002284E0): "DLightsLoad",
    # Automap_RevealRegion .. Automap_EdgeVisible (the one-function `Automap` module is later)
    ("vm1", 0x00400000): "AutomapDraw",
    # SingleMenu, TitleMenu, QuitMenu, PauseMenu, SoundMenu: the front-end menus
    ("vm1", 0x00419560): "TitleMenu",
    # CScene__Construct, LoadLevel, the Decompress* sections, warps, UnloadLevel: the level loader
    # the engine pages in (MatrixHeap_Construct first)
    ("vm1", 0x00429F20): "SceneLoad",
}


def src_dirs(mods):
    """id(module) -> its directory under the repo root, for every module that has C.

    .code: src/us/code/<name>; the paged VM module: src/us/vm1/<name> (a `#n` dropped: the
    VM's own tree) and src/us/vm2/<its first-copy twin's directory name>, paired in order
    (both copies link the same objects in the same order). A libultra module keeps its
    ultralib path without the `.c` (code/libultra/io/pfsisplug)."""
    def clean(n):
        n = re.sub(r"#\d+$", "", n)
        n = re.sub(r"_2$", "", n)
        return re.sub(r"\.[cs]$", "", n)
    vm1 = [m for m in mods if m["region"] == "vm1"]
    vm2 = [m for m in mods if m["region"] == "vm2"]
    out = {}
    for m in mods:
        if m["region"] == "engine":
            out[id(m)] = f"src/us/code/{clean(m['name'])}"
        elif m["region"] == "vm1":
            out[id(m)] = f"src/us/vm1/{clean(m['name'])}"
    # module 2 starts inside module 1 (its first module is the twin of a later module-1 one)
    first = next((i for i, a in enumerate(vm1) if clean(a["name"]) == clean(vm2[0]["name"])), 0) if vm2 else 0
    for a, b in zip(vm1[first:], vm2):
        twin = clean(a["name"])
        if clean(b["name"]) != twin and not b["name"].startswith("unk_"):
            raise SystemExit(f"module 2 out of step with module 1: {b['name']} against {a['name']}")
        out[id(b)] = f"src/us/vm2/{twin}"
    if len(vm2) > len(vm1) - first:
        raise SystemExit("module 2 has more modules than module 1")
    return out


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
        if (region(f), f[0]) in RODATA_PAD:
            starts.setdefault(i, set()).add("rodata pad")
    # The two VM copies come from two links of the same objects: a padding boundary
    # between two functions in one copy is one between their twins in the other.
    base = lambda n: n[:-2] if n.endswith("_2") else n
    where = {}                                    # (region, base name) -> index
    for i, f in enumerate(fs):
        if region(f) in ("vm1", "vm2") and not re.match(r"func_[0-9A-F]{8}$", f[2]):
            where[(region(f), base(f[2]))] = i
    for i in sorted(starts):
        rg = region(fs[i])
        if rg not in ("vm1", "vm2") or not ({"pad", "gap", "rodata pad"} & starts[i]) or i == 0:
            continue
        other = "vm2" if rg == "vm1" else "vm1"
        j = where.get((other, base(fs[i][2])))
        k = where.get((other, base(fs[i - 1][2])))
        if j is not None and k == j - 1 and j not in starts:
            starts[j] = {"rodata pad(other copy)" if "rodata pad" in starts[i] else "pad(other copy)"}
    idx = sorted(starts) + [len(fs)]
    mods = []
    for a, b in zip(idx, idx[1:]):
        run = fs[a:b]
        ev = sorted(starts[a])
        if (region(run[0]), run[0][0]) in RODATA_PAD:
            ev.append(RODATA_PAD[(region(run[0]), run[0][0])])
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
    for m in mods:
        m["name"] = RENAMES.get((m["region"], m["funcs"][0][0]), m["name"])
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
                 "// docs/MODULES.md explains the evidence; module names are inferred, not original file names.",
                 "// `dir`: where the module's C files are (tools/srcfiles.py; libultra modules built from",
                 "// lib/ultralib keep only their host-only copies, under src/port_libultra)."]
        dirs = src_dirs(mods)
        for m in mods:
            f0, f1 = m["funcs"][0], m["funcs"][-1]
            lines.append(f"\nmodule {m['name']}: 0x{f0[0]:08X}..0x{f1[0] + f1[1]:08X}, "
                         f"{len(m['funcs'])} functions; {' + '.join(m['evidence'])}")
            lines.append(f"  // dir {dirs[id(m)]}")
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
