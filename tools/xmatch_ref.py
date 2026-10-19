#!/usr/bin/env python3
"""Find another decomp's C functions in the Turok 2 ROM.

Generalises tools/xmatch_t3.py to any decomp of a game on the same Acclaim
engine and compiler (SN64 GCC 2.8.1): Turok: Rage Wars has two
(references/rw_abu = AbuN64/ragewars-decomp, references/rw_junk =
thejunkman9000/Turok-Rage-Wars-Decomp). Each of the reference's .c files is
compiled with Turok 2's toolchain (cc1 -O2, tools/sn_as.sh); every function
that comes out is compared, relocations masked, with the pending Turok 2
functions:

  identical  same size, every word equal: its C matches as it is
  near       same size, at least --near of the words equal: a draft

Writes build/xmatch_<tag>/matches.json. Run where cc1 runs natively (the x86
build box) or inside the turok2-build container.

Usage:
    tools/xmatch_ref.py references/rw_abu --tag abu [--include include ...]
                        [--define VERSION_US_REV1] [--jobs 32] [--near 0.85]
"""

import argparse
import json
import os
import struct
import subprocess
import sys
from collections import defaultdict
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
import match_func  # noqa: E402

CC = ("tools/sn64/cc1 -quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mno-fix4300 "
      "-mhard-float -mfp64")


def compile_one(src, out, incs, defs, repo=None):
    import hashlib
    o = out / (hashlib.sha1(src.as_posix().encode()).hexdigest()[:16] + ".o")   # names can be too long for a file
    if o.exists():
        return o
    i, s = o.with_suffix(".i"), o.with_suffix(".s")
    cpp = (["cpp", "-P", "-undef", "-lang-c", "-D_LANGUAGE_C", "-DF3DEX_GBI_2", "-D__GNUC__=2", "-nostdinc"]
           + [f"-D{d}" for d in defs] + [f"-I{d}" for d in incs + [str(src.parent)]] + [str(src), "-o", str(i)])
    if subprocess.run(cpp, capture_output=True).returncode:
        return None
    if subprocess.run(CC.split() + [str(i), "-o", str(s)], capture_output=True).returncode:
        return None
    # Its asm may .include files relative to its own root (include/labels.inc).
    env = {**os.environ, "INC_DIR": str(repo)} if repo else None
    if subprocess.run(["tools/sn_as.sh", str(s), str(o)], capture_output=True, env=env).returncode:
        return None
    return o


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("repo", type=Path)
    ap.add_argument("--tag", required=True)
    ap.add_argument("--include", nargs="*", default=["include"], help="include dirs, relative to the repo")
    ap.add_argument("--define", nargs="*", default=[])
    ap.add_argument("--exclude", nargs="*", default=[], help="path substrings to skip")
    ap.add_argument("--jobs", type=int, default=os.cpu_count() or 8)
    ap.add_argument("--near", type=float, default=0.85)
    args = ap.parse_args()

    out = ROOT / "build" / f"xmatch_{args.tag}"
    out.mkdir(parents=True, exist_ok=True)
    incs = [str(args.repo / d) for d in args.include]
    srcs = [p for p in sorted(args.repo.rglob("src/**/*.c"))
            if not any(x in p.as_posix() for x in args.exclude)]
    with ThreadPoolExecutor(args.jobs) as pool:
        objs = list(pool.map(lambda p: compile_one(p, out, incs, args.define, args.repo), srcs))
    built = [(s, o) for s, o in zip(srcs, objs) if o]
    print(f"{len(srcs)} files, {len(built)} compiled")

    have = {p.stem for p in (ROOT / "src").rglob("func_*.c")}
    rom = match_func.ROM.read_bytes()
    t2 = defaultdict(list)                       # size -> [(name, words)]
    for name, (off, size) in match_func.rom_symbols().items():
        if name.startswith("func_") and size and name not in have:
            t2[size].append((name, struct.unpack(f">{size // 4}I", rom[off:off + size])))

    found, seen = {}, 0
    for src, obj in built:
        for fname, (words, relocs) in match_func.object_functions(obj).items():
            if fname == "gcc2_compiled." or len(words) < 3:
                continue
            seen += 1
            want = [match_func.mask(w, relocs.get(i)) for i, w in enumerate(words)]
            for name, ws in t2.get(4 * len(words), []):
                same = sum(match_func.mask(ws[i], relocs.get(i)) == want[i] for i in range(len(want)))
                ratio = same / len(want)
                if ratio >= args.near:
                    best = found.get(name)
                    if not best or ratio > best["ratio"]:
                        found[name] = {"ratio": ratio, "ref": fname, "file": str(src.relative_to(args.repo)),
                                       "words": len(want)}
    (out / "matches.json").write_text(json.dumps(found, indent=1))
    ident = sorted(n for n, m in found.items() if m["ratio"] == 1.0)
    print(f"{seen} functions compiled; Turok 2 pending functions found: "
          f"{len(ident)} identical, {len(found) - len(ident)} near (>= {args.near:.0%})")
    print(f"identical code: {sum(found[n]['words'] * 4 for n in ident):,} bytes")
    return 0


if __name__ == "__main__":
    sys.exit(main())
