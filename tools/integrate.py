#!/usr/bin/env python3
"""Wire a batch of matched C files into the build and keep only what verifies.

Each file passed matched on its own (tools/match_func.py). Linked together
with everything else, some still break the build; this finds those, takes
them out and tries again, until `make verify` passes. What it handles:

  - a clean slate every time: `make setup` does not delete asm a previous
    split left in us/, and `make verify` compares a stale ROM unless the ELF
    and the ROM are both gone;
  - pool symbols (D_..., jtbl_...) a C file's .rodata slice now carries while
    asm elsewhere still names them: defined for ld in
    versions/foreign_labels.us.txt;
  - a function some asm calls or jumps into past its start (a second entry
    point, a label): the link names it; that function is left out;
  - a function that links but builds other bytes (the VM modules' second
    copy, whose jumps land in the first copy): tools/romdiff.py names it;
    left out.

Left-out functions are listed in build/integrate/excluded.txt, with why, so
the next batch skips them.

Usage (on the build box):
    tools/integrate.py build/manual/abu/func_X.c [...]      # or @list.txt
"""

import re
import struct
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
OUT = ROOT / "build" / "integrate"
EXCL = OUT / "excluded.txt"
FOREIGN = ROOT / "versions" / "foreign_labels.us.txt"
PY = sys.executable


def sh(cmd):
    """A fixed command line of this script's own (no outside input in it)."""
    return subprocess.run(cmd, shell=True, cwd=ROOT, capture_output=True, text=True)


def clean_build():
    sh("rm -rf us build/src build/asm build/turok2.us.elf build/turok2.us.z64")
    sh(f"{PY} tools/apply_names.py")
    sh("make setup")
    sh("make -j$(nproc) build")


def link_errors():
    r = sh("make verify")
    return r.stdout + r.stderr


def address(name):
    m = re.match(r"^(?:func_|\.L|D_|jtbl_)?([0-9A-F]{8})$", name)
    return int(m.group(1), 16) if m else None


def owner(addr, names):
    """The batch function whose code holds addr: the nearest start below it."""
    below = [n for n in names if int(n[5:], 16) <= addr]
    return max(below, key=lambda n: int(n[5:], 16)) if below else None


def main():
    args = sys.argv[1:]
    files = [Path(l.strip()) for a in args for l in (Path(a[1:]).read_text().split() if a.startswith("@") else [a])]
    OUT.mkdir(parents=True, exist_ok=True)
    excluded = dict(l.split(" ", 1) for l in EXCL.read_text().splitlines() if l) if EXCL.exists() else {}
    have = {p.stem for p in (ROOT / "src").rglob("func_*.c")}
    todo = {p.stem: p for p in files if p.stem not in have and p.stem not in excluded}
    print(f"{len(files)} files, {len(todo)} to try")
    for attempt in range(1, 7):
        if not todo:
            break
        sh("git checkout -q -- versions/turok2.us.yaml versions/foreign_labels.us.txt src && git clean -fdq src")
        clean_build()
        if "OK" not in link_errors():
            sys.exit("the tree does not verify before the batch: fix that first")
        sh("cp build/turok2.us.elf build/ref.elf")
        res = subprocess.run([PY, "tools/accept_manual.py", *map(str, todo.values())], cwd=ROOT,
                             capture_output=True, text=True, env={**__import__("os").environ, "MATCH_ELF": "build/ref.elf"})
        for m in re.finditer(r"skip (func_[0-9A-F]{8}): (.*)", res.stdout):
            excluded[m.group(1)] = m.group(2)
            todo.pop(m.group(1), None)
        clean_build()
        bad = {}
        for _ in range(4):
            log = link_errors()
            if "z64: OK" in log:
                break
            undef = set(re.findall(r"undefined reference to `([^']+)'", log))
            pool = {s for s in undef if re.match(r"^(D|jtbl)_[0-9A-F]{8}$", s)}
            if pool:
                with FOREIGN.open("a") as fh:
                    fh.writelines(f"{s} = 0x{s.split('_')[1]};\n" for s in sorted(pool))
                sh("rm -f build/turok2.us.elf build/turok2.us.z64")
                continue
            for s in undef - pool:
                a = address(s)
                o = owner(a, todo) if a else None
                for m in re.finditer(r"code/(func_[0-9A-F]{8})\.i:\S* undefined reference to `" + re.escape(s), log):
                    bad[m.group(1)] = f"calls {s}, which nothing defines here"
                if o:
                    bad[o] = f"holds {s}, which other code reaches"
            if not bad:
                diff = sh(f"{PY} tools/romdiff.py --max-runs 5000").stdout
                for n in set(re.findall(r"^code/(func_[0-9A-F]{8})\s+c\s", diff, re.M)):
                    bad[n] = "links, but builds other bytes"
            break
        if "z64: OK" in link_errors():
            wired = sh("git status --short src").stdout.count("?? ")
            print(f"attempt {attempt}: verifies, {wired} functions in")
            break
        if not bad:
            sys.exit(f"attempt {attempt}: does not verify and no culprit found; see make verify")
        print(f"attempt {attempt}: leaving out {len(bad)}")
        excluded.update(bad)
        for n in bad:
            todo.pop(n, None)
    EXCL.write_text("".join(f"{n} {why}\n" for n, why in sorted(excluded.items())))
    sh("cp build/turok2.us.elf build/ref.elf")
    print(sh(f"{PY} tools/progress.py").stdout)
    return 0


if __name__ == "__main__":
    sys.exit(main())
