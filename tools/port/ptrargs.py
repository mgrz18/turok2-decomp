#!/usr/bin/env python3
"""ptrargs.py BUILD_LOG [--apply] [--only NAMES]: the other half of ptrdecls.py. From a port build
log (clang -Wint-conversion), every call that passes a pointer to a parameter declared s32/u32:
the note names the declaration's parameter. The function's parameter i is then retyped sptr/uptr
in its DEFINITION(S) and in every top-level declaration (the same types on N64: matching-safe).
Rebuild the game library from clean first so the log has every file's warnings."""
import re, sys, collections, os
ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))) + '/'
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
log = open(sys.argv[1]).read().splitlines()
apply = '--apply' in sys.argv
only = set(sys.argv[sys.argv.index('--only') + 1].split(',')) if '--only' in sys.argv else None

src_cache = {}
def text(f):
    if f not in src_cache:
        src_cache[f] = open(f, errors='replace').read()
    return src_cache[f]

def locate(f, ln, col):
    """function name and parameter index of the declaration parameter at f:ln:col"""
    s = text(f)
    lines = s.split('\n')
    pos = sum(len(l) + 1 for l in lines[:ln - 1]) + col - 1
    d, i, idx = 0, pos, 0
    while i > 0:
        i -= 1
        c = s[i]
        if c == ')': d += 1
        elif c == '(':
            if d == 0: break
            d -= 1
        elif c == ',' and d == 0: idx += 1
    m = re.search(r'([A-Za-z_]\w*)\s*$', s[:i])
    return (m.group(1) if m else None), idx

want = collections.defaultdict(set)
for k, l in enumerate(log):
    if 'incompatible pointer to integer conversion passing' not in l: continue
    if not re.search(r"to parameter of type '(s32|u32|int|unsigned int)'", l): continue
    for n in log[k + 1:k + 8]:
        m = re.match(r'(/\S+):(\d+):(\d+): note: passing argument to parameter', n)
        if m:
            name, idx = locate(m.group(1), int(m.group(2)), int(m.group(3)))
            if name and (not only or name in only):
                want[name].add(idx)
            break

import glob
files0 = sorted(glob.glob(ROOT + 'src/**/*.c', recursive=True) + glob.glob(ROOT + 'include/**/*.h', recursive=True))
# definitions whose parameter i is an int while a declaration in scope somewhere has a pointer there
def _strip(s):
    return re.sub(r'/\*.*?\*/|//[^\n]*', lambda m: re.sub(r'[^\n]', ' ', m.group(0)), s, flags=re.S)
DEF = re.compile(r'(?m)^([A-Za-z_][\w \t\*]*?)\b([A-Za-z_]\w*)\s*\(([^;{}()]*)\)\s*\{')
DECL = re.compile(r'(?m)^[ \t]*(?:extern\s+)?[A-Za-z_][\w \t\*]*?\b([A-Za-z_]\w*)\s*\(([^;{}()]*)\)\s*;')
INTP = re.compile(r'^\s*(signed\s+int|unsigned\s+int|s32|u32|int)(\s+\w+)?\s*$')
defp = collections.defaultdict(list); declp = collections.defaultdict(list)
for f in files0:
    c = _strip(text(f))
    if f.endswith('.c'):
        for m in DEF.finditer(c):
            if m.group(1).strip().split()[0] in ('if', 'while', 'for', 'switch', 'return', 'else'): continue
            defp[m.group(2)].append([x.strip() for x in m.group(3).split(',')])
    for m in DECL.finditer(c):
        declp[m.group(1)].append([x.strip() for x in m.group(2).split(',')])
for name, dl in defp.items():
    if only and name not in only: continue
    for dps in dl:
        for decl in declp.get(name, []):
            if len(decl) != len(dps): continue
            for i, (a, b) in enumerate(zip(dps, decl)):
                if INTP.match(a) and '*' in b:
                    want[name].add(i)
files = sorted(glob.glob(ROOT + 'src/**/*.c', recursive=True) + glob.glob(ROOT + 'include/**/*.h', recursive=True))
def split_params(p):
    parts, d, cur = [], 0, ''
    for c in p:
        if c == '(': d += 1
        if c == ')': d -= 1
        if c == ',' and d == 0: parts.append(cur); cur = ''
        else: cur += c
    parts.append(cur)
    return parts
def retype(p):
    q = re.sub(r'^(\s*)(signed\s+int|s32|int)\b', r'\1sptr', p)
    return re.sub(r'^(\s*)(unsigned\s+int|u32)\b', r'\1uptr', q)

for k in [k for k in want if k in ('memset', 'memcpy', 'bzero', 'bcopy') or k.startswith(('os', '__os'))]:
    del want[k]   # libultra/libc: the port runtime provides them
total = 0
names = '|'.join(sorted(want, key=len, reverse=True))
if names:
    PAT = re.compile(r'\b(' + names + r')\s*\(([^;{}()]*)\)(?=\s*[;,{])')
    for f in files:
        s = text(f); out = []; last = 0; changed = False
        for m in PAT.finditer(s):
            name = m.group(1)
            # declarations/definitions only: at top level (line starts with a type, not inside a call)
            ls = s.rfind('\n', 0, m.start()) + 1
            head = s[ls:m.start()]
            if not re.match(r'^(extern\s+|static\s+|volatile\s+)*[A-Za-z_][\w\s\*]*$', head) or head.startswith((' ', '\t')) and 'extern' not in head:
                continue
            if re.match(r'^\s*(return|else|case)\b', head): continue
            ps = split_params(m.group(2))
            new = list(ps)
            for i in want[name]:
                if i < len(ps) and re.match(r'^\s*(signed\s+int|unsigned\s+int|s32|u32|int)(\s+\w+)?\s*$', ps[i]):
                    new[i] = retype(ps[i])
            if new != ps:
                out.append(s[last:m.start(2)]); out.append(','.join(new)); last = m.end(2); changed = True
                total += sum(1 for a, b in zip(ps, new) if a != b)
                print(f'{f[len(ROOT):]}: {name} {[i for i,(a,b) in enumerate(zip(ps,new)) if a!=b]}')
        if changed and apply:
            out.append(s[last:])
            open(f, 'w').write(''.join(out))
print('functions', len(want), 'parameters retyped', total)
