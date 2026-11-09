#!/usr/bin/env python3
"""64-bit hazard census of the game C (docs/port/64BIT.md).

  python3 tools/port/audit64.py [--out DIR] [--jobs N]

1. Compiles every src/us/code/*.c with clang -fsyntax-only for x86_64 and only the
   pointer-width warnings enabled (-Wpointer-to-int-cast, -Wint-to-pointer-cast,
   -Wvoid-pointer-to-int-cast, -Wint-to-void-pointer-cast, -Wint-conversion,
   -Wshorten-64-to-32). The include tree is copied to OUT/inc first and the
   `sizeof(T *)` pads of include/rw/abu_types.h are pinned to 4 (they go negative on
   a 64-bit host and stop 1,000 files). Nothing in the repo is changed.
2. Classifies each warning (pattern on the source line) and maps it to the enclosing
   function and its subsystem (CClass__ / Prefix_ of the name). `_2` copies (second VM
   module) are counted apart.
3. Compares every function definition with every other declaration of it (local
   prototypes in .c files, include/): pointer on one side, integer on the other.
"""
import argparse, collections, glob, json, os, re, shutil, subprocess, sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
CODE = os.path.join(ROOT, 'src/us/code')
WFLAGS = ['-DTARGET_PORT=1', '-Wno-everything', '-Wno-error=int-conversion', '-Wint-conversion', '-Wpointer-to-int-cast',
          '-Wint-to-pointer-cast', '-Wvoid-pointer-to-int-cast', '-Wint-to-void-pointer-cast',
          '-Wshorten-64-to-32', '-fno-caret-diagnostics', '-ferror-limit=0']


def sweep(out, jobs):
    inc = os.path.join(out, 'inc')
    shutil.rmtree(inc, ignore_errors=True)
    shutil.copytree(os.path.join(ROOT, 'include'), inc)
    p = os.path.join(inc, 'rw/abu_types.h')
    t = open(p).read()
    open(p, 'w').write(re.sub(r'sizeof\(([A-Za-z_ ]+)\*\)', '4', t))
    files = sorted(f for f in os.listdir(CODE) if f.endswith('.c'))
    wdir = os.path.join(out, 'w')
    os.makedirs(wdir, exist_ok=True)
    cmd = ('cd %s && printf "%%s\n" "$@" | xargs -P%d -I{} sh -c '
           '\x27clang -fsyntax-only -target x86_64-linux-gnu %s -I%s -I%s {} > %s/{}.txt 2>&1; echo "$?" >> %s/{}.txt\x27')
    subprocess.run(['sh', '-c', cmd % (CODE, jobs, ' '.join(WFLAGS), ROOT, inc, wdir, wdir), 'sh'] + files, check=True)
    lines = []
    for f in files:
        t = open(os.path.join(wdir, f + '.txt'), errors='replace').read().rstrip('\n').split('\n')
        lines += t[:-1] + ['@@RC %s %s' % (t[-1], f)]
    return lines


FDEF = re.compile(r'^(?!static\s+inline)[A-Za-z_][\w\s\*]*?\b(\w+)\s*\(([^;]*)$')
_cache = {}


def enclosing(fn, line):
    if fn not in _cache:
        lines = open(os.path.join(CODE, fn), errors='replace').read().split('\n')
        cur, name = [], None
        for l in lines:
            m = FDEF.match(l)
            if m and not l.rstrip().endswith(';') and m.group(1) not in ('if', 'while', 'for', 'switch', 'return'):
                name = m.group(1)
            cur.append(name)
        _cache[fn] = (cur, lines)
    cur, lines = _cache[fn]
    return cur[line - 1] or '?', lines[line - 1].strip()


def subsys(n):
    if n.startswith('func_') or n == '?':
        return 'unnamed'
    b = n[:-2] if n.endswith('_2') else n
    if '__' in b and not b.startswith('_'):
        return b.split('__')[0]
    if b.startswith(('al', '__CSP', '__al', 'n_al')):
        return 'libaudio'
    if b.startswith(('os', '__os', '__ll', 'gu')):
        return 'libultra'
    if b.startswith(('adec', 'mdct')):
        return 'adec'
    if b.startswith('_'):
        return '_internal'
    return b.split('_')[0] + '_' if '_' in b else b


def classify(w, t, name):
    if re.search(r'\b(GFX|GFX_W|GFX_CMD|gSP\w*|gDP\w*|_gbi|Acmd)\b|w1\s*=|->words\.w1', t):
        return 'gbi/acmd word holds an address'
    if re.search(r'0x[Aa]0000000|0x80000000|osVirtualToPhysical|0x1FFFFFFF', t):
        return 'KSEG/physical address math'
    if re.search(r'CList__|m_LastOffset|m_NextOffset|m_pHead|m_pTail', t) or name.startswith('CList__'):
        return 'CList intrusive links'
    if re.search(r'CISet__|CUSet__', t):
        return 'CISet/CUSet offset tables'
    if w == '-Wint-conversion' and re.search(r'\w+\s*\(', t):
        return 'call arg/return type mismatch (implicit)'
    if (re.search(r'\(\s*[su]32\s*\)\s*&?\w[\w>.\[\]-]*\s*[,)]', t) and re.search(r'\w+\s*\(', t)
            and w in ('-Wpointer-to-int-cast', '-Wvoid-pointer-to-int-cast')):
        return 'pointer passed as s32 argument (cast)'
    if (re.search(r'\(\s*[su]32\s*\)[^;]*[-+]', t) or re.search(r'\(\s*\w+\s*\*\s*\)\s*\(\s*\(?[su]32\)?[^;]*[-+]', t)
            or re.search(r'\*\s*\(\s*\w+\s*\*\s*\)\s*\(\w+\s*[-+]', t)):
        return 'pointer arithmetic through an integer'
    if re.search(r'(<|>|<=|>=|==|!=)\s*\(?\s*\(?[su]32\)', t) or re.search(r'\([su]32\)[^;]*(<|>|<=|>=)', t):
        return 'pointer compare/range check as integer'
    if re.search(r'(->|\.)\w+(\[[^]]*\])?\s*=', t):
        return 'pointer stored in/loaded from an int field'
    return 'other (locals, returns, misc casts)'


# pointer-sized integers (include/common.h): as wide as a pointer on the host
WIDE = {'sptr', 'uptr', 'intptr_t', 'uintptr_t'}
INT = {'s32', 'u32', 'int', 'unsigned', 'long', 's16', 'u16', 's8', 'u8', 'OSPri', 'OSId'}


def kind(t):
    t = t.strip()
    if not t or t == 'void':
        return 'v'
    if '*' in t or '[' in t or '(' in t:
        return 'P'
    toks = [x for x in t.split() if x not in ('const', 'volatile', 'register', 'extern', 'static', 'struct', 'signed')]
    if not toks:
        return '?'
    if toks[0] in WIDE:
        return 'W'
    if toks[0] in INT:
        return 'I'
    if toks[0] in ('f32', 'f64', 'float', 'double'):
        return 'F'
    return 'O'


def ptype(p):
    p = p.strip()
    if p in ('', 'void', '...'):
        return None
    if '(' in p:
        return 'P'
    toks = p.replace('*', ' * ').split()
    if len(toks) >= 2 and re.match(r'^\w+$', toks[-1]) and toks[-1] not in INT | WIDE | {'f32', 'f64', 'float', 'double'}:
        toks = toks[:-1]
    return kind(' '.join(toks) + ('*' if '[' in p else ''))


SIG = re.compile(r'(?:^|[;}\n])\s*((?:extern\s+|static\s+)?[A-Za-z_][\w\s\*]*?)\b([A-Za-z_]\w*)\s*\(([^()]*(?:\([^()]*\)[^()]*)*)\)\s*([;{])', re.M)


def prototypes():
    files = sorted(glob.glob(CODE + '/*.c') + glob.glob(ROOT + '/src/us/libultra/*.c'))
    files += glob.glob(ROOT + '/include/**/*.h', recursive=True)
    # include/functions.h is generated (gitignored) and included by no C file
    files = [f for f in files if not f.endswith('include/functions.h')]
    defs, decls = {}, collections.defaultdict(list)
    for f in files:
        s = open(f, errors='replace').read()
        s = re.sub(r'//[^\n]*', '', re.sub(r'/\*.*?\*/', ' ', s, flags=re.S))
        for m in SIG.finditer(s):
            ret, name, params, end = m.groups()
            if name in ('if', 'while', 'for', 'switch', 'return', 'sizeof') or re.search(r'\b(return|else|case)\b|[=+\-]', ret):
                continue
            if 'static' in ret and end == '{':
                continue
            r = 'P' if '*' in ret else kind(ret)
            ps = [ptype(p) for p in re.split(r',(?![^(]*\))', params)] if params.strip() not in ('', 'void') else []
            rec = (r, ps, os.path.relpath(f, ROOT))
            if end == '{':
                defs[name] = rec
            else:
                decls[name].append(rec)
    return defs, decls


# a 32-bit integer on one side, a pointer (or a pointer-sized integer) on the other
NARROW = ({'P', 'I'}, {'W', 'I'})


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--out', default=os.path.join(ROOT, 'port/build/audit64'))
    ap.add_argument('--jobs', type=int, default=os.cpu_count() or 4)
    a = ap.parse_args()
    os.makedirs(a.out, exist_ok=True)
    lines = sweep(a.out, a.jobs)
    rc = collections.Counter()
    rows, seen = [], set()
    for l in lines:
        if l.startswith('@@RC'):
            rc['ok' if l.split()[1] == '0' else 'error'] += 1
            continue
        m = re.match(r'([^:\s]+\.c):(\d+):(\d+): warning: (.*) \[(-W[\w-]+)\]', l)
        if not m or l in seen:
            continue
        seen.add(l)
        fn, ln, col, msg, w = m.groups()
        name, text = enclosing(fn, int(ln))
        rows.append(dict(file=fn, line=int(ln), func=name, subsystem=subsys(name), warning=w,
                         category=classify(w, text, name), text=text))
    json.dump(rows, open(os.path.join(a.out, 'warnings.json'), 'w'), indent=0)
    main_rows = [r for r in rows if not r['func'].endswith('_2')]
    print('files: %d without errors, %d with errors (not pointer-width ones)' % (rc['ok'], rc['error']))
    print('pointer-width warnings: %d (%d in second-module _2 copies)' % (len(rows), len(rows) - len(main_rows)))
    print('\nby flag:')
    for k, v in collections.Counter(r['warning'] for r in rows).most_common():
        print('  %5d %s' % (v, k))
    print('\nby category (first-module code):')
    for k, v in collections.Counter(r['category'] for r in main_rows).most_common():
        print('  %5d %s' % (v, k))
    defs, decls = prototypes()
    mism = collections.Counter()
    ret = set()
    for n, (r, ps, f) in defs.items():
        for (r2, ps2, f2) in decls.get(n, []):
            bad = {r, r2} in NARROW
            if bad:
                ret.add(n)
            if bad or any({x, y} in NARROW for x, y in zip(ps, ps2)):
                mism[n] += 1
    print('\nprototype census: %d definitions; %d functions have a declaration elsewhere that puts an integer '
          'where the definition has a pointer (or the reverse): %d declarations, %d of them on the return type'
          % (len(defs), len(mism), sum(mism.values()), len(ret)))
    w = collections.Counter(r['subsystem'] for r in main_rows)
    mm = collections.Counter(subsys(n) for n in mism if not n.endswith('_2'))
    print('\nby subsystem: warnings, mismatching functions')
    for k in sorted(set(w) | set(mm), key=lambda k: -(w[k] + mm[k]))[:45]:
        print('  %-26s %5d %5d' % (k, w[k], mm[k]))
    json.dump({n: dict(count=c, definition=defs[n][2]) for n, c in mism.items()},
              open(os.path.join(a.out, 'prototype_mismatches.json'), 'w'), indent=0)
    print('\nwritten: %s/warnings.json, prototype_mismatches.json' % a.out)


if __name__ == '__main__':
    main()
