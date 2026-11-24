#!/usr/bin/env python3
"""unify.py DIR HEADER NAME... : drop the files' own declarations of NAMEs (one-line prototypes or
extern variables, file or block scope) in src/us/DIR and include HEADER in each file that used one."""
import os, re, sys
d, hdr, names = sys.argv[1], sys.argv[2], sys.argv[3:]
D = os.path.join("src/us", d)
for f in sorted(os.listdir(D)):
    if not f.endswith(".c"):
        continue
    p = os.path.join(D, f)
    s = open(p).read()
    out, hit = [], False
    for line in s.split("\n"):
        st = line.strip()
        drop = False
        for n in names:
            # a declaration: ends with ';' (comment allowed), names n followed by ( or [ or ;, no '=' and no 'return'
            if re.match(r"^(extern\s+)?[A-Za-z_][\w\s\*]*?\b%s\s*(\(.*\)|\[[^\]]*\])?\s*;\s*(/\*.*\*/)?\s*$" % re.escape(n), st) \
                    and "return" not in st and "=" not in st and not re.match(r"^%s\s*\(" % re.escape(n), st):
                drop = True
        if drop:
            hit = True
            continue
        out.append(line)
    if not hit:
        continue
    s = "\n".join(out)
    inc = '#include "%s"' % hdr
    if inc not in s:
        incs = list(re.finditer(r"^#include .*$", s, re.M))
        # first include block only (before any code)
        last = None
        for m in incs:
            if last is None or s[last.end():m.start()].strip() == "" or s[last.end():m.start()].strip().startswith("/*"):
                last = m
            else:
                break
        if last is None:
            s = inc + "\n" + s
        else:
            s = s[:last.end()] + "\n" + inc + s[last.end():]
    open(p, "w").write(s)
    print("edited", f)
