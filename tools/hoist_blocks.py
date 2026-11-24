#!/usr/bin/env python3
"""Hoist local types that several files define with the same text into a shared header, also
when the definition is an `#ifdef TARGET_PORT ... #else ... #endif` pair (the host view and the N64
view of one type, which tools/hoist_twins.py does not take).

For each NAME, the definition in each FILE is the top-level `typedef struct/union ... NAME;`, widened
to the `#ifdef TARGET_PORT` / `#endif` block around it when it sits in one (a block that holds
only that type's definitions), with the comment lines right above it. Every file must have the same
text (whitespace aside), or the name is skipped. The text is appended to HEADER (before its last
#endif; created when missing) in the order of the first file, removed from every file, and
`#include "HEADER"` is put in place of the first removed definition unless the file includes it
above that point. Check the result with tools/asm_diff.py.

Usage: python3 tools/hoist_blocks.py structs/twin_x.h NAME[,NAME...] -- FILE...
"""
import re, sys, os
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
HEAD = re.compile(r"^typedef\s+(?:struct|union)\b[^;{]*\{", re.M)


def typedefs(src):
    for m in HEAD.finditer(src):
        i, depth = m.end(), 1
        while i < len(src) and depth:
            depth += {"{": 1, "}": -1}.get(src[i], 0)
            i += 1
        e = src.index(";", i)
        name = re.findall(r"\w+", src[i:e])[-1]
        nl = src.find("\n", e)
        yield name, m.start(), (nl + 1 if nl >= 0 else len(src))


def line_start(src, i):
    return src.rfind("\n", 0, i) + 1


def find(src, name, group=()):
    tds = [(s, e) for n, s, e in typedefs(src) if n == name]
    if not tds:
        return None
    s, e = tds[0][0], tds[-1][1]
    # widen to an enclosing #ifdef TARGET_PORT ... #endif (host view / N64 view)
    k = src.rfind("#ifdef TARGET_PORT", 0, s)
    j = src.find("\n#endif", e - 1)
    if k >= 0 and j >= 0 and not re.search(r"^#(if|endif)", src[k + 1:s], re.M) \
            and not re.search(r"^#(if|endif)", src[e:j + 1], re.M):
        inner = src[k:j]
        if not ({n for n, _, _ in typedefs(inner)} - {name} - set(group)) and len(re.findall(r"^#if", inner, re.M)) == 1:
            s, e = k, src.find("\n", j + 1) + 1
    # comment lines right above
    while True:
        ls = line_start(src, s - 1) if s > 0 else 0
        line = src[ls:s]
        if s > 0 and (line.lstrip().startswith(("/*", "*")) and line.rstrip().endswith("*/") or line.lstrip().startswith(" * ") or line.lstrip().startswith("* ")):
            # take a whole comment block
            cs = src.rfind("/*", 0, s)
            if cs < 0 or "*/" in src[cs:s - 1].rstrip()[:-2]:
                break
            s = line_start(src, cs)
        else:
            break
    return s, e


def norm(t):
    return re.sub(r"\s+", " ", t).strip()


def main():
    a = sys.argv[1:]
    hdr, names, files = a[0], a[1].split(","), a[a.index("--") + 1:]
    srcs = {f: open(f).read() for f in files}
    texts, cuts = [], {f: [] for f in files}
    for n in names:
        found = {f: find(s, n, names) for f, s in srcs.items()}
        if any(v is None for v in found.values()):
            print("skip", n, "(not in every file)"); continue
        t = {norm(srcs[f][s:e]) for f, (s, e) in found.items()}
        if len(t) != 1:
            print("skip", n, "(texts differ)"); continue
        f0 = files[0]; s, e = found[f0]
        if (s, srcs[f0][s:e]) not in texts:
            texts.append((s, srcs[f0][s:e]))
        for f, se in found.items():
            if se not in cuts[f]:
                cuts[f].append(se)
        print("hoist", n)
    if not texts:
        return
    hp = ROOT / "include" / hdr
    body = "".join(t for _, t in sorted(texts))
    if hp.exists():
        h = hp.read_text(); k = h.rstrip().rfind("#endif")
        h = h[:k].rstrip("\n") + "\n" + body + "\n#endif\n"
    else:
        g = "STRUCTS_" + re.sub(r"\W", "_", Path(hdr).stem.upper()) + "_H"
        h = "#ifndef %s\n#define %s\n\n#include \"common.h\"\n\n%s\n#endif\n" % (g, g, body)
    hp.write_text(h)
    inc = '#include "%s"' % hdr
    for f, cs in cuts.items():
        s = srcs[f]
        cs = sorted(cs)
        first = cs[0][0]
        for a_, b_ in reversed(cs):
            s = s[:a_] + s[b_:]
        if inc not in s[:first]:
            s = s[:first] + inc + "\n" + s[first:]
        open(f, "w").write(s)


main()
