#!/usr/bin/env python3
"""Gather the typed prototypes the imported functions carry, into a header.

Every function taken from AbuN64's Rage Wars decomp (tools/import_ref.py)
declares what it calls with that decomp's real types: `void
func_002A0B10(struct ALVoice_s *voice, s32 vol)` where our own prototypes
(include/functions.h, inferred from the asm) say `s32 func_002A0B10(s32,
s32)`. Collected, they type the calls in m2c's drafts of the functions still
pending, and the pending function's own parameters when it is one of them
(tools/auto_match.py --rw-protos).

Writes include/rw/abu_protos.h: for each Turok 2 function, the declaration
most of the files agree on.

Usage:
    tools/rw_protos.py
"""

import re
import sys
from collections import Counter, defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
import import_ref as I  # noqa: E402

OUT = ROOT / "include" / "rw" / "abu_protos.h"


def main():
    named = set(re.findall(r"^(\w+)\s*=\s*0x[0-9A-Fa-f]+;", (ROOT / "versions" / "names.us.txt").read_text(), re.M))
    seen = defaultdict(Counter)
    for p in sorted((ROOT / "src").rglob("*.c")):
        t = p.read_text(errors="replace")
        if '#include "rw/abu_types.h"' not in t:
            continue
        for it in I.top_level(t.split('#include "rw/abu_types.h"', 1)[1]):
            h = it.strip()
            if not h.endswith(";") or "(" not in h or "{" in h:
                continue
            name = I.func_name(h.rstrip(";").strip())
            if name and (re.match(r"^func_[0-9A-F]{8}$", name) or name in named):
                seen[name][re.sub(r"\s+", " ", re.sub(r"^\s*(extern|static)\s+", "", h))] += 1
    lines = [c.most_common(1)[0][0] for _, c in sorted(seen.items())]
    OUT.write_text("/* Prototypes of Turok 2 functions as AbuN64's Rage Wars decomp types them,\n"
                   " * taken from the functions imported from it (tools/rw_protos.py). */\n\n"
                   "#ifndef RW_ABU_PROTOS_H\n#define RW_ABU_PROTOS_H\n\n"
                   '#include "rw/abu_types.h"\n\n' + "\n".join(lines) + "\n\n#endif\n")
    print(f"{len(lines)} prototypes ({sum(len(c) > 1 for c in seen.values())} with more than one form)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
