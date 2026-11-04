#!/usr/bin/env python3
"""Count the local struct views in src/us/code, for docs/STRUCTS_TODO.md.

Every top-level `typedef struct/union ... { ... } Name;` of a C file is a
local view of some object. Grouped by name, the counts say which objects are
still described file by file instead of in include/structs/. Prints two
markdown tables: globals that 3 or more files declare with a struct type of
their own, and every local struct name defined in 2 or more files.

Usage:
    python3 tools/struct_census.py [src/us/code] [--top N]
"""

import argparse
import collections
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
HEAD = re.compile(r"^typedef\s+(?:struct|union)\b[^;{]*\{", re.M)
FIELD = re.compile(r"/\*\s*(0x[0-9A-Fa-f]+)\s*\*/\s*(?:(?:struct|union|const|unsigned|signed)\s+)*\w+\s*(\**)\s*(\w+)\s*(\[[^\]]*\])?")
PAD = re.compile(r"^(?:_?pad|unk_?pad|padding|_)", re.I)


def typedefs(src):
    """(name, body) of each top-level typedef struct/union in a file."""
    for m in HEAD.finditer(src):
        i, depth = m.end(), 1
        while i < len(src) and depth:
            depth += {"{": 1, "}": -1}.get(src[i], 0)
            i += 1
        n = re.match(r"\s*(?:__attribute__\(\(.*?\)\)\s*)?(\w+)", src[i:i + 200])
        if n:
            yield n.group(1), src[m.end():i - 1]


def fields(body):
    named = [f"{off} {star}{name}{arr or ''}" for off, star, name, arr in FIELD.findall(body)
             if not PAD.match(name)]
    if named:
        s = ", ".join(named[:8])
        return s + (", ..." if len(named) > 8 else "")
    flat = " ".join(re.sub(r"/\*.*?\*/", "", body, flags=re.S).split())
    return "`" + (flat[:70] + "..." if len(flat) > 70 else flat) + "`"


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("dir", nargs="?", default=str(ROOT / "src" / "us" / "code"))
    ap.add_argument("--top", type=int, default=40)
    args = ap.parse_args()

    files = collections.defaultdict(list)     # name -> [(file, body)]
    globals_ = collections.defaultdict(lambda: collections.defaultdict(set))
    for path in sorted(Path(args.dir).glob("*.c")):
        src = path.read_text(errors="replace")
        local = {}
        for name, body in typedefs(src):
            if name not in local:
                local[name] = body
                files[name].append((path.stem, body))
        for t, g in re.findall(r"^extern\s+(?:struct\s+)?(\w+)\s*\**\s*(D_\w+)\b", src, re.M):
            if t in local:
                globals_[g][t].add(path.stem)

    total = len(files)
    multi = {n: v for n, v in files.items() if len(v) >= 2}
    rows = sorted(globals_.items(), key=lambda kv: -len(set().union(*kv[1].values())))
    rows = [(g, t) for g, t in rows if len(set().union(*t.values())) >= 3]
    print(f"## Globals declared under several local types\n")
    print(f"Each row is a global that 3 or more files declare with a struct type of")
    print(f"their own (top {min(args.top, len(rows))} of {len(rows)}).\n")
    print("| Global | Files | Local types (files) |\n|---|---|---|")
    for g, types in rows[:args.top]:
        n = len(set().union(*types.values()))
        ts = sorted(types.items(), key=lambda kv: -len(kv[1]))
        desc = ", ".join(f"{t} ({len(f)})" for t, f in ts[:8]) + (", ..." if len(ts) > 8 else "")
        print(f"| {g} | {n} | {desc} |")
    print(f"\n## Every local struct name defined in 2 or more files\n")
    print(f"{len(multi)} of the {total:,} local typedef names. \"Fields\" are the named offsets of the")
    print("most common layout (pads left out); a body without offset comments is shown")
    print("as written.\n")
    print("| Name | Files | Layouts | Fields | Files |\n|---|---|---|---|---|")
    for name, v in sorted(multi.items(), key=lambda kv: (-len(kv[1]), kv[0])):
        layouts = collections.Counter(" ".join(b.split()) for _, b in v)
        common = layouts.most_common(1)[0][0]
        body = next(b for _, b in v if " ".join(b.split()) == common)
        names = [f for f, _ in v]
        shown = ", ".join(names[:4]) + (f" +{len(names) - 4}" if len(names) > 4 else "")
        print(f"| {name} | {len(v)} | {len(layouts)} | {fields(body)} | {shown} |")


if __name__ == "__main__":
    main()
