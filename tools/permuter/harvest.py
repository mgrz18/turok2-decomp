#!/usr/bin/env python3
"""Collect the permuter's matches as clean C files.

A permuter candidate (build/permuter/<func>/output-0-*/source.c) is the
preprocessed draft: common.h and every macro expanded. Only the function
definition is taken from it and put back into the draft it started from
(draft.c, still with its includes and externs), so the result reads like the
rest of src/. Writes build/permuter_hits/<func>.c; check each with
tools/match_func.py and wire it in with tools/accept_manual.py.

Usage:
    tools/permuter/harvest.py
"""

import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent.parent
OUT = ROOT / "build" / "permuter_hits"


def definition(text, func):
    """(start, end) of func's definition: its header line to the closing brace at column 0."""
    m = re.search(rf"^[A-Za-z_][^;{{}}\n]*\b{func}\s*\([^;]*?\)\s*\n?\{{", text, re.M)
    if not m:
        return None
    end = re.compile(r"^\}", re.M).search(text, m.end())
    return (m.start(), end.end()) if end else None


def main():
    OUT.mkdir(parents=True, exist_ok=True)
    found = 0
    for d in sorted((ROOT / "build" / "permuter").glob("func_*")):
        hits = sorted(d.glob("output-0-*/source.c"))
        draft = d / "draft.c"
        if not hits or not draft.exists():
            continue
        func = d.name
        src, base = hits[0].read_text(), draft.read_text()
        a, b = definition(src, func), definition(base, func)
        if not a or not b:
            print(f"  {func}: could not find the definition to splice; see {hits[0]}")
            continue
        (OUT / f"{func}.c").write_text(base[:b[0]] + src[a[0]:a[1]] + base[b[1]:])
        found += 1
        print(f"  {func}")
    print(f"{found} matches in {OUT.relative_to(ROOT)}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
