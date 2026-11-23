#!/usr/bin/env python3
"""Merge the one-function files of a module directory into one C file (docs/MODULES.md, "Merged modules").

The matching build compiles every file as its own object, so merging is only allowed where
`make verify` stays byte-exact: one translation unit pools equal literals in its .rodata, links
the module's .rodata as one block (the yaml's `.rodata` subsegments of its files must be adjacent
and in .text order) and may type a call differently once the callee is defined in the same file.
This writes src/us/<dir>/<Module>.c from the files in .text order:

  - each file's top-level declarations (includes, typedefs, externs, prototypes, macros) before
    its functions, a declaration already given by an earlier file dropped;
  - a prototype of a function the module defines replaced by the definition's own signature
    (placed once, before the first use), or dropped after the definition;
  - the `/* cc1-flags: */` line once (every file must have the same one or none).

and rewrites versions/turok2.us.yaml: the first file's `c` and `.rodata` subsegments now name the
merged file, the others are gone. Then `make setup`, a clean build and `make verify` decide.

    python3 tools/merge_module.py src/us/code/CSun [--name CSun] [--dry-run]
"""

import argparse
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
YAML = ROOT / "versions" / "turok2.us.yaml"
FLAGS = re.compile(r"^/\* cc1-flags: .* \*/\n", re.M)
ENTRY = re.compile(r"^\s+- \[(0x[0-9A-Fa-f]+),\s*(\w+|\.rodata),\s*([^\]\s]+)\s*\]")


def strip_comments(s):
    return re.sub(r"/\*.*?\*/|//[^\n]*", "", s, flags=re.S)


def items(text):
    """Top-level items: (code, kind, name). kind: 'cpp', 'func' (a definition), 'proto', 'decl'."""
    out, cur, depth, in_cpp, cond = [], [], 0, False, 0
    for line in text.splitlines(keepends=True):
        if not cur and not line.strip():
            continue
        cur.append(line)
        code = strip_comments("".join(cur))
        d = line.lstrip()
        if depth == 0 and d.startswith("#"):
            # a conditional block (#if .. #endif) stays one item: never split, never merged
            if re.match(r"#\s*if", d):
                cond += 1
            elif re.match(r"#\s*endif", d):
                cond -= 1
            if not in_cpp and cond == 0 and not re.match(r"#\s*endif", d):
                in_cpp = True
        if cond:
            continue
        if re.match(r"#\s*endif", d) and depth == 0:
            out.append("".join(cur))
            cur = []
            continue
        if in_cpp:
            if line.rstrip().endswith("\\"):
                continue
            in_cpp = False
            out.append("".join(cur))
            cur = []
            continue
        depth = code.count("{") - code.count("}")
        s = code.strip()
        if depth == 0 and s and (s.endswith(";") or s.endswith("}")):
            out.append("".join(cur))
            cur = []
    if cur:
        out.append("".join(cur))
    res = []
    for it in out:
        c = strip_comments(it).strip()
        if c.startswith("#"):
            res.append((it, "cpp", None))
            continue
        m = re.match(r"([^{;=]*?)\b(\w+)\s*\(([^{;]*)\)\s*\{", c, re.S)
        if m and not re.match(r"(typedef|struct|union|enum)\b", c):
            res.append((it, "func", m.group(2)))
            continue
        m = re.fullmatch(r"(?:extern\s+)?[\w\s\*]+?\b(\w+)\s*\((.*)\)\s*;", c, re.S)
        if m and not c.startswith("typedef"):
            res.append((it, "proto", m.group(1)))
            continue
        res.append((it, "decl", None))
    return res


def signature(item):
    """`T name(params);` from a definition (its text up to the body's brace), comments dropped."""
    c = strip_comments(item)
    head = c[:c.index("{")].strip()
    head = re.sub(r"^\s*(INLINE\s+)?", "", head)
    return re.sub(r"\s+", " ", head) + ";\n"


def yaml_entries(stems):
    """(line index, kind, path) of the yaml subsegments that name these files."""
    lines = YAML.read_text().splitlines(keepends=True)
    out = []
    for n, line in enumerate(lines):
        m = ENTRY.match(line)
        if m and m.group(3).rsplit("/", 1)[-1] in stems:
            out.append((n, m.group(2), m.group(3)))
    return lines, out


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("dir")
    ap.add_argument("--name")
    ap.add_argument("--dry-run", action="store_true")
    a = ap.parse_args()
    d = (ROOT / a.dir).resolve()
    rel_dir = d.relative_to(ROOT / "src" / "us")
    name = a.name or d.name
    files = {p.stem: p for p in d.glob("*.c")}
    lines, ents = yaml_entries(set(files))
    text_order = [e[2].rsplit("/", 1)[-1] for e in ents if e[1] == "c"]
    if sorted(text_order) != sorted(files) or len(files) < 2:
        sys.exit(f"{a.dir}: the yaml does not link exactly these files as `c` ({len(text_order)} of {len(files)})")
    # the module's c entries must be adjacent; so must its .rodata entries, in the same order
    cs = [e for e in ents if e[1] == "c"]
    ro = [e for e in ents if e[1] == ".rodata"]
    entry_lines = [n for n, l in enumerate(lines) if ENTRY.match(l)]
    pos = {n: i for i, n in enumerate(entry_lines)}
    def adjacent(es):
        idx = [pos[e[0]] for e in es]
        return idx == list(range(idx[0], idx[0] + len(idx))) if idx else True
    if not adjacent(cs):
        sys.exit(f"{a.dir}: its .text subsegments are not adjacent")
    ro_order = [e[2].rsplit("/", 1)[-1] for e in ro]
    if not adjacent(ro) or ro_order != [s for s in text_order if s in ro_order]:
        sys.exit(f"{a.dir}: its .rodata is not one block in .text order ({ro_order})")
    texts = [files[s].read_text() for s in text_order]
    flags = {tuple(FLAGS.findall(t)) for t in texts}
    if len(flags) > 1:
        sys.exit(f"{a.dir}: the files do not share their cc1 flags")
    parsed = [items(FLAGS.sub("", t)) for t in texts]
    defined = {}
    for its in parsed:
        for it, kind, n in its:
            if kind == "func":
                defined.setdefault(n, signature(it))
    out, seen, done, last = [], set(), set(), None
    if flags and list(flags)[0]:
        out.append(list(flags)[0][0])
    for its in parsed:
        for it, kind, n in its:
            if kind == "proto" and n in defined:
                if n in done or n in seen:
                    continue
                seen.add(n)
                if last == "cpp" and not out[-1].endswith("\n\n"):
                    out.append("\n")
                out.append(defined[n])
                last = "proto"
                continue
            if kind == "func":
                done.add(n)
                out.append("\n" + it if out and not out[-1].endswith("\n\n") else it)
                last = "func"
                continue
            key = re.sub(r"\s+", " ", strip_comments(it)).strip()
            if key in seen and ("{" not in key or re.match(r"(typedef|struct|union|enum)\b", key)):
                continue
            seen.add(key)
            if last in ("cpp", "func") and kind != last and not out[-1].endswith("\n\n"):
                out.append("\n")
            out.append(it)
            last = kind
            continue
    merged = "".join(out)
    target = d / f"{name}.c"
    if a.dry_run:
        print(merged)
        return 0
    target.write_text(merged)
    first = {"c": cs[0], ".rodata": ro[0] if ro else None}
    drop = {e[0] for e in ents if e not in first.values()}
    for k, e in first.items():
        if e:
            lines[e[0]] = lines[e[0]].replace(e[2], f"{rel_dir}/{name}")
    YAML.write_text("".join(l for n, l in enumerate(lines) if n not in drop))
    subprocess.run(["git", "rm", "-q", *[str(files[s].relative_to(ROOT)) for s in text_order]], check=True, cwd=ROOT)
    subprocess.run(["git", "add", str(target.relative_to(ROOT))], check=True, cwd=ROOT)
    print(f"{target.relative_to(ROOT)}: {len(files)} files, {len(ro)} .rodata subsegments")
    return 0


if __name__ == "__main__":
    sys.exit(main())
