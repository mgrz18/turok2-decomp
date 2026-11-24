"""portwrap.py MERGED.c STEM[,STEM..] : put the functions of the given original files
(link.cmake T2_LINK_REPLACED / game.cmake entries) under #ifndef TARGET_PORT in the merged
file and turn their cmake lines into comments. Functions found by name from the cmake comment."""
import re,sys
merged=sys.argv[1]; stems=sys.argv[2].split(",")
cm={}
for f in ("port/cmake/link.cmake","port/cmake/game.cmake"):
    t=open(f).read()
    for st in stems:
        m=re.search(r"^(\s*)%s\.c\s+#\s*(\w+).*$"%st,t,re.M)
        if m: cm[st]=(f,m.group(0),m.group(1),m.group(2))
missing=[s for s in stems if s not in cm]; assert not missing,missing
s=open(merged).read()
names=[]
for st,(f,line,ind,name) in cm.items():
    m=re.search(r"^[A-Za-z_][\w \*]*\b%s\s*\([^;{]*\)\s*\{"%name,s,re.M)
    assert m,name
    i=s.index("\n}\n",m.start())+3
    s=s[:m.start()]+"#ifndef TARGET_PORT /* the port's in port/ (port/cmake: %s) */\n"%name+s[m.start():i]+"#endif\n"+s[i:]
    names.append(name)
    t=open(f).read(); t=t.replace(line,"%s# (%s: under #ifndef TARGET_PORT in the merged %s)"%(ind,name,merged.split("/")[-1])); open(f,"w").write(t)
open(merged,"w").write(s); print("wrapped",names)
