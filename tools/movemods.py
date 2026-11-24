#!/usr/bin/env python3
"""Move each function's C file into its module's directory (versions/modules.us.txt `// dir`)
and rewrite the paths in versions/turok2.us.yaml and in src comments. Round 26 o2."""
import re, subprocess, sys
from pathlib import Path
ROOT = Path.cwd()
mods = []; cur = None
for l in (ROOT / "versions/modules.us.txt").read_text().splitlines():
    m = re.match(r"module (\S+): ", l)
    if m: cur = {"name": m.group(1), "dir": None, "funcs": []}; mods.append(cur); continue
    m = re.match(r"\s+// dir (\S+)", l)
    if m and cur: cur["dir"] = m.group(1); continue
    m = re.match(r"\s+0x([0-9A-F]{8}) (\S+)", l)
    if m and cur: cur["funcs"].append(int(m.group(1), 16))
idx = {p.stem: p for p in (ROOT / "src/us").rglob("*.c")}
moves = []
for m in mods:
    d = m["dir"]
    if not d or not d.startswith("src/us/"): continue
    off = 0x39000 if d.startswith("src/us/vm2/") else 0
    for v in m["funcs"]:
        stem = f"func_{v + off:08X}"
        p = idx.get(stem)
        if p is None: continue
        rel = p.relative_to(ROOT)
        if str(rel.parent) != d:
            moves.append((rel, Path(d) / p.name))
for a, b in moves:
    b.parent.mkdir(parents=True, exist_ok=True)
    subprocess.run(["git", "mv", str(a), str(b)], check=True)
pm = {str(a)[len("src/us/"):-2]: str(b)[len("src/us/"):-2] for a, b in moves}
print(len(moves), "files moved")
pat = re.compile(r"\b(code|vm1|vm2)/[A-Za-z0-9_/]+/func_[0-9A-F]{8}\b")
files = [ROOT / "versions/turok2.us.yaml"] + list((ROOT / "src/us").rglob("*.c")) + list((ROOT / "versions").glob("*.txt"))
n = 0
for f in files:
    t = f.read_text(); t2 = pat.sub(lambda m: pm.get(m.group(0), m.group(0)), t)
    if t2 != t: f.write_text(t2); n += 1
print(n, "files rewritten")
