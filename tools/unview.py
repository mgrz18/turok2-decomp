#!/usr/bin/env python3
"""Replace a file's host-view twins (tools/port/hostview.py) by the real struct.

A view the port needed has two definitions in its file:

    #ifdef TARGET_PORT
    typedef struct V {
    #define T2_HOFF_V(m) __builtin_offsetof(REAL, m)
        union { struct { u8 _hp_f[T2_HOFF_V(path)]; T f; }; ... };
    } V;
    #else
    typedef struct { ... N64 offsets ... } V;
    #endif

The host twin already says which member of REAL each view field is. This drops
both definitions, includes REAL's header, renames V to REAL and every `->f` /
`.f` of the file to `->path` / `.path`, then compiles the file with
tools/asm_diff.py against BASE and puts it back unless the N64 code is
unchanged (a field the real struct types differently, a name another struct
of the file also uses: those stay views). Views listed with --only are the only
ones touched.

With --twin-from OTHER.c (read at --base), the twins of OTHER.c are applied to
the N64-only views of the same names in the files given: the second copy of a
VM module has the views of the first without their host twins.

Usage:
    TMPDIR=... python3 tools/unview.py [--base REV] [--only V,...] [--twin-from F.c] [--dry-run] src/us/code/X.c ...
"""
import argparse
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
TWIN = re.compile(
    r"#ifdef TARGET_PORT\n(typedef struct (\w+) \{\n(?:/\*.*?\*/\n)?"
    r"#define T2_HOFF_\2\(m\) __builtin_offsetof\((\w+), m\)\n.*?\} \2;\n)"
    r"#else\n(typedef (?:struct|union)\b[^{]*\{.*?\} \2;\n)#endif\n", re.S)
FIELD = re.compile(r"struct \{ u8 _hp_\w+\[T2_HOFF_\w+\((.+?)\)\]; [^;]*?\**\s*(\w+)\s*(?:\[[^\]]*\])?; \};")


def headers():
    out = {}
    for h in sorted((ROOT / "include" / "structs").glob("*.h")):
        for n in re.findall(r"^\}\s*(\w+)\s*;", h.read_text(), re.M):
            out.setdefault(n, f"structs/{h.name}")
        for n in re.findall(r"^struct (\w+) \{", h.read_text(), re.M):
            out.setdefault(n, f"structs/{h.name}")
    return out


MODE = {"precise": True}


def rename(src, view, real, fmap):
    """V -> REAL, and its fields: only through the variables declared as V (precise) or everywhere."""
    names = set(re.findall(rf"\b{view}\s*\*+\s*(?:const\s+)?(\w+)", src))
    names |= set(re.findall(rf"\b{view}\s+(\w+)\s*[;,)=\[]", src))
    src = re.sub(rf"\b{view}\b", real, src)
    for f, path in fmap.items():
        if f == path:
            continue
        if MODE["precise"]:
            for n in names:
                src = re.sub(rf"\b{n}\s*(->|\.)\s*{f}\b", lambda x, p=path, n=n: n + x.group(1) + p, src)
            src = re.sub(rf"(\({real}\s*\*\)\s*\w+\)\s*->\s*){f}\b", lambda x, p=path: x.group(1) + p, src)
        else:
            src = re.sub(rf"(->|\.)\s*{f}\b", lambda x, p=path: x.group(1) + p, src)
    return src


def convert_from(src, twins, only, hdrs):
    """Apply twins {view: (real, fmap)} found elsewhere to this file's plain views."""
    sys.path.insert(0, str(ROOT / "tools"))
    from hoist_structs import spans
    done = []
    for view, s0, e0 in sorted(spans(src), key=lambda t: -t[1]):
        if view not in twins or (only and view not in only) or twins[view][0] not in hdrs:
            continue
        src = src[:s0] + src[e0:]
        done.append(view)
    for view in done:
        real, fmap = twins[view]
        inc = f'#include "{hdrs[real]}"\n'
        # a header the file included only for its host twins is needed on the N64 too now
        src = src.replace("#ifdef TARGET_PORT\n" + inc + "#endif\n", inc)
        if inc not in src:
            incs = [x.end() for x in re.finditer(r"^#include[^\n]*\n", src, re.M)]
            at = incs[0] if incs else 0
            src = src[:at] + inc + src[at:]
        src = rename(src, view, real, fmap)
    return (src if done else None), [(v, twins[v][0]) for v in done]


def convert(src, only, hdrs):
    done = []
    for m in list(TWIN.finditer(src)):
        view, real = m.group(2), m.group(3)
        if only and view not in only:
            continue
        fmap = {f: path for path, f in FIELD.findall(m.group(1))}
        if real not in hdrs:
            continue
        done.append((m, view, real, fmap))
    if not done:
        return None, []
    for m, view, real, fmap in reversed(done):
        src = src[:m.start()] + src[m.end():]
    for m, view, real, fmap in done:
        inc = f'#include "{hdrs[real]}"\n'
        # a header the file included only for its host twins is needed on the N64 too now
        src = src.replace("#ifdef TARGET_PORT\n" + inc + "#endif\n", inc)
        if inc not in src:
            incs = [x.end() for x in re.finditer(r"^#include[^\n]*\n", src, re.M)]
            at = incs[0] if incs else 0
            src = src[:at] + inc + src[at:]
        src = rename(src, view, real, fmap)
    return src, [(v, r) for _, v, r, _ in done]


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--base", default="HEAD")
    ap.add_argument("--only", default="")
    ap.add_argument("--twin-from", default="")
    ap.add_argument("--dry-run", action="store_true")
    ap.add_argument("files", nargs="+")
    a = ap.parse_args()
    only = set(filter(None, a.only.split(",")))
    hdrs = headers()
    twins = {}
    if a.twin_from:
        other = subprocess.run(["git", "-C", str(ROOT), "show", f"{a.base}:{a.twin_from}"],
                               capture_output=True, text=True, check=True).stdout
        twins = {m.group(2): (m.group(3), {f: path for path, f in FIELD.findall(m.group(1))})
                 for m in TWIN.finditer(other)}
    for f in a.files:
        p = Path(f)
        views = list(twins) if twins else [m.group(2) for m in TWIN.finditer(p.read_text())]
        for v in views:
            if only and v not in only:
                continue
            old = p.read_text()
            ok = False
            for mode in (True, False):
                MODE["precise"] = mode
                if twins:
                    new, done = convert_from(old, twins, {v}, hdrs)
                else:
                    new, done = convert(old, {v}, hdrs)
                if new is None:
                    break
                p.write_text(new)
                if a.dry_run:
                    print("converted", p.name, done)
                    ok = True
                    break
                r = subprocess.run([sys.executable, str(ROOT / "tools" / "asm_diff.py"), "--base", a.base, str(p)],
                                   capture_output=True, text=True)
                ok = " 0 differ or fail" in r.stdout
                if ok:
                    break
                p.write_text(old)
            if new is None or a.dry_run:
                continue
            print("OK  " if ok else "KEPT", p.name, " ".join(f"{x}->{y}" for x, y in done))

if __name__ == "__main__":
    main()
