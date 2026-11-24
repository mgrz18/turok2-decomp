#!/usr/bin/env python3
"""view_label.py FILE NAME [NAME..] : a file's own extern view of a global (`extern T NAME[..];`, a type of its own
where the header's changes the ROM's code) becomes an asm-label alias, `extern T NAME_v<stem> __asm__("NAME");`,
and the file's uses of NAME take the alias: one declaration per name in a merged module (docs/MODULES.md,
round 27 CPlayer). Comments and #include lines are left alone. Run asm_diff on the file afterwards."""
import re, sys, os
path, names = sys.argv[1], sys.argv[2:]
s = open(path).read()
stem = os.path.basename(path)[:-2].replace("func_", "")
for n in names:
    alias = "%s_v%s" % (n, stem)
    d = re.compile(r"^([ \t]*extern\s+[^;=()]*?\b)%s(\s*(?:\[[^\]]*\])*)\s*;" % re.escape(n), re.M)
    if not d.search(s):
        sys.exit("%s: no extern declaration of %s" % (path, n))
    s = d.sub(lambda m: '%s%s%s __asm__("%s");' % (m.group(1), alias, m.group(2), n), s)
    out, i = [], 0
    tok = re.compile(r'/\*.*?\*/|//[^\n]*|"(?:\\.|[^"\\])*"|^[ \t]*#[ \t]*include[^\n]*|\b%s\b' % re.escape(n), re.S | re.M)
    def rep(m):
        t = m.group(0)
        return alias if t == n else t
    s = tok.sub(rep, s)
open(path, "w").write(s)
