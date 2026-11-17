#!/usr/bin/env python3
"""Round 15 d5: pointer out-parameters declared as 32-bit cells elsewhere.

A function defined with a `T **` parameter (it stores a pointer: 8 bytes on the host) but
declared in another file with `s32 *` / `u32 *` / `int *` there: the caller's 4-byte local is
overrun by 4 bytes (AI_FindClearHeading's `s32 out` for GameObject_TestMove). Regex-level scan
of src/ (definitions and prototypes on one line). For each such prototype, the calls in that
file that pass `&local` with the local declared as a 32-bit type are the overruns (BAD); the
others are listed as ok (the local is pointer-sized, sptr, or a pointer is passed through)."""
import os, re, sys

root = sys.argv[1] if len(sys.argv) > 1 else "src"
DEF = re.compile(r"^[A-Za-z_][\w \*]*?\b(\w+)\s*\(([^;{)]*)\)\s*\{", re.M)
PROTO = re.compile(r"^\s*(?:extern\s+)?[A-Za-z_][\w \*]*?\b(\w+)\s*\(([^;{)]*)\)\s*;", re.M)
defs, protos, texts = {}, [], {}
for d, _, fs in os.walk(root):
    for f in fs:
        if not f.endswith(".c"):
            continue
        p = os.path.join(d, f)
        s = open(p, errors="replace").read()
        s = re.sub(r"/\*.*?\*/", "", s, flags=re.S)
        texts[p] = s
        for m in DEF.finditer(s):
            defs.setdefault(m.group(1), []).append((p, [a.strip() for a in m.group(2).split(",")]))
        for m in PROTO.finditer(s):
            protos.append((p, m.group(1), [a.strip() for a in m.group(2).split(",")]))
def calls(text, name):
    """The argument lists of the calls of name in text (balanced parentheses; top-level commas)."""
    for m in re.finditer(r"\b%s\s*\(" % name, text):
        depth, cur, out, j = 1, "", [], m.end()
        while j < len(text) and depth:
            ch = text[j]
            if ch == "(":
                depth += 1
            elif ch == ")":
                depth -= 1
            if depth == 1 and ch == ",":
                out.append(cur.strip()); cur = ""
            elif depth:
                cur += ch
            j += 1
        out.append(cur.strip())
        yield out


narrow = re.compile(r"^(const\s+)?(s32|u32|int|unsigned|f32|s16|u16)\s*\*\s*\w*$")
for p, name, args in protos:
    for dp, dargs in defs.get(name, []):
        if len(dargs) != len(args):
            continue
        for i, (a, b) in enumerate(zip(args, dargs)):
            if narrow.match(a) and b.count("*") >= 2:
                bad = []
                for parts in calls(texts[p], name):
                    if i < len(parts) and parts[i].startswith("&"):
                        v = parts[i][1:].strip()
                        if re.search(r"\b(s32|u32|int|f32|unsigned)\s+(\w+\s*,\s*)*%s\b\s*[;,=]" % re.escape(v), texts[p]):
                            bad.append(v)
                print("%s %s: %s arg %d declared '%s', defined '%s' (%s)%s" % ("BAD" if bad else "ok ", p, name, i, a, b, dp,
                      " locals: " + ",".join(bad) if bad else ""))
