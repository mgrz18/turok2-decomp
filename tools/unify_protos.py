"""unify_protos.py DIR 'PROTO;' ['PROTO;' ...] : in every .c of src/us/DIR, replace each local declaration
of the prototype's name (one line, optionally extern, optionally trailing comment) with PROTO.
Files that declare it nowhere are left alone. Prints the files changed."""
import sys, os, re
d = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), "src/us", sys.argv[1])
protos = sys.argv[2:]
for p in protos:
    name = re.search(r"(\w+)\s*\(", p.replace("(*", "(")).group(1) if "(*" not in p else re.search(r"\(\*\s*(\w+)\)", p).group(1)
    pat = re.compile(r"^(?:extern\s+)?[A-Za-z_][\w\s\*]*?\b%s\s*\([^;{]*\)\s*;[ \t]*(?:/\*.*?\*/)?[ \t]*$" % re.escape(name), re.M)
    ch = []
    for f in sorted(os.listdir(d)):
        if not f.endswith(".c"):
            continue
        fp = os.path.join(d, f); s = open(fp).read()
        t = pat.sub(p.replace("\\", "\\\\"), s)
        if t != s:
            open(fp, "w").write(t); ch.append(f)
    print(name, len(ch), " ".join(ch))
