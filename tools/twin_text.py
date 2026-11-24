"""twin_text.py VM1DIR VM2DIR REV [STEMS|-] : write each vm2 file as its vm1 twin's text (from git REV, one
function per file), the VM module's names given their _2 suffix (every identifier X with an X_2 symbol in
versions/), then asm_diff the vm2 file; keep it only if asm_diff is 0. Pairs by the defined function's
name (vm2 = vm1 + _2), else by sorted order when the counts agree.

    --to-vm1: the other way round: each vm1 file becomes its vm2 twin's text (from git REV, one function per
              file, e.g. the commit before the vm2 merge), the _2 names without their suffix; asm_diff the vm1 file.
    --base B: the asm_diff base (default 88d57333)."""
import sys, os, re, subprocess
R = os.path.dirname(os.path.dirname(os.path.abspath(__file__))); os.chdir(R)
argv = sys.argv[1:]
TO1 = "--to-vm1" in argv
if TO1:
    argv.remove("--to-vm1")
BASE = "88d57333"
if "--base" in argv:
    i = argv.index("--base"); BASE = argv[i + 1]; del argv[i:i + 2]
sys.argv[1:] = argv
v1, v2, rev = sys.argv[1:4]
syms = set()
for f in os.listdir("versions"):
    if f.startswith("symbol") and f.endswith(".txt"):
        for m in re.finditer(r"\b([A-Za-z_]\w*_2)\b", open("versions/" + f).read()):
            syms.add(m.group(1))
if TO1:
    ID = re.compile(r"\b[A-Za-z_]\w*\b")
    only = sys.argv[4].split(",") if len(sys.argv) > 4 and sys.argv[4] != "-" else None
    DEF = re.compile(r"^[A-Za-z_][\w \*]*?\b(\w+)\s*\([^;{]*\)\s*\{", re.M)
    def defs(t):
        return [m.group(1) for m in DEF.finditer(t) if m.group(1) not in ("if", "while", "switch", "for")]
    ls2 = subprocess.run(["git", "ls-tree", "--name-only", rev, "src/us/%s/" % v2], capture_output=True, text=True).stdout.split()
    by1 = {}
    for f in sorted(os.listdir("src/us/" + v1)):
        if f.endswith(".c"):
            for x in defs(open(os.path.join("src/us", v1, f)).read()):
                by1.setdefault(x, os.path.join("src/us", v1, f))
    ok = bad = 0
    for b in sorted(x for x in ls2 if x.endswith(".c")):
        t2 = subprocess.run(["git", "show", "%s:%s" % (rev, b)], capture_output=True, text=True).stdout
        a = None
        for x in defs(t2):
            if x.endswith("_2") and x[:-2] in by1:
                a = by1[x[:-2]]
        if a is None:
            print("NONE", b); continue
        if only and os.path.basename(a)[:-2] not in only:
            continue
        def rn(m):
            x = m.group(0)
            return x[:-2] if x in syms else x
        nt = "".join(l if l.lstrip().startswith("#include") else ID.sub(rn, l) for l in t2.splitlines(keepends=True))
        old = open(a).read()
        if nt == old:
            ok += 1; print("SAME", b, "->", a); continue
        open(a, "w").write(nt)
        r = subprocess.run(["python3", "tools/asm_diff.py", "--base", BASE, a], capture_output=True, text=True)
        if " 0 differ or fail" in r.stdout + r.stderr:
            ok += 1; print("OK  ", b, "->", a)
        else:
            bad += 1; open(a, "w").write(old); print("BAD ", b, "->", a)
    print("ok", ok, "bad", bad)
    sys.exit(0)
ls1 = subprocess.run(["git", "ls-tree", "--name-only", rev, "src/us/%s/" % v1], capture_output=True, text=True).stdout.split()
ls1 = sorted(x for x in ls1 if x.endswith(".c"))
ls2 = sorted(os.path.join("src/us", v2, f) for f in os.listdir("src/us/" + v2) if f.endswith(".c"))
DEF = re.compile(r"^[A-Za-z_][\w \*]*?\b(\w+)\s*\([^;{]*\)\s*\{", re.M)
def defs(t):
    return [m.group(1) for m in DEF.finditer(t) if m.group(1) not in ("if", "while", "switch", "for")]
txt1 = {a: subprocess.run(["git", "show", "%s:%s" % (rev, a)], capture_output=True, text=True).stdout for a in ls1}
by1 = {}
for a, t in txt1.items():
    d = [x for x in defs(t) if not t[:t.find(x)].rstrip().endswith("static")]
    for x in d:
        by1.setdefault(x, a)
pairs = []
for b in ls2:
    t = open(b).read()
    a = None
    for x in defs(t):
        if x.endswith("_2") and x[:-2] in by1:
            a = by1[x[:-2]]
    pairs.append((a, b))
if any(a is None for a, b in pairs) and len(ls1) == len(ls2):
    pairs = [(a if a else ls1[i], b) for i, (a, b) in enumerate(pairs)]
ID = re.compile(r"\b[A-Za-z_]\w*\b")
only = sys.argv[4].split(",") if len(sys.argv) > 4 and sys.argv[4] != "-" else None
ok = bad = 0
for a, b in pairs:
    if only and os.path.basename(b)[:-2] not in only:
        continue
    if a is None:
        print("NONE", b); continue
    t = txt1[a]
    def rn(m):
        x = m.group(0)
        return x + "_2" if (x + "_2") in syms and not x.endswith("_2") else x
    out = []
    for line in t.splitlines(keepends=True):
        out.append(line if line.lstrip().startswith("#include") else ID.sub(rn, line))
    nt = "".join(out)
    old = open(b).read()
    if nt == old:
        ok += 1; print("SAME", a, "->", b); continue
    open(b, "w").write(nt)
    r = subprocess.run(["python3", "tools/asm_diff.py", "--base", BASE, b], capture_output=True, text=True)
    if " 0 differ or fail" in r.stdout + r.stderr:
        ok += 1; print("OK  ", a, "->", b)
    else:
        bad += 1; open(b, "w").write(old); print("BAD ", a, "->", b)
print("ok", ok, "bad", bad)
