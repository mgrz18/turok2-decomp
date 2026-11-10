#!/usr/bin/env python3
"""List float -> unsigned integer conversions in the game sources (docs/port/MACOS.md section 9).

    python3 tools/port/f2ucheck.py --build OUT/build [-j 8] [files...]

C leaves a float -> unsigned conversion of a negative value undefined. The R4300 code the
matching build reproduces converts through a signed word (cvt.w.s / trunc.w.s), so a negative
value comes out as its two's complement. x86-64 clang does the same in practice: it converts
through a 64-bit signed integer and keeps the low bits. arm64's fcvtzu SATURATES: every
negative value becomes 0. That is a macOS-only (arm64) difference. It is invisible on the Linux
box. A typical case is guMtxF2L-style matrix packing through (u32) casts, where every negative
matrix element becomes 0.

Runs `clang -fsyntax-only -Xclang -ast-dump` on each TU from compile_commands.json and prints every
FloatingToIntegral conversion, explicit or implicit, whose destination type is unsigned
(u8/u16/u32/u64, unsigned char/short/int/long).
"""
import argparse
import json
import os
import re
import shlex
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor

CONV = re.compile(r"(CStyleCastExpr|ImplicitCastExpr) 0x[0-9a-f]+ <([^>]*)> '([^']*)'(?::'([^']*)')? <FloatingToIntegral>")
LOC = re.compile(r"(?:([^\s:<>]+\.[ch]):)?(?:line:)?(\d+):(\d+)")


def scan(entry):
    args = entry.get('arguments') or shlex.split(entry['command'])
    out = []
    skip = False
    for a in args[1:]:
        if skip:
            skip = False
            continue
        if a in ('-o', '-MF', '-MT', '-MQ'):
            skip = True
            continue
        if a in ('-c', '-MD', '-MMD') or a.startswith('-W'):
            continue
        out.append(a)
    cmd = [args[0], '-fsyntax-only', '-w', '-Xclang', '-ast-dump', '-fno-color-diagnostics'] + out
    try:
        r = subprocess.run(cmd, cwd=entry['directory'], capture_output=True, text=True, timeout=300)
    except subprocess.TimeoutExpired:
        return entry['file'], [('timeout', '', '')]
    hits = []
    cur_file = entry['file']
    cur_line = '?'
    for line in r.stdout.splitlines():
        # track the last absolute location (the dump elides file/line when unchanged)
        for m in re.finditer(r"<([^>]*)>", line[:400]):
            s = m.group(1)
            fm = re.search(r"([^\s:<>,]+\.[ch]):(\d+):\d+", s)
            if fm:
                cur_file, cur_line = fm.group(1), fm.group(2)
            else:
                lm = re.search(r"line:(\d+):\d+", s)
                if lm:
                    cur_line = lm.group(1)
            break
        m = CONV.search(line)
        if not m:
            continue
        t = (m.group(4) or m.group(3))
        if t.startswith('unsigned') or t in ('u8', 'u16', 'u32', 'u64', '_Bool'):
            if t == '_Bool':
                continue
            hits.append((cur_file, cur_line, m.group(1) + ' ' + m.group(3)))
    return entry['file'], hits


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--build', required=True)
    ap.add_argument('-j', type=int, default=4)
    ap.add_argument('files', nargs='*')
    a = ap.parse_args()
    cc = json.load(open(os.path.join(a.build, 'compile_commands.json')))
    want = {os.path.abspath(f) for f in a.files}
    todo = [e for e in cc if e['file'].endswith('.c') and (not want or os.path.abspath(e['file']) in want)
            and ('/src/' in e['file'] or '/port/game/' in e['file'])]
    seen = set()
    n = 0
    with ThreadPoolExecutor(a.j) as ex:
        for f, hits in ex.map(scan, todo):
            for h in hits:
                key = (os.path.relpath(h[0]) if h[0] != 'timeout' else f, h[1], h[2])
                if key in seen:
                    continue
                seen.add(key)
                n += 1
                print('%s:%s: %s' % key)
    print('f2ucheck: %d float -> unsigned conversions in %d files' % (n, len(todo)), file=sys.stderr)


if __name__ == '__main__':
    main()
