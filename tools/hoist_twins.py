"""Hoist the local struct typedefs that a vm1 module and its vm2 copy define with one identical
layout into include/structs/twin_<module>.h, then run tools/hoist_structs.py on the files."""
import re,glob,collections,subprocess,sys,os
TD=re.compile(r"^typedef (struct|union)( \w+)? \{.*?^\} (\w+);[^\n]*\n",re.M|re.S)
skip=set(sys.argv[1].split(",")) if len(sys.argv)>1 else set()
defs=collections.defaultdict(list)
files=glob.glob("src/us/vm1/**/*.c",recursive=True)+glob.glob("src/us/vm2/**/*.c",recursive=True)+glob.glob("src/us/code/**/*.c",recursive=True)
for p in files:
    s=open(p).read()
    for m in TD.finditer(s):
        body=re.sub(r"/\*.*?\*/","",m.group(0),flags=re.S); body=re.sub(r"\s+"," ",body).strip()
        defs[m.group(3)].append((p,body,m.group(0),m.start()))
bymod=collections.defaultdict(list)
for n,l in defs.items():
    if n in skip or len(l)<2: continue
    segs={p.split("/")[2] for p,*_ in l}
    if segs!={"vm1","vm2"}: continue
    if len({b for _,b,_,_ in l})!=1: continue
    mods={p.split("/")[3] for p,*_ in l}
    if len(mods)!=1: continue
    bymod[mods.pop()].append(n)
for mod,names in sorted(bymod.items()):
    l=[(p,raw,pos) for n in names for (p,b,raw,pos) in defs[n]]
    v1=sorted({p for p,_,_ in l if "/vm1/" in p}); allf=sorted({p for p,_,_ in l})
    # order: as in the first vm1 file that has them
    src=open(v1[0]).read()
    order=sorted(names,key=lambda n: min(pos for (p,b,raw,pos) in defs[n] if p==v1[0]) if any(p==v1[0] for p,*_ in defs[n]) else 1e9)
    incs=[]
    for p in v1:
        t=open(p).read(); first=min([pos for n in names for (q,b,raw,pos) in defs[n] if q==p] or [len(t)])
        for i in re.findall(r'^#include "[^"]+"',t[:first],re.M):
            if i not in incs and re.search(r'"(common.h|ultra_types.h|structs/[^"]*|rw/abu_types.h)"',i): incs.append(i)
    hn="twin_%s.h"%mod.lower(); guard="STRUCTS_TWIN_%s_H"%mod.upper()
    body="".join(next(raw for (p,b,raw,pos) in defs[n]) for n in order)
    h=("/* Local types that vm1/%s and its vm2 copy both define (one layout): one definition\n"
       " * for both copies (tools/struct_census.py). */\n#ifndef %s\n#define %s\n\n%s\n\n%s\n#endif\n")%(mod,guard,guard,"\n".join(incs),body)
    open("include/structs/"+hn,"w").write(h)
    r=subprocess.run(["python3","tools/hoist_structs.py","structs/"+hn,"--"]+allf,capture_output=True,text=True)
    print(mod,hn,order,len(allf),"files",r.returncode,r.stderr.strip()[:200])
