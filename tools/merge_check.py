#!/usr/bin/env python3
"""Would a module directory merge into one C file? (docs/MODULES.md, "Merging the other modules")

For each directory: tools/merge_module.py --dry-run (its refusals: .rodata not one block in .text order, cc1 flags),
then the merged file through cpp and the game's cc1, and, when it compiles, every function's assembly and the
.rodata data compared with the one-file-per-function build (labels renumbered). OK there means the merged object
should be byte-identical; `make setup && make build && make verify` after the real merge decides.

    python3 tools/merge_check.py                      # every module directory with 2+ C files
    python3 tools/merge_check.py code/Sort code/Demo  # these (relative to src/us)
    python3 tools/merge_check.py --json ...           # one JSON object per directory (the compiler's messages)

Scratch files go to $TMPDIR/merge_check. Runs without the build tree (cpp and tools/sn64/cc1 only).
"""
import argparse, os, re, subprocess, sys, json, tempfile
from concurrent.futures import ThreadPoolExecutor
REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
OUT = os.path.join(tempfile.gettempdir(), "merge_check")
MERGE = "tools/merge_module.py"
CPP = ["cpp", "-P", "-undef", "-Wall", "-lang-c", "-D_LANGUAGE_C", "-DF3DEX_GBI_2", "-D__GNUC__=2", "-DGAME_VERSION=\"us\"",
       "-I.", "-Iinclude", "-Ius/include", "-nostdinc"]
CCF = "-quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mno-fix4300 -mhard-float -mfp64".split()
FL = re.compile(r"^/\* cc1-flags: (.*) \*/$", re.M)
EXTRA = []
LAB = re.compile(r"\$?\.?L[C]?\d+\b|\$L\d+")


def compile_(src_text, path, src_dir):
    os.makedirs(os.path.dirname(path), exist_ok=True)
    c, i, s = path + ".c", path + ".i", path + ".s"
    open(c, "w").write(src_text)
    m = FL.search(src_text)
    flags = m.group(1).split() if m else CCF
    r = subprocess.run(CPP + ["-I" + src_dir, "-o", i, c], cwd=REPO, capture_output=True, text=True)
    if r.returncode:
        return None, "CPP: " + r.stderr
    subprocess.run(["unix2dos", "-q", i])
    if os.path.exists(s):
        os.remove(s)
    r = subprocess.run([os.path.join(REPO, "tools/sn64/cc1")] + flags + ["-o", s, i], cwd=REPO, capture_output=True, text=True)
    if r.returncode or not os.path.exists(s):
        return None, r.stderr
    return open(s).read(), r.stderr


def norm(asm):
    funcs, ro, cur, body, sect = {}, [], None, [], ".text"
    for line in asm.splitlines():
        st = line.strip()
        t = st if st.startswith("#nop") else line.split("#")[0].strip()
        if not t or t.startswith((".file", ".version")) or t == "gcc2_compiled.:":
            continue
        m = re.match(r"\.(text|rdata|data|bss)\b|\.section\s+([^,\s]+)", t)
        if m:
            sect = m.group(2) or "." + m.group(1)
        m = re.match(r"\.ent\s+(\w+)", t)
        if m:
            cur, body = m.group(1), []
            continue
        m = re.match(r"\.end\s+(\w+)", t)
        if m and cur:
            funcs[cur] = body
            cur = None
            continue
        if cur and sect in (".text", ".text_vm"):
            body.append(t)
        elif sect in (".rdata", ".rodata") and not re.match(r"\S+:$", t):
            ro.append(LAB.sub("L", t))

    def relabel(lines):
        mp = {}

        def f(m):
            mp.setdefault(m.group(0), "L%d" % len(mp))
            return mp[m.group(0)]
        return [LAB.sub(f, x) for x in lines]
    return {k: relabel(v) for k, v in funcs.items()}, ro


def one(d):
    tag = d.strip("/").replace("/", "__")
    od = os.path.join(OUT, tag)
    src_dir = os.path.join(REPO, "src/us", d)
    files = sorted(f for f in os.listdir(src_dir) if f.endswith(".c"))
    res = {"dir": d, "files": len(files)}
    r = subprocess.run(["python3", MERGE, "src/us/" + d.strip("/"), "--dry-run"] + EXTRA, cwd=REPO, capture_output=True, text=True)
    if r.returncode:
        res["status"] = "REFUSED"
        res["msg"] = ((r.stderr or r.stdout).strip().splitlines() or ["?"])[-1]
        return res
    asm, err = compile_(r.stdout, os.path.join(od, "merged"), src_dir)
    if asm is None:
        res["status"] = "COMPILE"
        res["msg"] = err.strip()
        return res
    mf, mro = norm(asm)
    rf, rro = {}, []
    refs = [(src_dir, f) for f in files]
    if "--vm" in EXTRA:   # one object over the engine and the VM: the VM part's files are references too
        vd = os.path.join(REPO, EXTRA[EXTRA.index("--vm") + 1])
        refs += [(vd, f) for f in sorted(os.listdir(vd)) if f.endswith(".c")]
        res["files"] = len(refs)
    for sd, f in refs:
        a, e = compile_(open(os.path.join(sd, f)).read(), os.path.join(od, "ref", f[:-2]), sd)
        if a is None:
            res["status"] = "REFCOMPILE"
            res["msg"] = f + ": " + e.strip()[:300]
            return res
        ff, fro = norm(a)
        rf.update(ff)
        rro += fro
    res["funcs"] = len(rf)
    res["textdiff"] = [k for k in rf if mf.get(k) != rf[k]]
    res["extra"] = [k for k in mf if k not in rf]
    res["rodiff"] = [len(mro), len(rro)] if sorted(mro) != sorted(rro) else None
    res["status"] = "OK" if not res["textdiff"] and not res["extra"] and not res["rodiff"] else "DIFF"
    if "warning" in err:
        res["warn"] = err.strip()[:800]
    return res


def all_dirs():
    out = []
    for top in ("code", "vm1", "vm2"):
        for root, _, fs in os.walk(os.path.join(REPO, "src/us", top)):
            if sum(f.endswith(".c") for f in fs) >= 2:
                out.append(os.path.relpath(root, os.path.join(REPO, "src/us")) + "/")
    return sorted(out)


def summary(r):
    st = r["status"]
    if st == "OK":
        return "OK      %s (%d files)" % (r["dir"], r["files"])
    if st == "DIFF":
        return "DIFF    %s: asm differs in %s%s" % (r["dir"], ",".join(r["textdiff"][:5]), " (.rodata differs)" if r["rodiff"] else "")
    msg = [l for l in r.get("msg", "").splitlines() if "warning" not in l and "previous" not in l and "In function" not in l]
    return "%-7s %s: %s" % (st, r["dir"], re.sub(r"^\S+:\d+: ", "", msg[0] if msg else "")[:160])


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("dirs", nargs="*")
    ap.add_argument("-j", type=int, default=3)
    ap.add_argument("--json", action="store_true")
    ap.add_argument("--margs", default="", help="more merge_module.py arguments (one directory)")
    ap.add_argument("--rodata-free", action="store_true", help="pass --rodata-free to merge_module (compile check only)")
    a = ap.parse_args()
    if a.rodata_free:
        EXTRA.append("--rodata-free")
    EXTRA.extend(a.margs.split())
    dirs = [d.rstrip("/") + "/" for d in a.dirs] or all_dirs()
    n = {}
    with ThreadPoolExecutor(a.j) as ex:
        for res in ex.map(one, dirs):
            n[res["status"]] = n.get(res["status"], 0) + 1
            print(json.dumps(res) if a.json else summary(res), flush=True)
    print(" ".join("%s %d" % kv for kv in sorted(n.items())), file=sys.stderr)


if __name__ == "__main__":
    main()
