#!/usr/bin/env python3
"""Canonical prototypes generated from the function DEFINITIONS (docs/port/64BIT.md).

Every matched C file carries its own prototypes, written when it was matched; on a
64-bit host a local `s32 f(s32)` for a function that takes and returns pointers
truncates at the call. This tool writes one header per subsystem,
include/protos/<group>.h, from the definitions in src/us/code, and moves the
callers onto it:

  python3 tools/port/genprotos.py gen   [GROUP ...]   write include/protos/<group>.h
  python3 tools/port/genprotos.py apply [GROUP ...]   gen, then in every file that names a
        function of the group: drop its file-scope declarations of those functions and
        include the header. Each edited file goes through tools/asm_diff.py (--base, default
        HEAD); files whose N64 code changes or that stop compiling are put back as they were
        (GCC 2.8's code can depend on a callee's prototype: an s16 parameter, a float
        argument, a signed vs unsigned return compared), and listed.
  python3 tools/port/genprotos.py list  [GROUP ...]   show what the headers would declare

The header keeps the definition's types where they are self-contained (common.h
scalars, sptr/uptr, void, struct tags); a typedef'd struct becomes `struct Tag *`
with a forward declaration (so the header pulls in no struct header and cannot
clash with a file's local struct definitions); a pointer to a type the header
cannot name (a struct local to the defining file, an anonymous typedef) becomes
`void *`. A function whose by-value parameter cannot be named is left out
(reported). Second-module copies (`_2`) are left out: module 2 is not linked on
the host.
"""
import argparse, collections, glob, os, re, subprocess, sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
CODE = os.path.join(ROOT, 'src/us/code')
OUT = os.path.join(ROOT, 'include/protos')

# group -> (name regex, comment)
GROUPS = collections.OrderedDict([
    ('list', (r'CList__\w+', 'CList, the intrusive list (func_00200500..)')),
    ('iset', (r'C[IU]Set__\w+', 'CISet / CUSet, offset-table containers')),
    ('cache', (r'CCache__\w+', 'CCache, the memory / ROM cache')),
    ('scene', (r'CScene__\w+', 'CScene, the level world')),
    ('ai', (r'CAI__SetMode|CAI__CallHurt|AI_PickAnimFromTable|AI_TurnClamped|AI_PlayAnim',
            'AI helpers called from every enemy')),
    ('menu', (r'MenuPool_OpenMenu|MenuPool_OpenChildMenu|Menu_StepValue', 'menu pool')),
    ('player', (r'CPlayerPool__GetIndex|CPlayerPool__GetFirstActive|CPlayer__StartDeathCinema|CPlayer__SetState',
                'player pool / player state')),
    ('game', (r'LoopSounds_Get|CParticleSystem__Spawn|GameObject_FireTriggers|Controller_SetRumbleOption|'
              r'ImpactEvent_ApplyBlast|CInstancePool__Spawn|Render_DrawModel|GameObject_GetRegionFloorHeight',
              'other functions with many disagreeing declarations')),
])

SCALAR = {'s8', 'u8', 's16', 'u16', 's32', 'u32', 's64', 'u64', 'f32', 'f64', 'sptr', 'uptr', 'void',
          'char', 'short', 'int', 'long', 'float', 'double', 'unsigned', 'signed'}
# scalar typedefs of ultra_types.h, by their underlying common.h type
ALIAS = {'OSPri': 's32', 'OSId': 's32', 'OSTime': 'u64', 'OSMesg': 'void *', 'OSIntMask': 'u32',
         'OSEvent': 's32', 'size_t': 'u32', 'uintptr_t': 'uptr', 'intptr_t': 'sptr'}
QUAL = {'const', 'volatile', 'register', 'struct', 'union', 'enum'}


def strip_comments(s):
    s = re.sub(r'/\*.*?\*/', lambda m: re.sub(r'[^\n]', ' ', m.group(0)), s, flags=re.S)
    return re.sub(r'//[^\n]*', '', s)


def struct_tags():
    """typedef name -> (kind, tag) for every typedef'd struct/union with a tag in include/."""
    tags = {}
    for f in glob.glob(os.path.join(ROOT, 'include/**/*.h'), recursive=True):
        if f.endswith('include/functions.h') or '/protos/' in f:
            continue
        s = strip_comments(open(f, errors='replace').read())
        for m in re.finditer(r'typedef\s+(struct|union)\s+(\w+)\s+(\w+)\s*;', s):
            tags.setdefault(m.group(3), (m.group(1), m.group(2)))
        for m in re.finditer(r'typedef\s+(struct|union)\s+(\w+)\s*\{', s):
            depth, i = 0, m.end() - 1
            while i < len(s):
                depth += {'{': 1, '}': -1}.get(s[i], 0)
                if depth == 0:
                    break
                i += 1
            n = re.match(r'\s*(\w+)\s*;', s[i + 1:])
            if n:
                tags.setdefault(n.group(1), (m.group(1), m.group(2)))
    return tags


DEF = re.compile(r'^([A-Za-z_][\w \t\*]*?)\b([A-Za-z_]\w*)\s*\(([^;{}()]*(?:\([^()]*\)[^;{}()]*)*)\)\s*\{', re.M)


def definitions():
    defs = {}
    for f in sorted(glob.glob(CODE + '/*.c')):
        s = strip_comments(open(f, errors='replace').read())
        for m in DEF.finditer(s):
            ret, name, params = m.groups()
            if re.search(r'\b(static|inline|return|else)\b', ret) or name in ('if', 'while', 'for', 'switch'):
                continue
            defs.setdefault(name, (' '.join(ret.split()), ' '.join(params.split()), os.path.relpath(f, ROOT)))
    return defs


def split_params(p):
    out, depth, cur = [], 0, ''
    for ch in p:
        if ch == ',' and depth == 0:
            out.append(cur.strip())
            cur = ''
            continue
        depth += {'(': 1, ')': -1}.get(ch, 0)
        cur += ch
    if cur.strip():
        out.append(cur.strip())
    return out


class Unnamed(Exception):
    pass


def canon_type(t, tags, fwd):
    """'CMemEntry *entry' (name dropped by the caller) -> canonical text."""
    t = ' '.join(t.replace('*', ' * ').split())
    stars = t.count('*')
    toks = [x for x in t.replace('*', ' ').split() if x not in ('const', 'volatile', 'register')]
    kw = None
    if toks and toks[0] in ('struct', 'union', 'enum'):
        kw, toks = toks[0], toks[1:]
    if kw in ('struct', 'union'):
        fwd.add('%s %s' % (kw, toks[0]))
        return '%s %s%s' % (kw, toks[0], ' ' + '*' * stars if stars else '')
    if kw == 'enum':
        if stars:
            return 'void *'
        return 's32'
    if all(x in SCALAR for x in toks):
        return ' '.join(toks) + (' ' + '*' * stars if stars else '')
    if len(toks) == 1 and toks[0] in ALIAS:
        base = ALIAS[toks[0]]
        return base + (' ' + '*' * stars if stars else '') if '*' not in base or not stars else 'void *'
    if len(toks) == 1 and toks[0] in tags:
        k, tag = tags[toks[0]]
        fwd.add('%s %s' % (k, tag))
        return '%s %s%s' % (k, tag, ' ' + '*' * stars if stars else '')
    if stars:
        return 'void *'
    raise Unnamed(t)


def drop_name(p):
    p = p.strip()
    m = re.match(r'^(.*?)(\w+)\s*((?:\[[^\]]*\])*)$', p)
    if not m or not m.group(1).strip() or m.group(1).strip() in ('struct', 'union', 'enum'):
        return p
    typ, arr = m.group(1), m.group(3)
    if arr:
        typ += '*'
    if re.fullmatch(r'[\w\s]*', typ) and len(typ.split()) == 1 and m.group(2) in SCALAR:
        return p  # 'unsigned int': no name
    return typ


def prototype(name, ret, params, tags, fwd):
    r = canon_type(ret, tags, fwd)
    if params.strip() in ('', 'void'):
        ps = ['void']
    else:
        ps = []
        for p in split_params(params):
            if p == '...':
                ps.append(p)
            elif '(' in p:   # function pointer parameter
                ps.append('void *')
            else:
                ps.append(canon_type(drop_name(p), tags, fwd))
    return '%s%s%s(%s);' % (r, '' if r.endswith('*') else ' ', name, ', '.join(ps))


def gen(groups, verbose=True):
    defs, tags = definitions(), struct_tags()
    os.makedirs(OUT, exist_ok=True)
    result = {}
    for g in groups:
        rx, comment = GROUPS[g]
        names = sorted(n for n in defs if re.fullmatch(rx, n) and not n.endswith('_2'))
        fwd, lines, skipped = set(), [], []
        for n in names:
            ret, params, f = defs[n]
            try:
                lines.append(prototype(n, ret, params, tags, fwd))
            except Unnamed as e:
                skipped.append('%s (%s)' % (n, e))
        guard = 'PROTOS_%s_H' % g.upper()
        txt = ['/* %s.' % comment, ' *',
               ' * Canonical prototypes, generated from the definitions by tools/port/genprotos.py',
               ' * (do not edit; re-run it). Types the header cannot name without a struct header',
               ' * are struct tags or void *. */', '',
               '#ifndef %s' % guard, '#define %s' % guard, '', '#include "common.h"', '']
        txt += ['%s;' % x for x in sorted(fwd)] + ([''] if fwd else [])
        txt += lines + ['', '#endif', '']
        open(os.path.join(OUT, g + '.h'), 'w').write('\n'.join(txt))
        result[g] = [n for n in names if not any(s.startswith(n + ' ') for s in skipped)]
        if verbose:
            print('%-7s %3d prototypes%s' % (g, len(lines), (', left out: ' + '; '.join(skipped)) if skipped else ''))
    return result


def decl_re(names):
    alt = '|'.join(sorted(names, key=len, reverse=True))
    # a file-scope declaration statement (column 0), possibly over several lines
    return re.compile(r'^(?:extern\s+)?[A-Za-z_][\w \t\*]*?\b(?:%s)\s*\([^;{}]*\)\s*;[ \t]*(?:/\*[^\n]*?\*/|//[^\n]*)?[ \t]*\n'
                      % alt, re.M)


# a file that spells out the common.h scalar typedefs itself (imported code) gets common.h's
LOCAL_SCALARS = re.compile(r'^typedef\s+[\w ]+?\s+(?:s8|u8|s16|u16|s32|u32|s64|u64|f32|f64)\s*;[^\n]*\n', re.M)


def edit(text, names, header):
    rx = decl_re(names)
    new = LOCAL_SCALARS.sub('', rx.sub('', text))
    inc = '#include "protos/%s.h"\n' % header
    if inc in new:
        return new
    incs = list(re.finditer(r'^#include[^\n]*\n', new, re.M))
    first_code = re.search(r'^[A-Za-z_][^\n]*[;{(]', strip_comments(new), re.M)
    incs = [m for m in incs if not first_code or m.start() < first_code.start()]
    if incs:
        pos = incs[-1].end()
    else:
        m = re.match(r'(?:\s*/\*.*?\*/)*\s*\n?', new, re.S)
        pos = m.end() if m else 0
    return new[:pos] + inc + new[pos:]


def apply(groups, base):
    names_by = gen(groups)
    for g in groups:
        names = names_by[g]
        if not names:
            continue
        call = re.compile(r'\b(?:%s)\s*\(' % '|'.join(names))
        static = re.compile(r'\bstatic\b[^;{]*\b(?:%s)\s*\(' % '|'.join(names))
        orig = {}
        for f in sorted(glob.glob(CODE + '/*.c')):
            s = open(f, errors='replace').read()
            if not call.search(strip_comments(s)) or static.search(s):
                continue
            n = edit(s, names, g)
            if n != s:
                orig[f] = s
                open(f, 'w').write(n)
        if not orig:
            print('%-7s no file to edit' % g)
            continue
        r = subprocess.run([sys.executable, os.path.join(ROOT, 'tools/asm_diff.py'), '--base', base, '-j', '24']
                           + sorted(orig), capture_output=True, text=True, cwd=ROOT)
        bad = set()
        for l in r.stdout.splitlines():
            m = re.match(r'(DIFF|FAIL) (\S+)', l)
            if m:
                bad.add(os.path.join(ROOT, m.group(2)))
        for f in bad:
            open(f, 'w').write(orig[f])
        print('%-7s %4d files include it, %3d put back (N64 code would change: %s)'
              % (g, len(orig) - len(bad), len(bad),
                 ' '.join(sorted(os.path.basename(f) for f in bad))[:600] or '-'))


def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('cmd', choices=('gen', 'apply', 'list'))
    ap.add_argument('groups', nargs='*')
    ap.add_argument('--base', default='HEAD', help='asm_diff base revision (apply)')
    a = ap.parse_args()
    groups = a.groups or list(GROUPS)
    for g in groups:
        if g not in GROUPS:
            sys.exit('unknown group %s (%s)' % (g, ', '.join(GROUPS)))
    if a.cmd == 'gen':
        gen(groups)
    elif a.cmd == 'list':
        gen(groups)
        for g in groups:
            print(open(os.path.join(OUT, g + '.h')).read())
    else:
        apply(groups, a.base)


if __name__ == '__main__':
    main()
