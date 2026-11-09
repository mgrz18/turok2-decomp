#!/usr/bin/env python3
"""Retype the integers that carry pointers in the game C (docs/port/64BIT.md, part 2).

  python3 tools/port/retype.py [--base REV] [-j N] [--rules ACD] [--dry] [FILE ...]

Compiles each file with clang for x86_64 (the flags of tools/port/audit64.py), reads the
pointer-width warnings with their columns and rewrites the source at those sites with
edits that cannot change the N64 code by construction (sptr/uptr ARE s32/u32 there), or
that should not (a cast dropped where the callee's prototype already takes the pointer):

  D  `(s32) ptr` / `(u32) ptr` / `(int) ptr` casts that truncate -> `(sptr)` / `(uptr)`
  C  a local `s32 x` / `u32 x` that receives a pointer, or is cast to a pointer, or is
     passed to a pointer parameter, or receives an sptr -> `sptr x` / `uptr x`
     (a shared declaration `s32 a, x, b;` keeps its place: `s32 a, b;` + `sptr x;`
     right after it)
  A  `f(..., (s32) &x, ...)` where f's parameter is a pointer and the operand is a plain
     lvalue / address: the cast is dropped

and repeats until nothing changes (C feeds on the warnings D creates). Then every edited
file goes through tools/asm_diff.py; a file whose N64 code changes or that no longer
compiles is retried with rule D alone and, failing that, put back. Files with a `_2`
suffix (module 2, not linked on the host) are skipped unless named.
"""
import argparse, collections, glob, os, re, shutil, subprocess, sys, tempfile
from concurrent.futures import ThreadPoolExecutor

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
CODE = os.path.join(ROOT, 'src/us/code')
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from audit64 import WFLAGS  # noqa: E402

DIAG = re.compile(r'^(?:.*/)?([\w.]+\.c):(\d+):(\d+): warning: (.*) \[(-W[\w-]+)\]$')
CAST32 = re.compile(r'\(\s*(s32|u32|int|unsigned int|unsigned|signed int)\s*\)')
NARROW = {'s32': 'sptr', 'int': 'sptr', 'signed int': 'sptr', 'u32': 'uptr', 'unsigned': 'uptr',
          'unsigned int': 'uptr'}
INTT = r"'(?:s32|u32|int|unsigned int)'"
FDEF = re.compile(r'^(?!static\s+inline)[A-Za-z_][\w\s\*]*?\b(\w+)\s*\(([^;]*)$')
KEYWORDS = {'if', 'while', 'for', 'switch', 'return', 'sizeof', 'do', 'else'}


def prep_inc(tmp):
    inc = os.path.join(tmp, 'inc')
    shutil.rmtree(inc, ignore_errors=True)
    shutil.copytree(os.path.join(ROOT, 'include'), inc)
    p = os.path.join(inc, 'rw/abu_types.h')
    t = open(p).read()
    open(p, 'w').write(re.sub(r'sizeof\(([A-Za-z_ ]+)\*\)', '4', t))
    return inc


def diags(path, inc):
    r = subprocess.run(['clang', '-fsyntax-only', '-target', 'x86_64-linux-gnu'] + WFLAGS
                       + ['-I' + ROOT, '-I' + inc, path], capture_output=True, text=True, cwd=CODE)
    out = []
    for l in r.stderr.splitlines():
        m = DIAG.match(l)
        if m and m.group(1) == os.path.basename(path):
            out.append((int(m.group(2)), int(m.group(3)), m.group(4), m.group(5)))
    return out, r.returncode


def truncating(d):
    """a warning where a pointer meets a 32-bit integer, or an sptr/uptr is narrowed to one"""
    _, _, msg, flag = d
    if flag == '-Wshorten-64-to-32':
        return re.search(r"^implicit conversion loses integer precision: '(?:sptr|uptr|intptr_t|uintptr_t)'", msg) is not None
    return not re.search(r"'(?:sptr|uptr|long|unsigned long|intptr_t|uintptr_t|size_t)'", msg) or \
        re.search(INTT, msg) is not None


def func_bounds(lines):
    """line index -> index of the line that opens the enclosing function definition"""
    start, cur = [], None
    depth = 0
    for i, l in enumerate(lines):
        if depth == 0 and FDEF.match(l) and not l.rstrip().endswith(';') and FDEF.match(l).group(1) not in KEYWORDS:
            cur = i
        depth += l.count('{') - l.count('}')
        start.append(cur if depth > 0 or (cur is not None and i == cur) else None)
    return start


def arg_end(s, i):
    """end index (exclusive) of the call argument starting at s[i]"""
    depth = 0
    j = i
    while j < len(s):
        c = s[j]
        if c in '([':
            depth += 1
        elif c in ')]':
            if depth == 0:
                return j
            depth -= 1
        elif c == ',' and depth == 0:
            return j
        j += 1
    return j


SIMPLE = re.compile(r'^\s*\(?\s*&?\s*\w+(?:\s*(?:\.|->)\s*\w+|\s*\[[^\[\]]*\])*\s*\)?\s*$')
LHS = re.compile(r'^\s*(?:(?:s32|u32|int|unsigned int)\s+)?(\w+)\s*(?:[-+|&^]?=)(?!=)')


def find_decl(lines, fstart, use, name):
    """index of the local declaration `T ... name ...;` (T a 32-bit int) between fstart and use"""
    rx = re.compile(r'^(\s+)(s32|u32|int|unsigned int)\s+([^;()]*)\b%s\b([^;()]*);\s*$' % re.escape(name))
    for i in range(use, fstart, -1):
        m = rx.match(lines[i])
        if m:
            parts = [p.strip() for p in (m.group(3) + name + m.group(4)).split(',')]
            if any(re.match(r'^\**\s*%s\s*(=.*)?$' % re.escape(name), p) for p in parts):
                return i
        if re.match(r'^\s*\w[\w\s\*]*\b%s\b\s*[;=\[,]' % re.escape(name), lines[i]) and i != use:
            return None   # declared with another type (or shadowed)
    return None


def retype_decl(lines, i, name):
    m = re.match(r'^(\s+)(s32|u32|int|unsigned int)\s+(.*);(\s*)$', lines[i])
    ind, t, body = m.group(1), m.group(2), m.group(3)
    parts = [p.strip() for p in body.split(',')]
    if any('*' in p or '[' in p for p in parts if re.match(r'^\**\s*%s\b' % re.escape(name), p)):
        return False
    keep = [p for p in parts if not re.match(r'^%s\s*(=.*)?$' % re.escape(name), p)]
    mine = [p for p in parts if re.match(r'^%s\s*(=.*)?$' % re.escape(name), p)]
    if len(mine) != 1:
        return False
    new = '%s%s %s;' % (ind, NARROW[t], mine[0])
    if keep:
        lines[i] = '%s%s %s;' % (ind, t, ', '.join(keep))
        lines.insert(i + 1, new)
    else:
        lines[i] = new
    return True


def one_pass(path, inc, rules):
    ds, rc = diags(path, inc)
    if rc != 0 and not ds:
        return False, ds
    lines = open(path, errors='replace').read().split('\n')
    starts = func_bounds(lines)
    edits = {}      # line -> list of (col0, old, new)
    renames = set()  # (fstart, name)
    castcols = {(l, c) for l, c, msg, f in ds if f in ('-Wpointer-to-int-cast', '-Wvoid-pointer-to-int-cast')}
    done_cols = set()
    # rule A sites first: the cast warning of the same column must not become rule D
    ds = sorted(ds, key=lambda d: d[3] != '-Wint-conversion')
    for l, c, msg, flag in ds:
        li, c0 = l - 1, c - 1
        if li >= len(lines):
            continue
        s = lines[li]
        fs = starts[li]
        # A: drop `(s32)` in front of a pointer passed to a pointer parameter
        if ('A' in rules and flag == '-Wint-conversion' and 'integer to pointer conversion passing' in msg
                and (l, c) in castcols):
            m = CAST32.match(s, c0)
            if m:
                e = arg_end(s, m.end())
                if SIMPLE.match(s[m.end():e]) and s[e:e + 1] in (',', ')'):
                    ws = re.match(r'\s*', s[m.end():]).group(0)
                    edits.setdefault(li, []).append((c0, m.group(0) + ws, ''))
                    done_cols.add((l, c))
                    continue
        if 'D' in rules and flag in ('-Wpointer-to-int-cast', '-Wvoid-pointer-to-int-cast') and (l, c) not in done_cols:
            m = CAST32.match(s, c0)
            if m and fs is not None:
                edits.setdefault(li, []).append((c0, m.group(0), '(%s)' % NARROW[m.group(1)]))
                continue
        if 'C' not in rules or fs is None:
            continue
        name = None
        if flag in ('-Wint-to-pointer-cast', '-Wint-to-void-pointer-cast'):
            m = re.match(r'\(\s*[\w\s]+\*+\s*\)\s*\(?\s*(\w+)\s*(?!\s*(?:\(|->|\.|\[))', s[c0:])
            if m:
                name = m.group(1)
        elif flag == '-Wint-conversion' and re.search(r'integer to pointer conversion (?:passing|assigning|initializing|returning)', msg) \
                and re.search(INTT, msg) and 'returning' not in msg:
            if 'passing' in msg:
                e = arg_end(s, c0)
                m = re.match(r'^\s*(\w+)\s*$', s[c0:e])
            else:
                m = re.match(r'(\w+)\s*(?:[;,)]|$)', s[c0:])
            if m:
                name = m.group(1)
        elif flag == '-Wint-conversion' and 'pointer to integer conversion' in msg and \
                re.search(r'(?:assigning to|initializing) ' + INTT, msg):
            m = LHS.match(s)
            if m:
                name = m.group(1)
        elif flag == '-Wshorten-64-to-32' and re.search(r"'(?:sptr|uptr)'", msg):
            m = LHS.match(s)
            if m:
                name = m.group(1)
        if name and not name.isdigit() and name not in KEYWORDS:
            renames.add((fs, li, name))
    changed = False
    for li, es in edits.items():
        s = lines[li]
        for c0, old, new in sorted(set(es), reverse=True):
            if s[c0:c0 + len(old)] == old:
                s = s[:c0] + new + s[c0 + len(old):]
        if s != lines[li]:
            lines[li] = s
            changed = True
    # declarations last (they may insert lines); bottom-up keeps indices valid
    targets = {}
    for fs, li, name in renames:
        d = find_decl(lines, fs, li, name)
        if d is not None:
            targets[(d, name)] = True
    for d, name in sorted(targets, reverse=True):
        if retype_decl(lines, d, name):
            changed = True
    if changed:
        open(path, 'w').write('\n'.join(lines))
    return changed, ds


def process(path, inc, rules, maxpass=6):
    before, _ = diags(path, inc)
    for _ in range(maxpass):
        ch, _ = one_pass(path, inc, rules)
        if not ch:
            break
    after, _ = diags(path, inc)
    return path, before, after


def call_at(s, c0):
    """(callee, argument index) of the call argument that contains column c0 of line s"""
    depth, idx, j = 0, 0, c0 - 1
    while j >= 0:
        ch = s[j]
        if ch in ')]':
            depth += 1
        elif ch in '([':
            if depth == 0:
                m = re.search(r'(\w+)\s*$', s[:j]) if ch == '(' else None
                if m and m.group(1) not in KEYWORDS and not re.match(r'[su](8|16|32)|sptr|uptr|int|void', m.group(1)):
                    return m.group(1), idx
                return None
            depth -= 1
        elif ch == ',' and depth == 0:
            idx += 1
        elif ch in ';{}' or (ch == '=' and depth == 0 and s[j - 1:j] not in '=!<>' and s[j + 1:j + 2] != '='):
            return None
        j -= 1
    return None


INTP = re.compile(r'^\s*(?:(?:const|register|signed)\s+)*(s32|u32|int|unsigned int|unsigned)(\s+\w+|\s*)$')
PROTO = r'^(?P<pre>[^;{}()=]*?\b)%s\s*\((?P<params>[^()]*(?:\([^()]*\)[^()]*)*)\)\s*(?P<end>;|\{|$)'


def split_top(p):
    out, depth, cur = [], 0, ''
    for ch in p:
        if ch == '(':
            depth += 1
        elif ch == ')':
            depth -= 1
        if ch == ',' and depth == 0:
            out.append(cur)
            cur = ''
        else:
            cur += ch
    out.append(cur)
    return out


def params_pass(rows, srcs):
    """retype integer parameters that callers fill with pointers / sptr, and integer return
    types of functions that return a pointer / sptr, in the definition and every declaration"""
    want = {}      # name -> set(arg index) or 'ret'
    texts = {}
    for f, l, c, msg, flag in rows:
        if f not in texts:
            texts[f] = open(os.path.join(CODE, f), errors='replace').read().split('\n')
        s = texts[f][l - 1]
        if (flag == '-Wint-conversion' and msg.startswith('incompatible pointer to integer conversion passing')) or \
                (flag == '-Wshorten-64-to-32' and truncating((l, c, msg, flag))):
            r = call_at(s, c - 1)
            if r:
                want.setdefault(r[0], set()).add(r[1])
            elif flag == '-Wshorten-64-to-32' and re.match(r'^\s*return\b', s):
                fs = func_bounds(texts[f])[l - 1]
                if fs is not None:
                    m = FDEF.match(texts[f][fs])
                    want.setdefault(m.group(1), set()).add('ret')
        elif flag == '-Wint-conversion' and 'pointer to integer conversion returning' in msg:
            fs = func_bounds(texts[f])[l - 1]
            if fs is not None:
                want.setdefault(FDEF.match(texts[f][fs]).group(1), set()).add('ret')
    defs = {}
    for f in srcs:
        for i, line in enumerate(open(f, errors='replace').read().split('\n')):
            m = FDEF.match(line)
            if m and not line.rstrip().endswith(';') and m.group(1) in want and not line.startswith(' '):
                defs.setdefault(m.group(1), []).append((f, i))
    edits = collections.Counter()
    plan = {}
    for name, idxs in want.items():
        if len(defs.get(name, [])) != 1:
            continue
        f, i = defs[name][0]
        line = open(f, errors='replace').read().split('\n')[i]
        m = re.match(PROTO % re.escape(name), line)
        if not m:
            continue
        ps = split_top(m.group('params'))
        ok = set()
        for k in idxs:
            if k == 'ret':
                if re.match(r'^\s*(?:static\s+)?(s32|u32|int|unsigned int)\s+$', m.group('pre')):
                    ok.add(k)
            elif k < len(ps) and INTP.match(ps[k]):
                ok.add(k)
        if ok:
            plan[name] = ok
    if not plan:
        return 0, []
    rx = re.compile(PROTO % r'(?P<name>%s)' % '|'.join(map(re.escape, plan)))
    touched = []
    for f in srcs:
        t = open(f, errors='replace').read()
        lines = t.split('\n')
        ch = False
        for i, line in enumerate(lines):
            m = rx.match(line)
            if not m or not re.search(r'\w', m.group('pre')):
                continue
            ok = plan[m.group('name')]
            ps = split_top(m.group('params'))
            new_ps = list(ps)
            for k in ok:
                if k != 'ret' and k < len(ps):
                    mm = INTP.match(ps[k])
                    if mm:
                        new_ps[k] = re.sub(r'\b(s32|u32|int|unsigned int|unsigned)\b',
                                           lambda x: NARROW[x.group(1)], ps[k], count=1)
            pre = m.group('pre')
            if 'ret' in ok:
                pre = re.sub(r'\b(s32|u32|int|unsigned int)(\s+)$', lambda x: NARROW[x.group(1)] + x.group(2), pre)
            nl = pre + m.group('name') + '(' + ','.join(new_ps) + ')' + line[m.end('params') + 1:]
            if nl != line:
                lines[i] = nl
                ch = True
                edits[m.group('name')] += 1
        if ch:
            open(f, 'w').write('\n'.join(lines))
            touched.append(f)
    return len(plan), touched


def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('files', nargs='*')
    ap.add_argument('--base', default='HEAD')
    ap.add_argument('-j', type=int, default=4)
    ap.add_argument('--rules', default='ACD')
    ap.add_argument('--params', type=int, default=0, metavar='N',
                    help='N rounds of the cross-file rule P (parameters / return types) + the file rules')
    ap.add_argument('--dry', action='store_true', help='no asm_diff / put-back (debugging)')
    a = ap.parse_args()
    allc = sorted(os.path.join(CODE, f) for f in os.listdir(CODE) if f.endswith('.c'))
    files = [os.path.abspath(f) for f in a.files] or allc
    tmp = tempfile.mkdtemp(prefix='retype')
    inc = prep_inc(tmp)
    hdrs = sorted(glob.glob(os.path.join(ROOT, 'include/**/*.h'), recursive=True))
    orig = {f: open(f, errors='replace').read() for f in allc + hdrs}

    def run(rules, fl):
        with ThreadPoolExecutor(a.j) as ex:
            return list(ex.map(lambda f: process(f, inc, rules), fl))

    def rows_of(res):
        return [(os.path.basename(p), l, c, m, fl) for p, (b, af) in res.items() for l, c, m, fl in af]

    res = {p: (b, af) for p, b, af in run(a.rules, files)}
    first = {p: b for p, (b, af) in res.items()}
    for rnd in range(a.params):
        n, touched = params_pass(rows_of(res), allc + hdrs)
        print('round %d: %d functions retyped in %d files' % (rnd + 1, n, len(touched)))
        if not touched:
            break
        if any(t.endswith('.h') for t in touched):
            inc = prep_inc(tmp)
            again = allc
        else:
            again = sorted(set(touched) | {f for f in files if res[f][1]})
        res.update({p: (first.get(p, b), af) for p, b, af in run(a.rules, again)})
    edited = [f for f in allc if open(f, errors='replace').read() != orig[f]]
    hedited = [f for f in hdrs if open(f, errors='replace').read() != orig[f]]
    bad = set()
    if not a.dry and (edited or hedited):
        bad = asm_check(None if hedited else edited, a.base)
        if bad and a.rules != 'D' and not a.params:
            for f in bad:
                open(f, 'w').write(orig[f])
            res.update({p: (first.get(p, b), af) for p, b, af in run('D', sorted(bad))})
            again = [f for f in bad if open(f, errors='replace').read() != orig[f]]
            bad2 = asm_check(again, a.base) if again else set()
            bad = (bad - set(again)) | bad2
        for f in bad:
            open(f, 'w').write(orig[f])
    tb = sum(sum(map(truncating, first[f])) for f in res)
    ta = sum(sum(map(truncating, res[f][1] if f not in bad else first[f])) for f in res)
    wb = sum(len(first[f]) for f in res)
    wa = sum(len(res[f][1] if f not in bad else first[f]) for f in res)
    kept = [f for f in edited if f not in bad]
    print('files: %d edited (+%d headers), %d put back (%s)' % (len(kept), len(hedited), len(bad),
          ' '.join(sorted(os.path.basename(f) for f in bad))[:800] or '-'))
    print('pointer-width warnings: %d -> %d; truncating (pointer <-> 32-bit int): %d -> %d' % (wb, wa, tb, ta))
    shutil.rmtree(tmp, ignore_errors=True)


def asm_check(files, base):
    r = subprocess.run([sys.executable, os.path.join(ROOT, 'tools/asm_diff.py'), '--base', base, '-j', '8']
                       + sorted(files or []), capture_output=True, text=True, cwd=ROOT)
    bad = set()
    for l in r.stdout.splitlines():
        m = re.match(r'(DIFF|FAIL) (\S+)', l)
        if m:
            bad.add(os.path.join(ROOT, m.group(2)))
    if r.returncode and not bad:
        sys.exit('asm_diff failed:\n' + r.stdout[-2000:] + r.stderr[-2000:])
    return bad


if __name__ == '__main__':
    main()
