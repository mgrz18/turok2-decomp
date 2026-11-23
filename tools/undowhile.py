#!/usr/bin/env python3
"""Drop the `do { ... } while (0);` wrappers a file does not need.

The bulk matching and the permuter left `do { } while (0)` blocks in the C: a loop
note changes GCC's register priorities (refs are weighted by loop depth) and stops
cse from following a jump, so some of them are what the ROM's allocation needs.
Many are not. For each one outside a macro, this tries the plain block in its place
and keeps it when tools/match_func.py still reports every function of the file
MATCH; the rest are left as they are (docs/MATCHING.md, round 22). The bare blocks
left behind are unwrapped by hand (or by a later pass); check with tools/asm_diff.py.

Usage: tools/undowhile.py <file.c>...
"""
import re,subprocess,sys,os
from pathlib import Path
os.chdir(Path(__file__).resolve().parent.parent)
env=dict(os.environ)
def ok(path):
    r=subprocess.run([sys.executable,"tools/match_func.py",path],capture_output=True,text=True,env=env)
    lines=[l for l in r.stdout.splitlines() if l.startswith("  ") and "not in the ROM" not in l]
    return r.returncode==0 and lines and all(l.rstrip().endswith("MATCH") for l in lines)
def occurrences(s):
    out=[]
    for m in re.finditer(r"\bdo\s*\{",s):
        ls=s.rfind("\n",0,m.start())+1
        line=s[ls:s.find("\n",m.start())]
        if line.lstrip().startswith("#") or line.rstrip().endswith("\\"): continue
        i=s.index("{",m.start()); d=0; j=i
        while j<len(s):
            c=s[j]
            if c=="{": d+=1
            elif c=="}":
                d-=1
                if d==0: break
            j+=1
        w=re.match(r"\}\s*while\s*\(\s*0\s*\)\s*;",s[j:])
        if w: out.append((m.start(),i,j,j+w.end()))
    return out
for f in sys.argv[1:]:
    s=open(f).read()
    if not ok(f):
        print(f,"base does not match, skipped",flush=True); continue
    k=0; kept=0
    while True:
        occ=occurrences(s)
        if k>=len(occ): break
        a,i,j,e=occ[k]
        t=s[:a]+s[i:j]+"}"+s[e:]
        open(f,"w").write(t)
        if ok(f):
            s=t; kept+=1
        else:
            open(f,"w").write(s); k+=1
    print(f,"removed",kept,"kept",k,flush=True)
