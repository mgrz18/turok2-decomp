#!/usr/bin/env python3
"""Retype the caller-side declarations behind abicheck's ptr_from_int findings (docs/port/ABI.md).

    python3 tools/port/abiptrfix.py FINDINGS.json [--apply] [--callee NAME ...]

FINDINGS.json is `abicheck.py --json`. For an argument site, the parameter of the
declaration the caller sees (`decl` file:line) is `s32`/`int`/`u32`/`unsigned int` while the
definition takes a pointer (or sptr): the parameter becomes `sptr` (signed) or `uptr`. For a
return site where the definition returns a 32-bit int and the caller's declaration a pointer,
the DEFINITION's return type becomes sptr. sptr/uptr are s32/u32 on the N64 (common.h): the
matching build does not change (asm_diff to confirm). Declarations in OUT/build/hostified/
copies are edited in their src/us/code original. Prints what it changes; --apply writes.
"""
import argparse, collections, json, os, re, sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
INTS = {'s32': 'sptr', 'int': 'sptr', 'signed int': 'sptr', 'u32': 'uptr', 'unsigned int': 'uptr',
        'unsigned': 'uptr', 'long': 'sptr'}


def src_path(p):
    p = os.path.normpath(os.path.join(ROOT, p))
    if '/hostified/' in p:
        p = os.path.join(ROOT, 'src/us/code', os.path.basename(p))
    return p


def split_params(s):
    out, d, cur = [], 0, ''
    for ch in s:
        if ch in '([{':
            d += 1
        elif ch in ')]}':
            d -= 1
        if ch == ',' and d == 0:
            out.append(cur)
            cur = ''
        else:
            cur += ch
    out.append(cur)
    return out


def find_decl(text, name, line):
    """(start of the parameter list '(', end ')') of `name(` nearest to `line`."""
    lines = text.split('\n')
    best = None
    for m in re.finditer(r'\b%s\s*\(' % re.escape(name), text):
        ln = text.count('\n', 0, m.start()) + 1
        if best is None or abs(ln - line) < abs(best[0] - line):
            best = (ln, m.end() - 1)
    if best is None or abs(best[0] - line) > 3:
        return None
    i, d = best[1], 0
    for j in range(i, len(text)):
        if text[j] == '(':
            d += 1
        elif text[j] == ')':
            d -= 1
            if d == 0:
                return i, j
    return None


def retype_param(p):
    m = re.match(r'^(\s*)(const\s+)?(signed int|unsigned int|unsigned|s32|u32|int|long)\b(\s*\w*\s*)$', p)
    if not m:
        return None
    return m.group(1) + (m.group(2) or '') + INTS[m.group(3)] + m.group(4)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('json')
    ap.add_argument('--apply', action='store_true')
    ap.add_argument('--callee', nargs='*')
    a = ap.parse_args()
    fs = [f for f in json.load(open(a.json))['findings'] if f['cls'] == 'ptr_from_int']
    if a.callee:
        fs = [f for f in fs if f['callee'] in a.callee]
    edits = collections.defaultdict(set)   # (file, name, line) -> param indexes ('ret' = return)
    skipped = []
    for f in fs:
        m = re.match(r'arg (\d+):', f['what'])
        if m:
            file_, line = f['decl'].rsplit(':', 1)
            edits[(src_path(file_), f['callee'], int(line))].add(int(m.group(1)) - 1)
        elif f['what'].startswith('return'):
            file_, line = f['def'].rsplit(':', 1)
            edits[(src_path(file_), f['callee'], int(line))].add('ret')
        else:
            skipped.append(f)
    texts = {}
    changed = 0
    for (path, name, line), idx in sorted(edits.items(), key=lambda kv: (kv[0][0], -kv[0][2])):
        if path not in texts:
            texts[path] = open(path).read()
        t = texts[path]
        r = find_decl(t, name, line)
        if r is None:
            print('?? %s:%d %s: declaration not found' % (os.path.relpath(path, ROOT), line, name))
            continue
        i, j = r
        new_t = t
        if any(x != 'ret' for x in idx):
            ps = split_params(t[i + 1:j])
            ok = True
            for k in sorted(x for x in idx if x != 'ret'):
                if k >= len(ps):
                    ok = False
                    continue
                np_ = retype_param(ps[k])
                if np_ is None:
                    print('?? %s:%d %s param %d: %r' % (os.path.relpath(path, ROOT), line, name, k + 1,
                                                        ps[k].strip()))
                    ok = False
                    continue
                ps[k] = np_
            new_t = t[:i + 1] + ','.join(ps) + t[j:]
        if 'ret' in idx:
            # the return type: the tokens before the name on its line
            k = t.rfind('\n', 0, i) + 1
            head = new_t[k:i]
            m = re.match(r'^(\s*(?:static\s+|extern\s+)*)(s32|u32|int|unsigned int)(\s+)(%s\s*)$'
                         % re.escape(name), head)
            if m:
                new_t = new_t[:k] + m.group(1) + INTS[m.group(2)] + m.group(3) + m.group(4) + new_t[i:]
            else:
                print('?? %s:%d %s return: %r' % (os.path.relpath(path, ROOT), line, name, head))
        if new_t != t:
            old_line = t[t.rfind('\n', 0, i) + 1:t.find('\n', j)]
            new_line = new_t[new_t.rfind('\n', 0, i) + 1:new_t.find('\n', i)]
            print('%s:%d\n  - %s\n  + %s' % (os.path.relpath(path, ROOT), line, old_line.strip(),
                                            new_line.strip()))
            texts[path] = new_t
            changed += 1
    for f in skipped:
        print('-- not handled: %s %s %s' % (f['callee'], f['loc'], f['what']))
    print('%d declarations retyped' % changed)
    if a.apply:
        for p, t in texts.items():
            if open(p).read() != t:
                open(p, 'w').write(t)


if __name__ == '__main__':
    main()
