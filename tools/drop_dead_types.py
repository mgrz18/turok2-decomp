"""Drop the top-level type declarations of C files that nothing else in the file
uses (with their forward declarations and typedef aliases), keeping a removal
only if the file still compiles to the same code as at the base revision (DDT_BASE,
default HEAD; tools/asm_diff.py compile/normalize). Usage: TMPDIR=... [DDT_BASE=rev] python3 tools/drop_dead_types.py src/us/code/X.c ...
A comment right above a dropped type goes with it unless a blank line or a
"From ..." provenance line separates it."""
import re,sys,os,subprocess
from pathlib import Path
sys.path.insert(0,"tools")
import asm_diff as AD
ROOT=AD.ROOT
syms=AD.symbols(str(ROOT/"build"/"ref.elf"))
REV=subprocess.run(["git","-C",str(ROOT),"rev-parse","--short",os.environ.get("DDT_BASE","HEAD")],capture_output=True,text=True,check=True).stdout.strip()
BASE=Path(os.environ["TMPDIR"])/("base_"+REV)
if not (BASE/"src").exists():
    BASE.mkdir(parents=True,exist_ok=True)
    a=subprocess.run(["git","-C",str(ROOT),"archive",REV,"src","include"],capture_output=True,check=True).stdout
    subprocess.run(["tar","-x","-C",str(BASE)],input=a,check=True)
WORK=Path(os.environ["TMPDIR"])/("dt_%d"%os.getpid())
def compiled(tree,rel,tag):
    out=WORK/tag
    e=AD.compile_one(tree,rel,out)
    if e: return None
    return AD.normalize(out/(rel+".s"),syms)
def chunks(src):
    out=[];i=0;n=len(src);depth=0;start=0
    while i<n:
        c=src[i]
        if src.startswith("/*",i): i=src.index("*/",i)+2; continue
        if src.startswith("//",i): i=src.index("\n",i); continue
        if c in "\"'":
            j=i+1
            while src[j]!=c: j+= 2 if src[j]=="\\" else 1
            i=j+1; continue
        if c=="#" and depth==0 and (i==0 or src[i-1]=="\n"):
            j=src.find("\n",i)
            while src[j-1]=="\\": j=src.find("\n",j+1)
            out.append((start,j+1)); start=j+1; i=j+1; continue
        if c=="{": depth+=1
        elif c=="}":
            depth-=1
            if depth==0 and not re.match(r"[ \t]*(?:[A-Za-z_][\w \t,*\[\]()]*)?;",src[i+1:]):
                j=src.find("\n",i); j=n-1 if j<0 else j
                out.append((start,j+1)); start=j+1; i=j+1; continue
        elif c==";" and depth==0:
            j=src.find("\n",i); j=n-1 if j<0 else j
            out.append((start,j+1)); start=j+1; i=j+1; continue
        i+=1
    if start<n: out.append((start,n))
    return out
def strip_comments(t): return re.sub(r"/\*.*?\*/","",t,flags=re.S)
def declared(text):
    t=strip_comments(text).strip()
    if not re.match(r"(typedef\s|struct\s|union\s|enum\s)",t): return None
    head=t.split("{")[0]
    if "(" in head and not t.startswith("typedef"): return None
    if re.search(r"\)\s*\{",t): return None
    names=set(re.findall(r"\b(?:struct|union|enum)\s+(\w+)",t.split("{")[0]))
    if "{" in t:
        # members' nested tags count as declared only if defined with a body
        names|=set(re.findall(r"\b(?:struct|union|enum)\s+(\w+)\s*\{",t))
        tail=t[t.rindex("}")+1:]
    else:
        tail=t
    if t.startswith("typedef"):
        m=re.findall(r"(\w+)\s*(?:\[[^\]]*\])?\s*;\s*$",tail if "{" in t else t)
        if m: names.add(m[-1])
        # function pointer typedef: typedef r (*Name)(...)
        m=re.search(r"\(\s*\*\s*(\w+)\s*\)",t.split("{")[0] if "{" not in t else tail)
        if m: names.add(m.group(1))
    elif "{" in t and tail.strip(" ;\n"):
        return None   # defines a variable
    if "{" in t:
        # enum constants
        if re.match(r"(typedef\s+)?enum",t):
            body=t[t.index("{")+1:t.rindex("}")]
            names|=set(re.findall(r"(\w+)\s*(?:=[^,]*)?(?:,|$)",body))
    return names
for f in sys.argv[1:]:
    rel=str(Path(f).resolve().relative_to(ROOT))
    if not (BASE/rel).exists(): continue
    ref=compiled(BASE,rel,"a")
    cur=compiled(ROOT,rel,"b")
    if ref is None or cur!=ref:
        print("SKIP",f); continue
    orig=open(f).read(); removed=0
    changed=True
    while changed:
        changed=False
        src=open(f).read()
        cs=chunks(src)
        decl=[(a,b,declared(src[a:b])) for (a,b) in cs]
        for (a,b,names) in reversed(decl):
            if not names: continue
            group=[(x,y) for (x,y,nm) in decl if nm and nm<=names]
            keep=[src[x:y] for (x,y) in cs if (x,y) not in group]
            rest=strip_comments("".join(keep))
            if any(re.search(r"\b%s\b"%re.escape(x),rest) for x in names): continue
            lead=[]
            for (x,y) in group:
                t=src[x:y]
                m=re.match(r"(?:\s|/\*.*?\*/)*",t,flags=re.S)
                l=t[:m.end()]
                cut=l.rfind("\n\n")
                kept=l[:cut+2] if cut>=0 else ""
                rest_c=l[len(kept):]
                if "From " in rest_c or "WRONG CUT" in rest_c: kept=l
                lead.append(((x,y),kept))
            lead=dict(lead)
            new="".join(src[x:y] if (x,y) not in group else lead[(x,y)] for (x,y) in cs)
            open(f,"w").write(new)
            if compiled(ROOT,rel,"b")==ref:
                removed+=len(group); changed=True
                break
            else:
                open(f,"w").write(src)
    if removed:
        print(f,"removed",removed,len(orig.splitlines()),"->",len(open(f).read().splitlines()))
