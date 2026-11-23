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
import os
import re
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
YAML = ROOT / "versions" / "turok2.us.yaml"
SHARED = ROOT / "versions" / "shared_rodata.us.txt"
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


CPP = ["cpp", "-P", "-undef", "-lang-c", "-D_LANGUAGE_C", "-DF3DEX_GBI_2", "-D__GNUC__=2", "-DGAME_VERSION=\"us\"",
       "-I.", "-Iinclude", "-Ius/include", "-nostdinc"]
SIMPLE_TYPEDEF = re.compile(r"typedef\s+([\w\s\*]+?)\s*\b(\w+)\s*;")


def canon_type(t):
    """`signed int` and `int` are one type; so are `short int` and `short`, `unsigned` and `unsigned int`."""
    t = re.sub(r"\s+", " ", t).strip()
    t = re.sub(r"\bsigned (int|short|long)\b", r"\1", t)
    t = re.sub(r"\b(short|long) int\b", r"\1", t)
    t = re.sub(r"\bunsigned$", "unsigned int", t)
    return t.replace(" *", "*")


def header_typedefs(includes, src_dir):
    """name -> canonical type of every simple typedef the module's #includes give (one cpp run)."""
    if not includes:
        return {}
    with tempfile.NamedTemporaryFile("w", suffix=".c", dir=src_dir, delete=False) as f:
        f.write("".join(includes))
    try:
        r = subprocess.run(CPP + [f.name], cwd=ROOT, capture_output=True, text=True)
    finally:
        os.unlink(f.name)
    return {m.group(2): canon_type(m.group(1)) for m in SIMPLE_TYPEDEF.finditer(r.stdout)}


_INC = {}


def include_types(item, src_dir):
    """Type names (typedef names, struct tags) an #include line brings (cached; one cpp run each)."""
    key = re.sub(r"\s+", " ", strip_comments(item)).strip()
    if key not in _INC:
        with tempfile.NamedTemporaryFile("w", suffix=".c", dir=src_dir, delete=False) as f:
            f.write(item if item.endswith("\n") else item + "\n")
        try:
            t = subprocess.run(CPP + [f.name], cwd=ROOT, capture_output=True, text=True).stdout
        finally:
            os.unlink(f.name)
        _INC[key] = ({m.group(2) for m in SIMPLE_TYPEDEF.finditer(t)} | set(re.findall(r"}\s*(\w+)\s*;", t))
                     | set(re.findall(r"\b(?:struct|union|enum)\s+(\w+)", t)))
    return _INC[key]


def static_name(code, kind, name):
    """The name a `static` item defines (a function or a variable), else None."""
    c = strip_comments(code).strip()
    if not re.match(r"static\b", c):
        return None
    if kind == "func":
        return name
    if kind == "decl" and "(" not in c.split("=")[0]:
        m = re.match(r"static\b[^=;\[{]*?\b(\w+)\s*(\[[^\]]*\]\s*)*(=|;)", c, re.S)
        return m.group(1) if m else None
    return None


def rename_statics(parsed, stems):
    """A static defined by more than one file is a separate object in each original object file: keep
    them all, under the first file's name and `<name>_<vram>` in the others (the name the readable files
    already use for such helpers), unless the definitions are the same text and inline (kept once)."""
    defs = {}
    for i, its in enumerate(parsed):
        for code, kind, name in its:
            n = static_name(code, kind, name)
            if n:
                key = re.sub(r"\s+", " ", strip_comments(code)).strip()
                defs.setdefault(n, []).append((i, key))
    taken = {n for its in parsed for _, _, n in its if n}
    public = {n for its in parsed for c, k, n in its if k == "func" and not static_name(c, k, n)}
    renames = {}
    for n, ds in defs.items():
        files = sorted({i for i, _ in ds})
        if len(files) < 2 and n not in public:
            continue
        first_key = ds[0][1]
        # a static copy of a function the module also defines (Demo_GetPlayerCount): every copy renamed
        for i in (files if n in public else files[1:]):
            keys = [k for j, k in ds if j == i]
            if n not in public and all(k == first_key for k in keys) and re.search(r"\b(__inline__|inline|INLINE)\b", first_key):
                continue
            m = re.match(r"func_0*([0-9A-Fa-f]+)$", stems[i])
            new = f"{n}_{m.group(1)}" if m else f"{n}_{stems[i]}"
            while new in taken:
                new += "_"
            taken.add(new)
            renames.setdefault(i, {})[n] = new
    out = []
    for i, its in enumerate(parsed):
        rn = renames.get(i)
        if not rn:
            out.append(its)
            continue
        pat = re.compile(r"\b(" + "|".join(map(re.escape, rn)) + r")\b")
        out.append([(pat.sub(lambda m: rn[m.group(1)], c), k, rn.get(n, n) if n else n) for c, k, n in its])
    return out, renames


def type_names(code):
    """Type names an item declares (typedef names, struct/union/enum tags)."""
    c = strip_comments(code)
    names = set(re.findall(r"\b(?:struct|union|enum)\s+(\w+)\s*\{", c))
    if re.match(r"\s*typedef\b", c):
        m = re.search(r"\b(\w+)\s*(\[[^\]]*\]\s*)*;\s*$", c)
        if m:
            names.add(m.group(1))
        m = re.search(r"\(\s*\*\s*(\w+)\s*\)", c)
        if m:
            names.add(m.group(1))
    elif c.lstrip().startswith("#"):
        names |= {m.group(2) for m in SIMPLE_TYPEDEF.finditer(c)}
    return names


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
    ap.add_argument("--dry-run", action="store_true", help="print the merged file, change nothing")
    ap.add_argument("--no-fix", action="store_true", help="round 23's merge: no static renames, no include hoisting, "
                    "no typedef dedupe against the headers, no deferred prototypes")
    a = ap.parse_args()
    d = (ROOT / a.dir).resolve()
    rel_dir = d.relative_to(ROOT / "src" / "us")
    # file names are unique across src/ (tools/srcfiles.py, shared_rodata.py): vm2 holds the stale second copy of
    # vm1's modules under the same directory names, so its merged file takes vm2's function suffix
    name = a.name or (d.name + "_2" if rel_dir.parts[0] == "vm2" else d.name)
    clash = [p for p in (ROOT / "src").rglob(f"{name}.c") if p.parent != d]
    if clash:
        sys.exit(f"{a.dir}: {name}.c exists already ({clash[0].relative_to(ROOT)}): pass --name")
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
    # a second-VM-module file whose .rodata is placed over the first module's pool (tools/shared_rodata.py) owns
    # that placement by its file name: one such file per module can hand it to the merged file, two cannot
    # (one object has one .rodata section, placed at one address)
    shared = [l.split()[0] for l in SHARED.read_text().splitlines() if l.split("#")[0].strip()
              and l.split()[0] in files]
    if len(shared) > 1 or (shared and ro):
        sys.exit(f"{a.dir}: {len(shared)} files place their .rodata over the first VM module's pool one by one "
                 f"(versions/shared_rodata.us.txt){' and the yaml places others' if ro else ''}")
    texts = [files[s].read_text() for s in text_order]
    flags = {tuple(FLAGS.findall(t)) for t in texts}
    if len(flags) > 1:
        sys.exit(f"{a.dir}: the files do not share their cc1 flags")
    parsed = [items(FLAGS.sub("", t)) for t in texts]
    renames, hdr, hoisted = {}, {}, []
    if not a.no_fix:
        parsed, renames = rename_statics(parsed, text_order)
        # every file's leading #includes go first (a file before them that typedefs s32 itself would
        # otherwise meet common.h's after its own); a typedef the headers already give the same way is dropped
        for its in parsed:
            for it, kind, n in its:
                c = strip_comments(it).strip()
                if kind != "cpp" or re.match(r"#\s*define", c):
                    break
                if re.match(r"#\s*include\b", c) and it not in hoisted:
                    hoisted.append(it)
        hdr = header_typedefs(hoisted, d)
        local = [m for its in parsed for it, kind, n in its if kind == "decl"
                 for m in [SIMPLE_TYPEDEF.fullmatch(re.sub(r"\s+", " ", strip_comments(it)).strip())] if m]
        if not any(hdr.get(m.group(2)) == canon_type(m.group(1)) for m in local):
            hoisted, hdr = [], {}   # nothing to drop: the files keep their own include order
    defined = {}
    for its in parsed:
        for it, kind, n in its:
            if kind == "func":
                defined.setdefault(n, signature(it))
    local_types, type_item = set(), {}
    is_inc = lambda it: re.match(r"#\s*include\b", strip_comments(it).strip())
    for i, its in enumerate(parsed):
        for it, kind, n in its:
            if kind in ("decl", "cpp"):
                names = type_names(it)
                if not a.no_fix and i and is_inc(it):
                    names = include_types(it, d)   # a later file's header: a hoisted prototype may need it
                for t in names:
                    local_types.add(t)
                    type_item.setdefault(t, it)
    out, seen, done, last, have, pending = [], set(), set(), None, set(hdr), []
    if flags and list(flags)[0]:
        out.append(list(flags)[0][0])
    for it in hoisted:
        out.append(it)
        seen.add(re.sub(r"\s+", " ", strip_comments(it)).strip())
        last = "cpp"
    have |= {t for it in hoisted for t in include_types(it, d)}
    if not a.no_fix:   # the first file's own includes
        for it, kind, n in parsed[0]:
            if kind == "cpp" and is_inc(it):
                have |= include_types(it, d)
    dropped = []

    def needs(sig):
        return {w for w in re.findall(r"\b\w+\b", sig) if w in local_types and w not in have}

    def flush(force=False):
        nonlocal last, have
        for p in list(pending):
            if force:
                # a caller comes first: the type declarations its signature needs move up to it
                for t in sorted(needs(defined[p])):
                    if t in have:
                        continue
                    ti = type_item[t]
                    out.append(ti)
                    seen.add(re.sub(r"\s+", " ", strip_comments(ti)).strip())
                    have |= include_types(ti, d) if is_inc(ti) else type_names(ti)
            if force or not needs(defined[p]):
                pending.remove(p)
                if p not in done:
                    out.append(defined[p])
                    last = "proto"
    for its in parsed:
        for it, kind, n in its:
            if kind == "proto" and n in defined:
                if n in done or n in seen:
                    continue
                seen.add(n)
                if last == "cpp" and not out[-1].endswith("\n\n"):
                    out.append("\n")
                if not a.no_fix and needs(defined[n]):
                    pending.append(n)   # its signature names a type a later file declares: wait for it
                    continue
                out.append(defined[n])
                last = "proto"
                continue
            if kind == "func":
                key = re.sub(r"\s+", " ", strip_comments(it)).strip()
                if key in seen and static_name(it, kind, n):
                    continue   # the same static inline helper again (rename_statics kept the name)
                seen.add(key)
                if pending:
                    flush(force=any(re.search(r"\b%s\b" % p, strip_comments(it)) for p in pending))
                done.add(n)
                out.append("\n" + it if out and not out[-1].endswith("\n\n") else it)
                last = "func"
                continue
            key = re.sub(r"\s+", " ", strip_comments(it)).strip()
            if key in seen and ("{" not in key or re.match(r"(typedef|struct|union|enum)\b", key)):
                continue
            m = SIMPLE_TYPEDEF.fullmatch(key) if hdr else None
            if m and hdr.get(m.group(2)) == canon_type(m.group(1)):
                dropped.append(m.group(2))
                continue
            seen.add(key)
            if last in ("cpp", "func") and kind != last and not out[-1].endswith("\n\n"):
                out.append("\n")
            out.append(it)
            last = kind
            if kind in ("decl", "cpp"):
                have |= type_names(it)
                if not a.no_fix and is_inc(it):
                    have |= include_types(it, d)
                if pending:
                    flush()
    flush(force=True)
    merged = "".join(out)
    target = d / f"{name}.c"
    if renames or dropped:
        print(f"{a.dir}: statics renamed {sorted((n, v) for r in renames.values() for n, v in r.items())}, "
              f"typedefs the headers give dropped {sorted(set(dropped))}", file=sys.stderr)
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
    if shared:
        SHARED.write_text(re.sub(rf"^{shared[0]}\b", name, SHARED.read_text(), flags=re.M))
    subprocess.run(["git", "rm", "-q", *[str(files[s].relative_to(ROOT)) for s in text_order]], check=True, cwd=ROOT)
    subprocess.run(["git", "add", str(target.relative_to(ROOT))], check=True, cwd=ROOT)
    print(f"{target.relative_to(ROOT)}: {len(files)} files, {len(ro)} .rodata subsegments")
    return 0


if __name__ == "__main__":
    sys.exit(main())
