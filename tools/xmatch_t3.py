#!/usr/bin/env python3
"""Find Turok 3's matched engine functions in the Turok 2 ROM.

Turok 3 runs the same Acclaim engine and was built with the same SN64 GCC
2.8.1 and flags, and its decomp (Drahsid/turok3, cloned into
references/turok3) has engine modules in C with Acclaim's own names:
CList__AddHead, CHeap__..., memset. Code that did not change between the
two games compiles to the same instructions, so each of those functions,
compiled here with Turok 2's toolchain, can be compared word by word
(relocations masked) against every pending Turok 2 function of the same
size. An identical one gives a name and C that matches.

  1. compile every references/turok3/src/us/*.c in the container, with
     Turok 3's include paths and INCLUDE_ASM switched off (-DPERMUTER), so
     only the functions written in C come out;
  2. compare each against Turok 2's functions of equal size.

Writes build/xmatch/matches.json: {turok3 name: [turok2 functions]}.

Usage:
    python3 tools/xmatch_t3.py
"""

import json
import struct
import subprocess
import sys
from collections import defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
import match_func  # noqa: E402

T3 = "references/turok3"
OUT = ROOT / "build" / "xmatch"
CPP = (f"cpp -P -undef -lang-c -D_LANGUAGE_C -DF3DEX_GBI_2 -D__GNUC__=2 -DPERMUTER "
       f"-DGAME_VERSION=\\\"us\\\" -I{T3} -I{T3}/include -I{T3}/ultralib/include -nostdinc")
CC = ("tools/sn64/cc1 -quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mno-fix4300 "
      "-mhard-float -mfp64")


def compile_all():
    OUT.mkdir(parents=True, exist_ok=True)
    srcs = sorted((ROOT / T3 / "src" / "us").glob("*.c"))
    script = "; ".join(
        f"{CPP} {s.relative_to(ROOT)} > build/xmatch/{s.stem}.i 2>build/xmatch/{s.stem}.err && "
        f"{CC} build/xmatch/{s.stem}.i -o build/xmatch/{s.stem}.s 2>>build/xmatch/{s.stem}.err && "
        f"tools/sn_as.sh build/xmatch/{s.stem}.s build/xmatch/{s.stem}.o 2>>build/xmatch/{s.stem}.err "
        f"|| echo FAILED {s.stem}"
        for s in srcs)
    res = subprocess.run(["docker", "run", "--platform=linux/amd64", "--rm", "-v", f"{ROOT}:/work",
                          "-w", "/work", match_func.IMAGE, "bash", "-c", script],
                         capture_output=True, text=True)
    failed = [l.split()[1] for l in res.stdout.splitlines() if l.startswith("FAILED")]
    return srcs, failed


def main():
    srcs, failed = compile_all()
    print(f"{len(srcs)} Turok 3 files, {len(failed)} did not compile: {' '.join(failed)}")
    rom = match_func.ROM.read_bytes()
    t2 = defaultdict(list)                       # size -> [(name, words)]
    for name, (off, size) in match_func.rom_symbols().items():
        if name.startswith("func_") and size:
            t2[size].append((name, struct.unpack(f">{size // 4}I", rom[off:off + size])))
    matches, tried = {}, 0
    for s in srcs:
        obj = OUT / f"{s.stem}.o"
        if not obj.exists():
            continue
        for fname, (words, relocs) in match_func.object_functions(obj).items():
            if fname.startswith("func_") or fname == "gcc2_compiled.":
                continue
            tried += 1
            want = [match_func.mask(w, relocs.get(i)) for i, w in enumerate(words)]
            hits = [n for n, ws in t2.get(4 * len(words), [])
                    if all(match_func.mask(ws[i], relocs.get(i)) == want[i] for i in range(len(want)))]
            if hits:
                matches[fname] = {"file": s.stem, "turok2": hits}
    (OUT / "matches.json").write_text(json.dumps(matches, indent=1))
    print(f"{tried} named Turok 3 functions compiled; {len(matches)} found in Turok 2")
    for f, m in sorted(matches.items()):
        print(f"  {m['file']:12s} {f:40s} -> {' '.join(m['turok2'])}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
