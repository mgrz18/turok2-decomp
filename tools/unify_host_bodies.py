#!/usr/bin/env python3
"""Drop N64-only alternatives whose host (`TARGET_PORT`) branch compiles to the same N64 code.

Many files carry two versions of the same code:

    #ifdef TARGET_PORT
    ...typed, natural C written for the native port...
    #else
    ...the matched N64 draft (often m2c casts, a FAKEMATCH, a pin)...
    #endif

When the first branch, compiled for the N64 with the Makefile's cpp and cc1,
gives the same assembly as the second (every symbol resolved to its address,
local labels numbered by first appearance: tools/asm_diff.py's normalisation),
the N64 alternative is redundant: this replaces the whole conditional by the
host branch, so both builds compile one text. The port's preprocessed source is
unchanged by that (it already took the first branch), so the port binary is the
same; the N64 assembly is the same by construction.

Each `#ifdef TARGET_PORT / #else / #endif` (or `#ifndef TARGET_PORT / #else`,
`#if defined(TARGET_PORT)`) pair with both branches non-empty is tried on its
own, in file order, on top of the pairs already accepted in that file.
Conditionals with no `#else` (port-only additions) and `#elif` chains are left.

Usage:
    python3 tools/unify_host_bodies.py                 every src/ file, report only
    python3 tools/unify_host_bodies.py --write         rewrite the files
    python3 tools/unify_host_bodies.py --write src/us/code/func_00253720.c ...
    -j N   parallel files (default 3)

Re-runnable: after a merge, run it again with --write; it only touches pairs
that are still redundant. Run it until it reports 0: dropping a pair can make
another one redundant (a declaration the next pair's host branch needs).
"""

import argparse
import os
import re
import shutil
import sys
import tempfile
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import asm_diff  # noqa: E402

ROOT = asm_diff.ROOT
COND = re.compile(r"^\s*#\s*(ifdef|ifndef|if|elif|else|endif)\b(.*?)\s*(?:/\*.*\*/)?\s*$")
PORT = {"TARGET_PORT", "defined(TARGET_PORT)", "defined TARGET_PORT"}
NPORT = {"!defined(TARGET_PORT)", "!defined TARGET_PORT"}


def pairs(lines):
    """[(if_line, else_line, endif_line, port_first)] for TARGET_PORT if/else/endif blocks."""
    out, stack = [], []
    for n, line in enumerate(lines):
        m = COND.match(line)
        if not m:
            continue
        kw, rest = m.group(1), m.group(2).strip()
        if kw in ("ifdef", "if") and rest in PORT:
            stack.append(["T", n, None, True, False])
        elif (kw == "ifndef" and rest == "TARGET_PORT") or (kw == "if" and rest in NPORT):
            stack.append(["T", n, None, False, False])
        elif kw in ("ifdef", "ifndef", "if"):
            stack.append(["O"])
        elif kw == "elif":
            if stack and stack[-1][0] == "T":
                stack[-1][4] = True          # an #elif chain: leave it
        elif kw == "else":
            if stack and stack[-1][0] == "T":
                stack[-1][2] = n
        elif kw == "endif" and stack:
            e = stack.pop()
            if e[0] == "T" and e[2] is not None and not e[4]:
                out.append((e[1], e[2], n, e[3]))
    return out


def unify(lines, pair):
    i, e, d, port_first = pair
    keep = lines[i + 1:e] if port_first else lines[e + 1:d]
    drop = lines[e + 1:d] if port_first else lines[i + 1:e]
    if not any(l.strip() for l in keep) or not any(l.strip() for l in drop):
        return None
    return lines[:i] + keep + lines[d + 1:]


def n64_asm(rel_dir, text, rel, syms, tag):
    """Normalised N64 assembly of `text` compiled as if it were `rel`."""
    tmp_rel = os.path.join(rel_dir, tag, rel)
    path = ROOT / tmp_rel
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(text)
    out_dir = ROOT / rel_dir / (tag + ".s")
    err = asm_diff.compile_one(ROOT, tmp_rel, out_dir)
    if err:
        return None
    return asm_diff.normalize(out_dir / (tmp_rel + ".s"), syms)


def process(rel, syms, write, work):
    text = (ROOT / rel).read_text(errors="replace")
    lines = text.split("\n")
    tag = rel.replace("/", "_")
    base = n64_asm(work, text, rel, syms, tag + ".base")
    if base is None:
        return rel, 0, 0, "base does not compile"
    accepted = tried = 0
    while True:
        changed = False
        for pair in pairs(lines):
            cand = unify(lines, pair)
            if cand is None:
                continue
            key = "\n".join(lines[pair[0]:pair[2] + 1])
            if key in process.rejected.setdefault(rel, set()):
                continue
            tried += 1
            got = n64_asm(work, "\n".join(cand), rel, syms, tag + ".cand")
            if got is None:                 # did not compile: not judged, left as is
                process.rejected[rel].add(key)
                if (rel, key) not in process.failed:
                    process.failed.append((rel, key))
                continue
            if got == base:
                lines = cand
                accepted += 1
                changed = True
                process.rejected[rel] = set()   # earlier pairs may pass now
                break                       # line numbers moved: rescan
            process.rejected[rel].add(key)
        if not changed:
            break
    if accepted and write:
        (ROOT / rel).write_text("\n".join(lines))
    return rel, accepted, tried, ""


process.rejected = {}
process.failed = []


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("files", nargs="*")
    ap.add_argument("--write", action="store_true", help="rewrite the files")
    ap.add_argument("--elf", default=str(ROOT / "build" / "turok2.us.elf"))
    ap.add_argument("-j", type=int, default=3)
    args = ap.parse_args()
    files = [str(Path(f).resolve().relative_to(ROOT)) for f in args.files] or sorted(
        str(p.relative_to(ROOT)) for p in (ROOT / "src").rglob("*.c")
        if "TARGET_PORT" in p.read_text(errors="replace"))
    syms = asm_diff.symbols(args.elf)
    work = tempfile.mkdtemp(prefix="unify.", dir=ROOT / "build")
    work_rel = os.path.relpath(work, ROOT)
    total = 0
    try:
        with ThreadPoolExecutor(args.j) as ex:
            for rel, acc, tried, err in ex.map(lambda f: process(f, syms, args.write, work_rel), files):
                if err:
                    print(f"FAIL {rel}: {err}", flush=True)
                elif acc:
                    total += acc
                    print(f"{'UNIFIED' if args.write else 'REDUNDANT'} {rel}: {acc} of {tried}", flush=True)
    finally:
        shutil.rmtree(work, ignore_errors=True)
    print(f"{len(files)} files: {total} N64 alternatives {'dropped' if args.write else 'redundant'}")
    if process.failed:
        print(f"{len(process.failed)} candidates did not compile (host-only names): left as they are")
    return 0


if __name__ == "__main__":
    sys.exit(main())
