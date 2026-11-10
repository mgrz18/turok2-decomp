#!/usr/bin/env python3
"""ptrdecls.py [--apply] [--only NAMES]: top-level declarations (prototypes, in src/ and include/)
whose parameter i is s32/u32/int while the function's DEFINITION has a pointer (or sptr/uptr)
there; and declared s32 returns of definitions returning a pointer. --apply retypes the declared
s32 -> sptr, u32 -> uptr (the same types on N64: matching-safe)."""
import re, sys, glob, collections
import os
ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))) + '/'
apply = '--apply' in sys.argv
only = None
if '--only' in sys.argv:
    only = set(sys.argv[sys.argv.index('--only') + 1].split(','))
files = sorted(glob.glob(ROOT + 'src/**/*.c', recursive=True) + glob.glob(ROOT + 'include/**/*.h', recursive=True))

def strip_comments(s):
    return re.sub(r'/\*.*?\*/|//[^\n]*', lambda m: re.sub(r'[^\n]', ' ', m.group(0)), s, flags=re.S)

def depth_map(s):
    d = 0; out = bytearray(len(s))
    for i, c in enumerate(s):
        if c == '{': d += 1
        out[i] = min(d, 255)
        if c == '}': d -= 1
    return out

def split_params(p):
    parts, d, cur = [], 0, ''
    for c in p:
        if c == '(' : d += 1
        if c == ')' : d -= 1
        if c == ',' and d == 0: parts.append(cur.strip()); cur = ''
        else: cur += c
    if cur.strip(): parts.append(cur.strip())
    return parts

def is_ptr(p):
    return '*' in p or re.match(r'^(const\s+)?(sptr|uptr)\b', p) is not None
INT = re.compile(r'^(signed\s+int|unsigned\s+int|s32|u32|int)(\s+\w+)?$')

defs = collections.defaultdict(list)   # name -> list of (params, rettype)
DEF = re.compile(r'(?m)^([A-Za-z_][\w \t\*]*?)\b([A-Za-z_]\w*)\s*\(([^;{}()]*(?:\([^()]*\)[^;{}()]*)*)\)\s*\{')
texts = {}
for f in files:
    s = open(f, errors='replace').read(); texts[f] = s
    if not f.endswith('.c'): continue
    c = strip_comments(s)
    for m in DEF.finditer(c):
        ret = m.group(1).strip()
        if ret.split()[0] in ('if', 'while', 'for', 'switch', 'return', 'else', 'do'): continue
        defs[m.group(2)].append((split_params(m.group(3)), ret))

DECL = re.compile(r'\b([A-Za-z_]\w*)\s*\(([^;{}()]*)\)\s*(?=[;,])')
changes = collections.Counter(); total = 0
for f in files:
    s = texts[f]; c = strip_comments(s); dm = depth_map(c)
    edits = []
    for m in DECL.finditer(c):
        name = m.group(1)
        if name not in defs or dm[m.start()] != 0: continue
        if only and name not in only: continue
        # must look like a declaration: preceded by a type on the same statement
        pre = c[max(0, c.rfind(';', 0, m.start()), c.rfind('}', 0, m.start()), c.rfind('\n#', 0, m.start())) : m.start()]
        if '=' in pre or '#' in pre.strip()[:1]: continue
        dps = split_params(m.group(2))
        if dps in ([], ['void']): continue
        for dparams, dret in defs[name]:
            if len(dparams) != len(dps): continue
            for i, (dp, pp) in enumerate(zip(dps, dparams)):
                mm = INT.match(dp)
                if mm and is_ptr(pp):
                    edits.append((m.start(2), m.end(2), i, name))
            break
    if not edits: continue
    # rebuild params text for each decl
    out = s; seen = set()
    for st, en, i, name in sorted(set(edits), key=lambda e: (-e[0], -e[2])):
        params = split_params(out[st:en])
        p = params[i]
        np = re.sub(r'^(signed\s+int|s32|int)\b', 'sptr', p)
        np = re.sub(r'^(unsigned\s+int|u32)\b', 'uptr', np)
        if np == p: continue
        params[i] = np
        out = out[:st] + ', '.join(params) + out[en:]
        changes[name] += 1; total += 1
        print(f'{f[len(ROOT):]}: {name} param {i}: {p!r} -> {np!r}')
    if apply and out != s:
        open(f, 'w').write(out)
# declared int returns of definitions that return a pointer
RD = re.compile(r'(?m)^([ \t]*(?:extern\s+)?(?:volatile\s+)?)(s32|u32|int|unsigned int|signed int)(\s+([A-Za-z_]\w*)\s*\([^;{}]*\)\s*;)')
rtotal = 0
for f in files:
    s = texts[f] if not apply else open(f, errors='replace').read()
    def fix(m):
        global rtotal
        name = m.group(4)
        if only and name not in only: return m.group(0)
        if name in defs and any('*' in r or r.split()[-1] in ('sptr', 'uptr') for p, r in defs[name]):
            rtotal += 1
            print(f'{f[len(ROOT):]}: {name} return: {m.group(2)!r}')
            return m.group(1) + ('uptr' if m.group(2) in ('u32', 'unsigned int') else 'sptr') + m.group(3)
        return m.group(0)
    out = RD.sub(fix, s)
    if apply and out != s:
        open(f, 'w').write(out)
print('total', total, 'functions', len(changes), 'returns', rtotal)
