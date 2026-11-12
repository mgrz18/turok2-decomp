#!/usr/bin/env python3
"""Find host-compiled game code that views an engine struct at its N64 layout (docs/port/64BIT.md
part 6, STABILITY.md section 9).

    python3 tools/port/n64viewcheck.py --build OUT/build [-j 4] [--json out.json] [files...]

Parses every src/ file of the build's compile_commands.json the way the host compiles it (so only
the TARGET_PORT side of each #ifdef is seen) with libclang, for x86_64 and, when the file has a
candidate, again for mips (the N64 layout). Three kinds of site:

  raw     a pointer cast of `base + C` / `base - C` / `((u8 *) base)[C]` on a byte pointer or an
          integer (M2C_FIELD, `*(s32 *)((u8 *) p + 0x1C)`, `P_S32(p, 0x1C)`), C a constant byte offset
  stride  the same with an `i * K` / `i << n` term: an N64 array stride (CGameRegion 32, ...)
  view    a member read through a struct defined in the .c file itself whose fields are placed by
          pads (`u8 pad0[0xD4]; u32 flags;`): an m2c/Rage Wars view of an engine struct

Each site is classed by the base it is applied to:

  BAD      the base's static type is an include/ struct S and the N64 member of S at that offset
           sits at another host offset (or past S)
  suspect  a view with no static type behind it whose named fields fit a key engine struct
           (CGameObjectInstance, CIntelligence, CPlayer, CAI, CGameRegion, CCamera, ...: at least
           2 fields and 60 % of them start a member of the same size there) and some of those
           members are at another host offset than the view's field: review by hand (the fit
           also matches look-alikes: ROM records, the view's own allocations)
  moves    a view whose own host layout differs from its N64 one (a pointer before a named
           field) with no engine struct fitting: fine for the view's own allocations, wrong over
           ROM data or an engine struct
  same     S known and the member at that offset has the same offset and size on both hosts
  global   `&symbol +/- C`: a view across a data symbol (gendata places the N64 .data/.bss in
           order but at host sizes: check that every symbol in between keeps its N64 size)
  untyped  a raw offset on void * / sptr / an integer: the struct is not known statically. The
           report lists the key structs whose N64 layout has a member of that size there and
           moves on the host (`guess`)
  local    a view (or base) with no engine struct behind it that fits

Only the files linked into turok2 are read (module 2 and the replaced libultra files compile
but are not linked: port/cmake/link.cmake), unless --unlinked.
Exit 0; prints one line per site (`--all` also prints the `same` ones) and a summary.
"""
import argparse
import collections
import json
import multiprocessing
import os
import re
import shlex
import sys

import clang.cindex as ci

LIBCLANG = os.environ.get('LIBCLANG', '/usr/lib/llvm-19/lib/libclang-19.so.1')
ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
K = ci.CursorKind
CAST_KINDS = (K.CSTYLE_CAST_EXPR, K.UNEXPOSED_EXPR, K.PAREN_EXPR)
# the structs whose views broke the host before (rounds 9-11); `guess` ranks these first
KEY_STRUCTS = ('CGameObjectInstance', 'CIntelligence', 'CPlayer', 'CAI', 'CGameRegion', 'CCamera',
               'CRegionInfo', 'CScene', 'CEngineApp', 'CInstanceHeader', 'CGameSimpleInstance',
               'CParticle', 'CParticleSystem', 'CWarp', 'CLevelActor')


def compile_args(e):
    args = e.get('arguments') or shlex.split(e['command'])
    out, skip = [], False
    for a in args[1:]:
        if skip:
            skip = False
            continue
        if a in ('-o', '-c', '-MF', '-MT', '-MQ'):
            skip = True
            continue
        if a == e['file'] or a.endswith(('.c', '.cpp')) or a.startswith('-W') or a in ('-MD', '-MMD'):
            continue
        out.append(a)
    return out + ['-w']


def strip(c):
    """The expression under parens and casts, and whether a cast was crossed."""
    while c.kind in CAST_KINDS:
        kids = [k for k in c.get_children() if k.kind != K.TYPE_REF and k.kind.is_expression()]
        if not kids:
            break
        c = kids[-1]
    return c


def expr_kids(c):
    return [k for k in c.get_children() if k.kind.is_expression()]


def const_value(c):
    """The value of an integer constant expression (literals, unary minus, + - * <<), or None."""
    c = strip(c)
    if c.kind == K.INTEGER_LITERAL:
        t = ''.join(x.spelling for x in c.get_tokens())
        t = t.rstrip('uUlL')
        try:
            return int(t, 0)
        except ValueError:
            return None
    if c.kind == K.UNARY_OPERATOR:
        toks = [x.spelling for x in c.get_tokens()]
        kids = expr_kids(c)
        if toks and toks[0] == '-' and len(kids) == 1:
            v = const_value(kids[0])
            return None if v is None else -v
        return None
    if c.kind == K.BINARY_OPERATOR:
        kids = expr_kids(c)
        if len(kids) != 2:
            return None
        a, b = const_value(kids[0]), const_value(kids[1])
        if a is None or b is None:
            return None
        op = c.binary_operator
        B = ci.BinaryOperator
        if op == B.Add:
            return a + b
        if op == B.Sub:
            return a - b
        if op == B.Mul:
            return a * b
        if op == B.Shl:
            return a << b
    return None


def is_bytewise(t):
    """A byte pointer or an integer (address arithmetic in bytes)."""
    t = t.get_canonical()
    if t.kind == ci.TypeKind.POINTER:
        p = t.get_pointee().get_canonical()
        return p.kind == ci.TypeKind.VOID or p.get_size() == 1
    return t.kind in (ci.TypeKind.INT, ci.TypeKind.UINT, ci.TypeKind.LONG, ci.TypeKind.ULONG,
                      ci.TypeKind.LONGLONG, ci.TypeKind.ULONGLONG)


def record_of(t):
    """The include/ struct a pointer type points at (name, declaration), or None."""
    t = t.get_canonical()
    if t.kind != ci.TypeKind.POINTER:
        return None
    p = t.get_pointee().get_canonical()
    if p.kind != ci.TypeKind.RECORD:
        return None
    d = p.get_declaration()
    return d


def rec_name(d):
    n = d.spelling
    if not n or 'unnamed' in n or '(' in n:
        n = d.type.get_canonical().spelling
    return n.replace('struct ', '')


def in_include(d):
    f = d.location.file
    return f is not None and '/include/' in f.name


def flatten_add(c, sign, terms):
    c2 = c
    while c2.kind == K.PAREN_EXPR:
        c2 = expr_kids(c2)[0]
    if c2.kind == K.BINARY_OPERATOR and c2.binary_operator in (ci.BinaryOperator.Add, ci.BinaryOperator.Sub):
        kids = expr_kids(c2)
        if len(kids) == 2:
            flatten_add(kids[0], sign, terms)
            flatten_add(kids[1], sign if c2.binary_operator == ci.BinaryOperator.Add else -sign, terms)
            return
    terms.append((sign, c))


def stride_of(c):
    c = strip(c)
    if c.kind == K.BINARY_OPERATOR:
        kids = expr_kids(c)
        if len(kids) == 2:
            op = c.binary_operator
            if op == ci.BinaryOperator.Mul:
                for i in (0, 1):
                    v = const_value(kids[i])
                    if v is not None:
                        return v
            if op == ci.BinaryOperator.Shl:
                v = const_value(kids[1])
                if v is not None:
                    return 1 << v
    return None


def func_of(c):
    p = c.semantic_parent
    while p is not None and p.kind != K.FUNCTION_DECL:
        if p.kind == K.TRANSLATION_UNIT:
            return '?'
        p = p.semantic_parent
    return p.spelling if p is not None else '?'


class Layouts:
    """Member paths of a record on one target: [(path, off, size, is_record)]."""

    def __init__(self):
        self.cache = {}

    def leaves(self, d, depth=0):
        key = (d.hash, depth)
        if key in self.cache:
            return self.cache[key]
        out = []
        rt = d.type.get_canonical()
        for f in d.type.get_canonical().get_fields():
            ft = f.type.get_canonical()
            off = f.get_field_offsetof() // 8
            name = f.spelling or '<anon>'
            if f.is_anonymous() or not f.spelling:
                fd = ft.get_declaration()
                for (p, o, s, r) in self.leaves(fd, depth + 1):
                    out.append((p, off + o, s, r))
                continue
            sz = ft.get_size()
            fdcl = ft.get_declaration()
            isrec = ft.kind == ci.TypeKind.RECORD and fdcl.is_definition()
            out.append((name, off, sz, isrec or ft.kind == ci.TypeKind.CONSTANTARRAY))
            if isrec and depth < 5:
                for (p, o, s, r) in self.leaves(fdcl, depth + 1):
                    out.append((name + '.' + p, off + o, s, r))
            elif ft.kind == ci.TypeKind.CONSTANTARRAY:
                et = ft.element_type.get_canonical()
                es = et.get_size()
                ed = et.get_declaration()
                n = ft.element_count
                if et.kind == ci.TypeKind.RECORD and ed.is_definition() and depth < 5 and n <= 64:
                    sub = self.leaves(ed, depth + 1)
                    for k in range(n):
                        for (p, o, s, r) in sub:
                            out.append(('%s[%d].%s' % (name, k, p), off + k * es + o, s, r))
                elif es > 0 and n <= 256:
                    for k in range(n):
                        out.append(('%s[%d]' % (name, k), off + k * es, es, False))
        self.cache[key] = out
        return out


def locate(lv, off, size):
    """The deepest scalar member that starts at off (size match preferred), else the one containing it."""
    exact = [x for x in lv if x[1] == off and not x[3]]
    if exact:
        same = [x for x in exact if x[2] == size]
        return (same or exact)[0], 0
    best = None
    for x in lv:
        if x[1] <= off < x[1] + max(x[2], 1):
            if best is None or x[2] <= best[2]:
                best = x
    if best is None:
        return None, 0
    return best, off - best[1]


def find_records(tu, names):
    out = {}
    for c in tu.cursor.walk_preorder():
        if c.kind in (K.STRUCT_DECL, K.UNION_DECL) and c.is_definition():
            n = rec_name(c)
            if n in names and n not in out:
                out[n] = c
        elif c.kind == K.TYPEDEF_DECL and c.spelling in names and c.spelling not in out:
            d = c.underlying_typedef_type.get_canonical().get_declaration()
            if d.is_definition():
                out[c.spelling] = d
    return out


def analyse(job):
    path, args = job
    if not ci.Config.loaded:
        ci.Config.set_library_file(LIBCLANG)
    idx = ci.Index.create()
    try:
        th = idx.parse(path, args=args + ['-target', 'x86_64-linux-gnu'])
    except ci.TranslationUnitLoadError as e:
        return {'file': path, 'error': str(e), 'sites': []}
    main = os.path.abspath(path)
    raw = []    # (cursor-ish info)
    views = []
    def walk():
        for top in th.cursor.get_children():
            if top.kind != K.FUNCTION_DECL or not top.is_definition() or top.location.file is None \
                    or os.path.abspath(top.location.file.name) != main:
                continue
            for c in top.walk_preorder():
                yield top.spelling, c

    for fname, c in walk():
        if c.kind == K.CSTYLE_CAST_EXPR and c.type.get_canonical().kind == ci.TypeKind.POINTER:
            kids = expr_kids(c)
            if not kids:
                continue
            inner = kids[-1]
            while inner.kind == K.PAREN_EXPR:
                inner = expr_kids(inner)[0]
            if inner.kind != K.BINARY_OPERATOR or inner.binary_operator not in (ci.BinaryOperator.Add, ci.BinaryOperator.Sub):
                continue
            terms = []
            flatten_add(inner, 1, terms)
            base, off, strides, other = None, 0, [], 0
            for (sg, t) in terms:
                v = const_value(t)
                if v is not None:
                    off += sg * v
                    continue
                st = stride_of(t)
                if st is not None:
                    strides.append(st)
                    continue
                if base is None and (t.type.get_canonical().kind == ci.TypeKind.POINTER or is_bytewise(t.type)):
                    base = t
                else:
                    other += 1
            if base is None or other or not is_bytewise(base.type):
                continue
            if off == 0 and not strides:
                continue
            under = strip(base)
            raw.append((fname, c, under, off, strides, c.type.get_canonical().get_pointee().get_canonical().get_size()))
        elif c.kind == K.ARRAY_SUBSCRIPT_EXPR:
            kids = expr_kids(c)
            if len(kids) == 2 and kids[0].kind in CAST_KINDS and kids[0].type.get_canonical().kind == ci.TypeKind.POINTER:
                v = const_value(kids[1])
                if v is None or not is_bytewise(kids[0].type):
                    continue
                under = strip(kids[0])
                if under.type.get_canonical().kind == ci.TypeKind.POINTER and record_of(under.type) is not None:
                    raw.append((fname, c, under, v, [], 1))
        elif c.kind == K.MEMBER_REF_EXPR:
            f = c.referenced
            if f is None or f.kind != K.FIELD_DECL:
                continue
            rec = f.semantic_parent
            while rec is not None and rec.kind in (K.STRUCT_DECL, K.UNION_DECL) and (not rec.spelling or rec.is_anonymous()):
                if rec.semantic_parent is None or rec.semantic_parent.kind not in (K.STRUCT_DECL, K.UNION_DECL):
                    break
                rec = rec.semantic_parent
            if rec is None or rec.location.file is None or os.path.abspath(rec.location.file.name) != main:
                continue
            names = [x.spelling for x in rec.type.get_canonical().get_fields()]
            if not any(re.match(r'_?pad|unk_?pad|_hp_', n or '') for n in names):
                continue
            if any((n or '').startswith('_hp_') for n in names):
                continue  # hostview.py's host view: placed by offsetof
            kids = expr_kids(c)
            base = strip(kids[0]) if kids else None
            views.append((fname, c, rec, f, base))
    if not raw and not views:
        return {'file': path, 'sites': []}
    tm = idx.parse(path, args=args + ['-target', 'mips-linux-gnu'])
    LH, LM = Layouts(), Layouts()
    # every include/ record, both targets, that differs between them
    recm = {}
    for c in tm.cursor.walk_preorder():
        if c.kind in (K.STRUCT_DECL, K.UNION_DECL) and c.is_definition() and in_include(c):
            recm.setdefault(rec_name(c), c)
        elif c.kind == K.TYPEDEF_DECL and in_include(c):
            d = c.underlying_typedef_type.get_canonical().get_declaration()
            if d.kind in (K.STRUCT_DECL, K.UNION_DECL) and d.is_definition():
                recm.setdefault(c.spelling, d)
    rech = find_records(th, set(recm))
    # local records on mips, by spelling+line
    locm = {}
    for c in tm.cursor.walk_preorder():
        if c.kind in (K.STRUCT_DECL, K.UNION_DECL) and c.is_definition() and c.location.file is not None \
                and os.path.abspath(c.location.file.name) == main:
            locm[(c.location.line, c.location.column)] = c

    def moved(name):
        """{path: (n64off, n64size, hoff, hsize)} of struct `name`, or None if unknown on a side."""
        if name not in recm or name not in rech:
            return None
        hm = {p: (o, s) for (p, o, s, r) in LH.leaves(rech[name])}
        return LM.leaves(recm[name]), hm

    def judge(name, off, size):
        m = moved(name)
        if m is None:
            return None
        lvm, hm = m
        x, delta = locate(lvm, off, size)
        if x is None:
            return ('BAD', '%s+0x%X: past the N64 struct (0x%X)' % (name, off, recm[name].type.get_size()))
        p, o, s, r = x
        if p not in hm:
            return ('BAD', '%s.%s: not on the host' % (name, p))
        ho, hs = hm[p]
        if ho == o and (hs == s or delta):
            return ('same', '%s.%s%s' % (name, p, ('+%d' % delta) if delta else ''))
        return ('BAD', '%s.%s%s: N64 0x%X, host 0x%X%s' % (name, p, ('+%d' % delta) if delta else '', o + delta, ho + delta,
                                                          '' if hs == s else ' (size %d -> %d)' % (s, hs)))

    diverge = {}

    def guesses(off, size):
        out = []
        for name in recm:
            if name not in rech:
                continue
            m = moved(name)
            lvm, hm = m
            x, delta = locate(lvm, off, size)
            if x is None or delta or x[2] != size:
                continue
            p, o, s, r = x
            ho = hm.get(p, (None,))[0]
            if ho is not None and ho != o and name in KEY_STRUCTS:
                out.append('%s.%s' % (name, p))
        out.sort(key=lambda g: (g.split('.')[0] not in KEY_STRUCTS, g))
        return out

    sites = []
    seen_raw = set()
    for (fn, c, under, off, strides, size) in raw:
        t = under.type
        d = record_of(t)
        bkey = (fn, ' '.join(x.spelling for x in under.get_tokens()), off, tuple(strides))
        if bkey in seen_raw:
            continue
        seen_raw.add(bkey)
        line = c.location.line
        txt = ' '.join(x.spelling for x in c.get_tokens())[:120]
        if strides:
            sites.append({'kind': 'stride', 'cls': 'BAD' if any(s in (0x20, 0x18, 0x14) for s in strides) else 'untyped',
                          'file': path, 'line': line, 'func': fn, 'off': off, 'strides': strides,
                          'base': t.spelling, 'what': rec_name(d) if d is not None else '', 'text': txt})
            continue
        if d is not None and in_include(d):
            j = judge(rec_name(d), off, size)
            if j is not None:
                sites.append({'kind': 'raw', 'cls': j[0], 'file': path, 'line': line, 'func': fn, 'off': off,
                              'size': size, 'base': t.spelling, 'what': j[1], 'text': txt})
                continue
        if d is not None:
            sites.append({'kind': 'raw', 'cls': 'local', 'file': path, 'line': line, 'func': fn, 'off': off,
                          'size': size, 'base': t.spelling, 'what': rec_name(d), 'text': txt})
            continue
        if under.kind == K.UNARY_OPERATOR and under.referenced is None:
            dr = [k for k in under.walk_preorder() if k.kind == K.DECL_REF_EXPR]
            if dr and dr[0].referenced is not None and dr[0].referenced.kind == K.VAR_DECL:
                sites.append({'kind': 'raw', 'cls': 'global', 'file': path, 'line': line, 'func': fn, 'off': off,
                              'size': size, 'base': t.spelling, 'what': '&' + dr[0].spelling, 'text': txt})
                continue
        g = guesses(off, size) if off > 0 else []
        sites.append({'kind': 'raw', 'cls': 'untyped', 'file': path, 'line': line, 'func': fn, 'off': off,
                      'size': size, 'base': t.spelling, 'what': under.spelling, 'guess': g[:6], 'text': txt})
    # views: per (view record, field) once per function
    # views: one site per (function, view), its fields gathered
    byview = collections.OrderedDict()
    for (fn, c, rec, f, base) in views:
        k = (fn, rec.hash)
        e = byview.setdefault(k, {'rec': rec, 'fields': collections.OrderedDict(), 'line': c.location.line,
                                  'real': None})
        e['fields'].setdefault(f.spelling, c.location.line)
        bt = base.type if base is not None else None
        d = record_of(bt) if bt is not None else None
        if d is not None and in_include(d):
            e['real'] = rec_name(d)
    keyl = {}
    for name in KEY_STRUCTS:
        m = moved(name)
        if m is not None:
            keyl[name] = m
    for (fn, _), e in byview.items():
        rec = e['rec']
        recn = rec_name(rec)
        rm = locm.get((rec.location.line, rec.location.column))
        if rm is None:
            continue
        lvm = {x[0]: x for x in LM.leaves(rm)}
        lvh = {p: (o, s2) for (p, o, s2, r) in LH.leaves(rec)}
        flds = []
        for fname in e['fields']:
            x = lvm.get(fname)
            if x is None or x[3]:
                continue
            flds.append((fname, x[1], x[2], lvh.get(fname, (None,))[0]))
        if not flds:
            continue
        selfmove = [f for f in flds if f[3] != f[1]]
        site = {'kind': 'view', 'file': path, 'line': e['line'], 'func': fn, 'view': recn,
                'field': ','.join(f[0] for f in flds), 'off': flds[0][1], 'size': flds[0][2]}
        if e['real']:
            bad = []
            for (fname, o, s2, ho) in flds:
                j = judge(e['real'], o, s2)
                if j is not None and j[0] == 'BAD':
                    lv, hm = moved(e['real'])
                    x, delta = locate(lv, o, s2)
                    if x is not None and hm.get(x[0], (None,))[0] == ho and not delta:
                        continue  # the view's host offset is the member's
                    bad.append('%s->%s' % (fname, j[1]))
            site.update(cls='BAD' if bad else 'same', what='on %s: %s' % (e['real'], '; '.join(bad[:4])))
            sites.append(site)
            continue
        # the best-fitting key struct: fields that start a member of the same size there
        best = None
        for name, (lv, hm) in keyl.items():
            hit, mv = 0, []
            for (fname, o, s2, ho) in flds:
                x, delta = locate(lv, o, s2)
                if x is None or delta or x[2] != s2:
                    continue
                hit += 1
                hoff = hm.get(x[0], (None,))[0]
                if hoff != ho:  # the view's own host offset (a pointer field realigns it)
                    mv.append('%s@0x%X=%s.%s(host 0x%s)' % (fname, o, name, x[0], '%X' % hoff if hoff is not None else '?'))
            if best is None or hit > best[1]:
                best = (name, hit, mv)
        fit = best is not None and best[1] >= 2 and best[1] * 10 >= len(flds) * 6
        if fit and best[2]:
            site.update(cls='suspect', what='fits %s (%d/%d fields); moved: %s' % (best[0], best[1], len(flds), '; '.join(best[2][:4])))
        elif selfmove:
            site.update(cls='moves', what='the view itself moves on the host: ' + ', '.join(
                '%s 0x%X->0x%s' % (f[0], f[1], '%X' % f[3] if f[3] is not None else '?') for f in selfmove[:4]))
        else:
            site.update(cls='local', what='fits %s (%d/%d)' % (best[0], best[1], len(flds)) if fit else '')
        sites.append(site)
    return {'file': path, 'sites': sites}


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--build', required=True)
    ap.add_argument('-j', type=int, default=4)
    ap.add_argument('--json')
    ap.add_argument('--all', action='store_true', help='also print the `same` sites')
    ap.add_argument('--unlinked', action='store_true',
                    help='also the compile-only files (module 2, replaced libultra: port/cmake/link.cmake)')
    ap.add_argument('files', nargs='*')
    a = ap.parse_args()
    cc = json.load(open(os.path.join(a.build, 'compile_commands.json')))
    want = {os.path.abspath(f) for f in a.files}
    jobs, done = [], set()
    for e in cc:
        f = os.path.abspath(e['file'])
        if not f.endswith('.c') or '/src/' not in f or f in done:
            continue
        if want and f not in want:
            continue
        done.add(f)
        if 'turok2_game_unlinked' in e.get('output', '') + e.get('command', '') and not a.unlinked:
            continue
        jobs.append((f, compile_args(e)))
    jobs.sort()
    res = []
    with multiprocessing.Pool(a.j) as pool:
        for r in pool.imap_unordered(analyse, jobs, chunksize=4):
            res.append(r)
    res.sort(key=lambda r: r['file'])
    cnt = collections.Counter()
    out = []
    for r in res:
        if r.get('error'):
            print('ERROR %s: %s' % (r['file'], r['error']))
        for s in r['sites']:
            cnt[(s['kind'], s['cls'])] += 1
            if s['cls'] == 'same' and not a.all:
                continue
            rel = os.path.relpath(s['file'], ROOT)
            extra = s.get('what', '')
            if s.get('guess'):
                extra += ' guess: ' + ', '.join(s['guess'])
            if s['kind'] == 'stride':
                extra = 'strides %s on %s %s' % (s['strides'], s['base'], s.get('what', ''))
            if s['kind'] == 'view':
                head = '%s {%s}' % (s['view'], s['field'][:80])
            else:
                head = '%s +0x%X' % (s['base'], s['off']) if s['off'] >= 0 else '%s -0x%X' % (s['base'], -s['off'])
            out.append('%-7s %-6s %s:%d %s: %s  %s' % (s['cls'], s['kind'], rel, s['line'], s['func'], head, extra))
    print('\n'.join(out))
    print('files %d; sites: %s' % (len(res), ', '.join('%s/%s %d' % (k[0], k[1], v) for k, v in sorted(cnt.items()))))
    if a.json:
        json.dump(res, open(a.json, 'w'), indent=1)


if __name__ == '__main__':
    main()
