#!/usr/bin/env python3
"""Check a struct header's /* 0xNN */ offset comments with the game's compiler.

Each field line carries the offset it should land at. This writes a C file
that asserts every one of them (a negative array size when wrong) plus each
`/* size 0xNN */`, compiles it with SN64 cc1 in the build container (MIPS
o32: 4-byte pointers, natural alignment), and reports the fields whose
comment and real offset disagree.

Usage:
    python3 tools/check_structs.py structs/player.h [structs/game_object.h ...]
"""

import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
FIELD = re.compile(r"/\*\s*0x([0-9A-Fa-f]+)\s*\*/\s*(?:struct\s+)?[\w]+\s*\**\s*(\w+)\s*(?:\[[^\]]*\])*\s*;")


def asserts(header):
    out, cur, fields = [], None, []
    for line in (ROOT / "include" / header).read_text().splitlines():
        if line.startswith("typedef struct"):
            cur, fields = True, []
            continue
        m = re.match(r"^\}\s*(\w+)\s*;(.*)$", line)
        if m and cur:
            name = m.group(1)
            for off, field in fields:
                out.append(f"char chk_{name}_{field}[(OFFSETOF({name}, {field}) == 0x{off:X}) ? 1 : -1];")
            size = re.search(r"size 0x([0-9A-Fa-f]+)", m.group(2))
            if size:
                out.append(f"char chk_{name}__size[(sizeof({name}) == 0x{size.group(1)}) ? 1 : -1];")
            cur = None
            continue
        m = FIELD.search(line)
        if m and cur:
            fields.append((int(m.group(1), 16), m.group(2)))
    return out


def main():
    headers = sys.argv[1:]
    if not headers:
        print(__doc__)
        return 1
    lines = ['#include "common.h"'] + [f'#include "{h}"' for h in headers]
    lines.append("#define OFFSETOF(t, f) ((unsigned int) &((t *) 0)->f)")
    checks = [c for h in headers for c in asserts(h)]
    lines += checks
    work = ROOT / "build" / "check_structs.c"
    work.parent.mkdir(exist_ok=True)
    work.write_text("\n".join(lines) + "\n")
    script = ("cpp -P -undef -lang-c -D__GNUC__=2 -I/work/include -I/work/us/include -nostdinc "
              "build/check_structs.c > build/check_structs.i && "
              "tools/sn64/cc1 -quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mhard-float -mfp64 "
              "build/check_structs.i -o /dev/null")
    res = subprocess.run(["docker", "run", "--platform=linux/amd64", "--ulimit", "core=0", "--rm",
                          "-v", f"{ROOT}:/work", "-w", "/work", "turok2-build", "bash", "-c", script],
                         capture_output=True, text=True)
    bad = sorted(set(re.findall(r"`?chk_(\w+)'", res.stderr)))
    other = [l for l in res.stderr.splitlines() if "chk_" not in l and "In function" not in l and l.strip()]
    if other:
        print("\n".join(other[:20]))
    print(f"{len(checks)} offsets and sizes checked; {len(bad)} wrong")
    for b in bad:
        print(f"  wrong: {b}")
    return 1 if bad or other else 0


if __name__ == "__main__":
    sys.exit(main())
