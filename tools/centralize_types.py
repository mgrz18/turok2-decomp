#!/usr/bin/env python3
"""Move the types the imported C files each carry into one shared header.

tools/import_ref.py makes every imported function a self-contained file: the
types it uses (structs, unions, enums, typedefs) are copied into it, cut down
to what that one function needs. Over hundreds of files the same struct is
written out hundreds of times. This collects them all into
include/rw/<source>_types.h (one per source decomp), in an order where each type comes after the ones it
needs, and has each file include that instead.

A type whose definition differs between files (a field typed OSPri in one
and s32 in another) cannot have one shared definition; the files that use a
definition other than the header's keep their own copy and do not include
the header. Nothing changes in the code: `make verify` is the check.

Usage:
    tools/centralize_types.py [--write]
"""

import argparse
import re
import sys
from collections import Counter, defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
import import_ref as I  # noqa: E402

SOURCES = {"AbuN64/ragewars-decomp": "abu", "thejunkman9000/Turok-Rage-Wars-Decomp": "junk",
           "decompals/ultralib": "ultralib"}


def is_type(item):
    h = item.lstrip()
    return h.startswith("typedef") or re.match(r"^(struct|union|enum)\b[^;=(]*(\{|;\s*$)", h, re.S)


def norm(item):
    return re.sub(r"\s+", " ", item.strip())


def keys(item):
    """What an item defines, as (kind, name): a struct/union/enum tag with a
    body, a typedef name, or an enum constant. Two items with the same key
    and different text are a real clash."""
    out = {("tag", n) for n in re.findall(r"\b(?:struct|union|enum)\s+(\w+)\s*\{", item)}
    if item.lstrip().startswith("typedef"):
        out |= {("typedef", n) for n in I.declared(item) if ("tag", n) not in out
                or re.search(rf"\}}\s*{n}\s*;\s*$", item)}
    elif re.search(r"\benum\b", item):
        out |= {("const", n) for n in I.declared(item) if ("tag", n) not in out}
    return out


def split_file(text):
    """(header comment and cc1-flags lines, top-level items)."""
    m = re.match(r"^(/\*.*?\*/\n(?:/\* cc1-flags:.*?\*/\n)?)", text, re.S)
    head = m.group(1) if m else ""
    return head, I.top_level(text[len(head):])


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--write", action="store_true")
    args = ap.parse_args()
    groups = defaultdict(dict)
    for p in sorted((ROOT / "src").rglob("*.c")):
        t = p.read_text(errors="replace")
        if t.startswith("/* From ") and "tools/import_ref.py" in t[:400] and '#include "rw/' not in t:
            tag = next((v for k, v in SOURCES.items() if t.startswith(f"/* From {k}")), None)
            if tag:
                groups[tag][p] = split_file(t)
    for tag, files in sorted(groups.items()):
        one(tag, files, args.write)
    return 0


def one(tag, files, write):
    """One source decomp's files: its types are consistent among themselves."""
    HEADER = ROOT / "include" / "rw" / f"{tag}_types.h"
    INCLUDE = f'#include "rw/{tag}_types.h"'
    guard = f"RW_{tag.upper()}_TYPES_H"
    # Variants of each name, and how many files use each.
    variants = defaultdict(Counter)
    for head, items in files.values():
        for it in items:
            if is_type(it):
                for k in keys(it):
                    variants[k][norm(it)] += 1
    canon = {n: c.most_common(1)[0][0] for n, c in variants.items()}
    clash = {n for n, c in variants.items() if len(c) > 1}
    # A file joins when every full definition it carries is the canonical one.
    joins, keeps = {}, []
    for p, (head, items) in files.items():
        ok = all(norm(it) == canon[k] for it in items if is_type(it) for k in keys(it))
        (joins.__setitem__(p, (head, items)) if ok else keeps.append(p))
    # The header: every type item of the joining files, deduplicated.
    pool, order = {}, []
    for head, items in joins.values():
        for it in items:
            if is_type(it):
                k = norm(it)
                if k not in pool:
                    pool[k] = it
                    order.append(k)
    # Each file lists its types in an order that compiles, so first-seen order
    # over the files does too: when an item first appears, everything it
    # needs came before it in that file and is already out.
    out = [pool[k] for k in order]
    # The basic types are common.h's own, word for word: let a file that has
    # common.h already (an m2c draft) include this header too.
    basic = {"s8", "u8", "s16", "u16", "s32", "u32", "s64", "u64", "f32", "f64"}
    out = [f"#ifndef COMMON_H\n{it}\n#endif" if re.match(r"^\s*typedef\s+[\w\s]+\b(\w+)\s*;\s*$", it)
           and re.match(r"^\s*typedef\s+[\w\s]+\b(\w+)\s*;\s*$", it).group(1) in basic else it for it in out]
    src = next(k for k, v in SOURCES.items() if v == tag)
    text = (f"/* Types of the functions taken from {src} (tools/import_ref.py),\n"
            " * gathered in one place by tools/centralize_types.py. Their names are that\n"
            " * decomp's own. */\n\n"
            f"#ifndef {guard}\n#define {guard}\n\n" + "\n".join(out) + "\n\n#endif\n")
    print(f"{tag}: {len(files)} imported files: {len(joins)} share {HEADER.relative_to(ROOT)} "
          f"({len(out)} items), {len(keeps)} keep their own types ({len(clash)} names defined two ways)")
    if not write:
        return
    HEADER.parent.mkdir(parents=True, exist_ok=True)
    HEADER.write_text(text)
    for p, (head, items) in joins.items():
        rest = [it for it in items if not is_type(it)]
        p.write_text(head + "\n" + INCLUDE + "\n\n" + "\n".join(rest) + "\n")


if __name__ == "__main__":
    sys.exit(main())
