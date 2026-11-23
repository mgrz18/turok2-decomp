#!/usr/bin/env python3
"""Which libultra does Turok 2 link? Compile decompals/ultralib per version.

Every C file of lib/ultralib (the pinned submodule) is compiled with Turok 2's cc1 (SN64 GCC
2.8.1) for each ultralib version (2.0I/J/K/L) and target flavour (libgultra,
libgultra_rom), with the defines ultralib's Makefile and makefiles/gcc.mk give
that version (NDEBUG except 2.0I libgultra_rom, _FINALROM for _rom,
-funsigned-char up to 2.0J), under four code-generation configs (-O3 or -O2,
-mgp32 -mfp32 as ultralib or -mfp64 as the game). Each function that comes
out is compared, relocations masked, with every sized function of the
reference ELF (build/ref.elf) of the same size: identical = every word equal.

A function whose source differs between versions or flavours tells them apart
(docs/LIBULTRA.md). With --map it writes versions/ultralib.us.txt: for every
game function from the start of libaudio to the end of libc (and the Controller
Pak copies in the VM modules), the ultralib file and function it is, and
whether ultralib's C compiles to it.

Usage (on the x86 build box, from the repo root; ~1 min a version at -j6):
    tools/ultralib_version.py [--versions I J K L] [--jobs 6] [--map]
"""

import argparse
import json
import os
import re
import struct
import sys
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
os.environ.setdefault("MATCH_ELF", str(ROOT / "build" / "ref.elf"))
import match_func  # noqa: E402
import xmatch_ref  # noqa: E402

UL = ROOT / "lib" / "ultralib"   # the pinned submodule (docs/LIBULTRA.md)
CC = ("tools/sn64/cc1 -quiet -G0 -mips3 -mgas -meb -mno-fix4300 -mhard-float "
      "-mcpu=VR4300 -fforce-addr -fno-builtin")
CONFIGS = {                     # name -> extra cc1 flags
    "O3": "-O3 -mgp32 -mfp32",  # ultralib's gcc.mk
    "O3fp64": "-O3 -mfp64",
    "O2": "-O2 -mgp32 -mfp32",
    "O2fp64": "-O2 -mfp64",
}
O0_FILES = {"src/os/assert.c"}  # gcc.mk: assert.c is -O0
# The game's libultra/libaudio/libc: ROM 0x8FB90 (vram 0x0028EF90, __alCSeqGetTrackEvent)
# to the end of libc (0x0029F190, before Acclaim's own load.c); plus the pfs copies.
LIB_VRAM = (0x0028EF90, 0x0029F190)
MAP = ROOT / "versions" / "ultralib.us.txt"
# Game functions still under an address name (or another name) that are ultralib functions:
# cosf/sinf keep func_ names (GCC 2.8.1 treats them as builtins), sqrtf is sqrt.s,
# Rage Wars' __osUnmapTLBAll is ultralib's osUnmapTLBAll.
ALIAS = {"func_002974C0": "cosf", "func_002982F0": "sinf", "func_00298470": "sqrtf",
         "__osUnmapTLBAll": "osUnmapTLBAll"}


def flavours(versions):
    for v in versions:
        for rom in (False, True):
            defs = ["_MIPS_SZLONG=32", "__USE_ISOC99", "F3DEX_GBI", f"BUILD_VERSION=VERSION_{v}",
                    f'BUILD_VERSION_STRING="2.0{v}"']
            if not (v == "I" and rom):       # gcc.mk: 2.0I libgultra_rom has no NDEBUG
                defs.append("NDEBUG")
            if rom:
                defs.append("_FINALROM")
            yield f"{v}{'_rom' if rom else ''}", v, defs


def game_functions():
    """[(vram, size, name, rom offset)] of every sized function in build/ref.elf."""
    from elftools.elf.elffile import ELFFile
    out = []
    with match_func.ELF.open("rb") as fh:
        elf = ELFFile(fh)
        for s in elf.get_section_by_name(".symtab").iter_symbols():
            if s["st_info"]["type"] == "STT_FUNC" and s["st_size"]:
                r = match_func.sym_rom(elf, s)
                if r is not None:
                    out.append((s["st_value"], s["st_size"], s.name, r))
    return sorted(set(out))


def asm_names():
    """function name -> ultralib .s file, from LEAF()/XLEAF()/NESTED() and glabel."""
    out = {}
    for p in sorted((UL / "src").rglob("*.s")):
        for m in re.finditer(r"^\s*(?:X?LEAF|NESTED|glabel)\s*\(?\s*(\w+)", p.read_text(errors="replace"), re.M):
            out.setdefault(m.group(1), p.relative_to(UL).as_posix())
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--versions", nargs="*", default=["I", "J", "K", "L"])
    ap.add_argument("--configs", nargs="*", default=list(CONFIGS))
    ap.add_argument("--jobs", type=int, default=6)
    ap.add_argument("--map", action="store_true", help=f"write {MAP.relative_to(ROOT)} (needs version I)")
    args = ap.parse_args()

    incs = [str(UL / d) for d in (".", "include", "include/compiler/gcc", "include/PR")]
    srcs = sorted(p for p in (UL / "src").rglob("*.c") if "/voice/" not in p.as_posix())
    rom = match_func.ROM.read_bytes()
    funcs = game_functions()
    by_size, by_name = {}, {}
    for vram, size, name, off in funcs:
        ws = struct.unpack(f">{size // 4}I", rom[off:off + size])
        by_size.setdefault(size, []).append((name, ws))
        by_name[ALIAS.get(name, name)] = ws

    # result["file:function"] = {"file", "fn", "game": {flavour: {config: [names]}}, "near": {...}}
    result = {}
    out_root = ROOT / "build" / "ultralib_version"
    print(f"{'':8s}" + "".join(f"{c:>9s}" for c in args.configs) + "   (identical functions)")
    for tag, v, defs in flavours(args.versions):
        line = f"{tag:8s}"
        for cname in args.configs:
            cc = f"{CC} {CONFIGS[cname]}"
            if v in "DEFGHIJ":
                cc += " -funsigned-char"
            out = out_root / f"{tag}_{cname}"
            out.mkdir(parents=True, exist_ok=True)

            def one(p, cc=cc, out=out):
                rel = p.relative_to(UL).as_posix()
                c = re.sub(r"-O\d", "-O0", cc) if rel in O0_FILES else cc
                return xmatch_ref.compile_one(p, out, incs, defs, None, c)

            with ThreadPoolExecutor(args.jobs) as pool:
                objs = list(pool.map(one, srcs))
            nid = 0
            for src, obj in zip(srcs, objs):
                if not obj:
                    continue
                for fname, (words, relocs) in match_func.object_functions(obj).items():
                    if fname == "gcc2_compiled." or len(words) < 2:
                        continue
                    want = [match_func.mask(w, relocs.get(i)) for i, w in enumerate(words)]
                    hits = [n for n, ws in by_size.get(4 * len(words), [])
                            if all(match_func.mask(ws[i], relocs.get(i)) == want[i] for i in range(len(want)))]
                    # two-word functions (jr ra; nop and the like) match any stub: only by name
                    if len(words) <= 2:
                        hits = [n for n in hits if ALIAS.get(n, n).removesuffix("_2") == fname]
                    rel = src.relative_to(UL).as_posix()
                    ent = result.setdefault(f"{rel}:{fname}", {"file": rel, "fn": fname, "game": {}, "near": {}})
                    if hits:
                        nid += 1
                        ent["game"].setdefault(tag, {})[cname] = sorted(hits)
                    elif fname in by_name:
                        ws = by_name[fname]
                        same = sum(match_func.mask(ws[i], relocs.get(i)) == want[i]
                                   for i in range(min(len(ws), len(want))))
                        ent["near"].setdefault(tag, {})[cname] = [same, len(want), len(ws)]
            line += f"{nid:9d}"
        print(line, flush=True)
    (out_root / "result.json").write_text(json.dumps(result, indent=1))

    if args.map:
        write_map(result, funcs)
    return 0


def write_map(result, funcs):
    flav = "I_rom"
    asm = asm_names()
    cand = {}                                   # game name -> {(ultralib fn, file): configs}
    for e in result.values():
        for c, names in e["game"].get(flav, {}).items():
            for n in names:
                cand.setdefault(n, {}).setdefault((e["fn"], e["file"]), set()).add(c)
    game = {}                                   # game name -> (ultralib fn, file, configs)
    prev_file = None
    for vram, size, name, off in funcs:         # in address order: ties go to the neighbour's file
        if name not in cand:
            continue
        own = ALIAS.get(name, name).removesuffix("_2")
        opts = sorted(cand[name], key=lambda k: (k[0] != own, k[1] != prev_file, k))
        f, file = opts[0]
        # in the VM modules (the Controller Pak code) only the same name, or a long unnamed function:
        # short accessors there look like ultralib's one-line getters
        if off >= 0x14A000 and f != own and not (name.startswith("func_") and size >= 32):
            continue
        game[name] = (f, file, cand[name][opts[0]])
        prev_file = file
    near = {}                                   # game name -> (file, [same, ours, theirs])
    for e in result.values():
        for n, nr in ((g, v) for g in [e["fn"]] for v in [e["near"].get(flav)] if v):
            best = max(nr.values(), key=lambda x: x[0] / max(x[1], x[2]))
            near.setdefault(n, (e["file"], best))
    lines = [
        "// The game's libultra/libaudio/libc against decompals/ultralib 2.0I libgultra_rom.",
        "// Written by tools/ultralib_version.py --map (docs/LIBULTRA.md); do not edit.",
        "// vram size name = ultralib file:function; status",
        "//   identical [configs]  ultralib's C through SN64 cc1 gives these bytes (configs: -O3/-O2, fp32/fp64)",
        "//   differs a/b/c        the same name in ultralib, a of b words equal (c = the game's words)",
        "//   asm                  an ultralib .s file has it",
        "//   -                    not in ultralib (Acclaim's own, a padding/stray word, or renamed)",
    ]
    lo, hi = LIB_VRAM
    for vram, size, name, off in funcs:
        un = ALIAS.get(name, name)
        in_lib = lo <= vram < hi or (name in game and off >= 0x14A000)
        if not in_lib:
            continue
        if name in game:
            f, file, cs = game[name]
            st = f"{file}:{f}; identical {','.join(c for c in CONFIGS if c in cs)}"
        elif un in near:
            file, nr = near[un]
            st = f"{file}:{un}; differs {nr[0]}/{nr[1]}/{nr[2]}"
        elif un in asm:
            st = f"{asm[un]}:{un}; asm"
        else:
            st = "-; -"
        lines.append(f"0x{vram:08X} {size:5d} {name} = {st}")
    MAP.write_text("\n".join(lines) + "\n")
    print(f"wrote {MAP.relative_to(ROOT)}: {len(lines) - 7} functions")


if __name__ == "__main__":
    sys.exit(main())
