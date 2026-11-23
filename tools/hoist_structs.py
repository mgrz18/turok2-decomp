#!/usr/bin/env python3
"""Replace the local copies of struct typedefs with a shared header.

For each C file given, every top-level `typedef struct/union ... { } Name;`
whose Name the header defines (or maps to one, with OLD=NEW) is deleted,
together with its forward declarations (`typedef struct Name Name;`,
`struct Name;`); OLD names are renamed to NEW in the rest of the file (word
boundaries, `struct OLD` tags too); and `#include "HEADER"` is added after the
last #include above the first deleted typedef (or in its place when there is
none), unless the file already includes it. Bodies are
not compared: check the result with tools/asm_diff.py (a field the header
names differently fails to compile; a different layout changes the code).

Usage:
    python3 tools/hoist_structs.py structs/foo.h [OLD=NEW ...] -- src/us/code/a.c ...
"""
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
HEAD = re.compile(r"^typedef\s+(?:struct|union)\b[^;{]*\{", re.M)


def spans(src):
    """(name, start, end) of each top-level typedef struct/union, end after its newline."""
    for m in HEAD.finditer(src):
        i, depth = m.end(), 1
        while i < len(src) and depth:
            depth += {"{": 1, "}": -1}.get(src[i], 0)
            i += 1
        n = re.match(r"\s*(?:__attribute__\(\(.*?\)\)\s*)?(\w+)[^;]*;[^\n]*\n?", src[i:i + 300])
        if n:
            yield n.group(1), m.start(), i + n.end()


def header_names(header):
    src = (ROOT / "include" / header).read_text()
    names = {n for n, _, _ in spans(src)}
    names |= set(re.findall(r"^typedef\s+(?:struct|union)\s+\w+\s+(\w+)\s*;", src, re.M))
    names |= set(re.findall(r"^(?:struct|union)\s+(\w+)\s*\{", src, re.M))
    return names


def hoist(path, header, names, renames):
    src = path.read_text()
    local = {old: new for old, new in renames.items()}
    for n in names:
        local.setdefault(n, n)
    cut = [(s, e) for n, s, e in spans(src) if n in local]
    if not cut:
        return False
    first = cut[0][0]
    inc = f'#include "{header}"\n'
    # after the last #include above the first copy, else where the copy was
    incs = [m.end() for m in re.finditer(r"^#include[^\n]*\n", src[:first], re.M)]
    at = incs[-1] if incs else first
    out, pos = [], 0
    if inc not in src and at < first:
        out.append(src[:at] + inc)
        pos = at
    for s, e in cut:
        out.append(src[pos:s])
        if s == first and inc not in src and at == first:
            out.append(inc)
        pos = e
    out.append(src[pos:])
    src = "".join(out)
    for old in local:
        src = re.sub(rf"^typedef\s+(?:struct|union)\s+{old}\s+{old}\s*;[^\n]*\n", "", src, flags=re.M)
        src = re.sub(rf"^(?:struct|union)\s+{old}\s*;[^\n]*\n", "", src, flags=re.M)
    for old, new in renames.items():
        src = re.sub(rf"\b{old}\b", new, src)
    path.write_text(src)
    return True


def main():
    argv = sys.argv[1:]
    if "--" not in argv or not argv:
        sys.exit(__doc__)
    i = argv.index("--")
    header, maps, files = argv[0], argv[1:i], argv[i + 1:]
    renames = dict(m.split("=", 1) for m in maps)
    names = header_names(header)
    for old, new in renames.items():
        if new not in names:
            sys.exit(f"{header} does not define {new}")
    for f in files:
        p = Path(f)
        if hoist(p, header, names, renames):
            print("hoisted", p.name)


if __name__ == "__main__":
    main()
