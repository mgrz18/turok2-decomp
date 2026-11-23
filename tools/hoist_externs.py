#!/usr/bin/env python3
"""Drop the local `extern` declarations of globals a header declares, and include the header.

For every C file under src/ (or the files given) that declares one of HEADER's
globals itself, the local declaration lines go and `#include "globals/<h>"` is
added after the file's first #include. Lines that carry a `local` comment
(`/* local type: ... */`, `/* local: ... */`) stay, and so do the lines of the
files given with --keep. A line that declares several names goes only when the
header declares all of them.

The header's type replaces the local one, so run tools/asm_diff.py afterwards;
a file whose code changes is reverted and listed with --keep (its view then
gets a `local type` comment), and the run repeated.

Usage:
    python3 tools/hoist_externs.py include/globals/menu_text.h [--keep F ...] [FILE ...]
"""
import argparse
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
EXT = re.compile(r"^\s*extern\b([^;(]*);\s*(/\*.*\*/|//.*)?\s*$")
NAMES = re.compile(r"([A-Za-z_]\w*)\s*(?:\[[^\]]*\]\s*)*(?=,|$)")
INCLUDE = re.compile(r'^\s*#\s*include\s+"[^"]+"')


def header_names(path):
    names = set()
    for line in path.read_text().splitlines():
        m = EXT.match(line)
        if m:
            names.update(NAMES.findall(m.group(1).strip()))
    return names


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("header")
    ap.add_argument("files", nargs="*")
    ap.add_argument("--keep", nargs="*", default=[])
    args = ap.parse_args()
    hpath = (ROOT / args.header).resolve()
    names = header_names(hpath)
    inc = '#include "%s"' % hpath.relative_to(ROOT / "include")
    keep = {str((ROOT / k).resolve()) for k in args.keep}
    files = [ROOT / f for f in args.files] or sorted((ROOT / "src").rglob("*.c"))
    changed = dropped = 0
    for f in files:
        if str(f.resolve()) in keep:
            continue
        text = f.read_text()
        out, n = [], 0
        for line in text.split("\n"):
            m = EXT.match(line)
            if m and not (m.group(2) and "local" in m.group(2)):
                decl = m.group(1).strip()
                ns = NAMES.findall(decl)
                if ns and all(x in names for x in ns):
                    n += 1
                    continue
            out.append(line)
        if not n:
            continue
        if inc not in out:
            # after the first #include (never inside a later #ifdef block)
            first = next((i for i, l in enumerate(out) if INCLUDE.match(l)), -1)
            out.insert(first + 1, inc)
        f.write_text("\n".join(out))
        changed += 1
        dropped += n
    print(f"{hpath.name}: {dropped} local declarations dropped in {changed} files")


if __name__ == "__main__":
    main()
