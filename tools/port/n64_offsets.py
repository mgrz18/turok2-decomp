#!/usr/bin/env python3
"""Count the hard-coded N64 field offsets left in the game's C (docs/port/STATUS.md, round 4 s5).

A site is one of:
  m2c    M2C_FIELD(x, T *, 0xNN)
  cast   ((s8 *) x) + 0xNN / ((u8 *) x) + 0xNN / ((char *) x) + 0xNN / ((void *) x) + 0xNN
  deref  *(T *)(x + 0xNN) / *(T *) x + 0xNN)
  macro  P_S32(p, 0xNN) and the other P_xx(p, off) view macros
Each works on the N64 and reads the wrong bytes on a host whose struct has pointers before
the offset. Usage: n64_offsets.py [--git REV] [--top N] [--files] [paths...]
(default: src/us/code; with --git, the files as they are at REV).
"""
import argparse
import re
import subprocess
import sys
from collections import Counter
from pathlib import Path

PATTERNS = {
    "m2c": re.compile(r"M2C_FIELD\("),
    "cast": re.compile(r"\(\((?:s8|u8|char|void) *\*\) *[a-zA-Z0-9_>.-]+\) *\+ *0x[0-9A-Fa-f]+"),
    "deref": re.compile(r"\*\([a-zA-Z0-9_ ]+\*+\) *\(?[a-zA-Z_][a-zA-Z0-9_>.-]* *\+ *0x[0-9A-Fa-f]+\)"),
    "macro": re.compile(r"\bP_[A-Z0-9]+\([^,()]+, *0x[0-9A-Fa-f]+\)"),
}


def count(text):
    c = Counter()
    for line in text.splitlines():
        s = line.lstrip()
        if s.startswith(("/*", "*", "//", "#define")):
            continue
        for k, p in PATTERNS.items():
            c[k] += len(p.findall(line))
    return c


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("paths", nargs="*", default=["src/us/code"])
    ap.add_argument("--git", help="count the files at this revision")
    ap.add_argument("--top", type=int, default=0, help="list the N files with most sites")
    args = ap.parse_args()

    per_file = {}
    if args.git:
        names = subprocess.run(["git", "ls-tree", "-r", "--name-only", args.git, "--", *args.paths],
                               capture_output=True, text=True, check=True).stdout.split()
        for n in names:
            if n.endswith(".c"):
                text = subprocess.run(["git", "show", f"{args.git}:{n}"], capture_output=True,
                                      text=True, errors="replace").stdout
                per_file[n] = count(text)
    else:
        for p in args.paths:
            for f in sorted(Path(p).rglob("*.c")) if Path(p).is_dir() else [Path(p)]:
                per_file[str(f)] = count(f.read_text(errors="replace"))
    total = Counter()
    for c in per_file.values():
        total.update(c)
    files = sum(1 for c in per_file.values() if sum(c.values()))
    print(f"{sum(total.values())} sites in {files} files: " +
          ", ".join(f"{k} {total[k]}" for k in PATTERNS))
    if args.top:
        for f, c in sorted(per_file.items(), key=lambda kv: -sum(kv[1].values()))[:args.top]:
            if sum(c.values()):
                print(f"  {sum(c.values()):4d} {f}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
