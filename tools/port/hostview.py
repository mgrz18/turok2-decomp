#!/usr/bin/env python3
"""Rebuild an m2c/Rage Wars "view" struct (a local struct that reads a real engine struct
at its N64 offsets through pads) at the HOST offsets of the real struct.

    python3 tools/port/hostview.py --build OUT/build src/us/code/func_0024C47C.c Player CPlayer

Parses the .c twice with libclang (mips-linux-gnu: the N64 layout the view was written for;
x86_64: the host), maps every named field of the view (fields named pad*/_pad* are padding)
to the member path of the real struct at that N64 offset, and prints a TARGET_PORT
replacement: the same fields, in the same order and with the same declarations, separated by
placed by __builtin_offsetof(<real>, <path>) (each field in its own anonymous struct
inside one anonymous union: no running sizes, overlaps allowed). Fields that fall inside a member get
the byte delta (only right for byte-sized views). Prints a warning for every field whose
host size differs from the real member's (an s32 view over a pointer...).

The output is meant to be pasted under #ifdef TARGET_PORT (the N64 view stays for the
matching build); check the warnings by hand.
"""
import argparse
import json
import os
import shlex
import sys

import clang.cindex as ci

LIBCLANG = os.environ.get('LIBCLANG', '/usr/lib/llvm-19/lib/libclang-19.so.1')


def compile_args(build, path):
    cc = json.load(open(os.path.join(build, 'compile_commands.json')))
    ap = os.path.abspath(path)
    base = os.path.basename(path)
    for e in cc:
        f = e['file']
        if f == ap or os.path.basename(f) == base:
            args = e.get('arguments') or shlex.split(e['command'])
            out = []
            skip = False
            for a in args[1:]:
                if skip:
                    skip = False
                    continue
                if a in ('-o', '-c', '-MF', '-MT', '-MQ'):
                    skip = True
                    continue
                if a == f or a.endswith('.c') or a.startswith('-W') or a in ('-MD', '-MMD'):
                    continue
                out.append(a)
            return out
    sys.exit('no compile command for %s' % path)


def find_record(tu, name):
    """The definition of struct/typedef `name` in this TU."""
    for c in tu.cursor.walk_preorder():
        if c.kind == ci.CursorKind.TYPEDEF_DECL and c.spelling == name:
            t = c.underlying_typedef_type.get_canonical()
            d = t.get_declaration()
            if d.is_definition():
                return d
        if c.kind == ci.CursorKind.STRUCT_DECL and c.spelling == name and c.is_definition():
            return c
    sys.exit('no definition of %s' % name)


def fields(rec):
    """(name, byte offset, type, cursor) of a record's fields, anonymous members flattened."""
    out = []
    for f in rec.get_children():
        if f.kind != ci.CursorKind.FIELD_DECL:
            continue
        off = rec.type.get_offset(f.spelling) // 8 if f.spelling else None
        if not f.spelling or f.is_anonymous():
            # anonymous union/struct member
            d = f.type.get_canonical().get_declaration()
            base = f.get_field_offsetof() // 8
            for (n, o, t, c) in fields(d):
                out.append((n, base + o, t, c))
            continue
        out.append((f.spelling, off, f.type, f))
    return out


def leaves(rec, prefix='', base=0, depth=0):
    """Every member path of rec with its N64 offset and size, outermost first."""
    out = []
    for (n, o, t, c) in fields(rec):
        path = prefix + n
        ct = t.get_canonical()
        out.append((path, base + o, ct.get_size(), ct))
        d = ct.get_declaration()
        if ct.kind == ci.TypeKind.RECORD and d.is_definition() and depth < 6:
            out.extend(leaves(d, path + '.', base + o, depth + 1))
        elif ct.kind == ci.TypeKind.CONSTANTARRAY:
            et = ct.element_type.get_canonical()
            ed = et.get_declaration()
            if et.kind == ci.TypeKind.RECORD and ed.is_definition() and depth < 6:
                for k in range(ct.element_count):
                    out.extend(leaves(ed, '%s[%d].' % (path, k), base + o + k * et.get_size(), depth + 1))
    return out


def locate(lv, off, size):
    """The deepest member containing [off, off+size), as (path, delta)."""
    best = None
    for (p, o, s, t) in lv:
        if o <= off and off + size <= o + s:
            if best is None or s <= best[2]:
                best = (p, o, s, t)
    if best is None:
        return None
    p, o, s, t = best
    delta = off - o
    if t.kind == ci.TypeKind.CONSTANTARRAY and delta:
        es = t.element_type.get_canonical().get_size()
        if delta % es == 0:
            return ('%s[%d]' % (p, delta // es), 0, t.element_type.get_canonical().get_size())
    return (p, delta, s)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--build', required=True)
    ap.add_argument('--host-target', default='x86_64-linux-gnu')
    ap.add_argument('file')
    ap.add_argument('view')
    ap.add_argument('real')
    ap.add_argument('--anchor', default=None,
                    help='the view starts at this member of REAL (a symbol inside the struct, e.g. '
                         'gLevelScript_spawnsLeft = LevelScriptState.spawnsLeft): view offset 0 is '
                         'that member, the host offsets are relative to it')
    ap.add_argument('--whole', action='store_true',
                    help='make the host view at least as large as REAL (from the anchor on): a '
                         'buffer the callee fills as REAL (a stack CAITargets)')
    ap.add_argument('--apply', action='store_true',
                    help='rewrite the file: the host view under #ifdef TARGET_PORT, the N64 one in #else')
    a = ap.parse_args()
    ci.Config.set_library_file(LIBCLANG)
    idx = ci.Index.create()
    args = compile_args(a.build, a.file) + ['-w']
    tm = idx.parse(a.file, args=args + ['-target', 'mips-linux-gnu'])
    th = idx.parse(a.file, args=args + ['-target', a.host_target])
    vm = find_record(tm, a.view)
    rm = find_record(tm, a.real)
    rh = find_record(th, a.real)
    lvm = leaves(rm)
    lvh = {p: (o, s) for (p, o, s, t) in leaves(rh)}
    anchor_n = 0
    if a.anchor:
        hits = [o for (p, o, s, t) in lvm if p == a.anchor]
        if not hits:
            sys.exit('no member %s in %s' % (a.anchor, a.real))
        anchor_n = hits[0]
    src = open(a.file).read().encode()
    R = a.real
    lines = []
    warn = []
    for (n, o, t, c) in fields(vm):
        if n.startswith('pad') or n.startswith('_pad'):
            continue
        size = t.get_canonical().get_size()
        loc = locate(lvm, anchor_n + o, max(size, 1))
        if loc is None:
            warn.append('%s at 0x%X: no member of %s there' % (n, o, R))
            continue
        path, delta, rsize = loc
        hoff = 'T2_HOFF(%s)%s%s' % (path, (' + %d' % delta) if delta else '',
                                    (' - T2_HOFF(%s)' % a.anchor) if a.anchor else '')
        decl = src[c.extent.start.offset:c.extent.end.offset].decode()
        lines.append('        struct { u8 _hp_%s[%s]; %s; }; /* N64 0x%X */' % (n, hoff, decl, o))
        if delta == 0 and size != rsize:
            warn.append('%s (0x%X, %d bytes) over %s (%d bytes)' % (n, o, size, path, rsize))
    mac = 'T2_HOFF_' + a.view
    out = ['/* the view at the host offsets of %s (tools/port/hostview.py): each field in its own' % R,
           ' * member of one union, placed by __builtin_offsetof of the member it reads */',
           '#define %s(m) __builtin_offsetof(%s, m)' % (mac, R),
           '    union {']
    if a.whole:
        lines.append('        u8 _hp_whole[sizeof(%s)%s]; /* the whole %s */' % (
            R, (' - T2_HOFF(%s)' % a.anchor) if a.anchor else '', R))
    out += [l.replace('T2_HOFF(', mac + '(') for l in lines]
    out += ['    };']
    for w in warn:
        print('WARNING: ' + w, file=sys.stderr)
    if not a.apply:
        print('\n'.join(out))
        return
    # the definition's extent (with its typedef when it has one)
    text = src.decode()
    td = None
    for c in tm.cursor.get_children():
        if c.location.file is None or os.path.abspath(c.location.file.name) != os.path.abspath(a.file):
            continue
        if c.kind == ci.CursorKind.TYPEDEF_DECL and c.spelling == a.view:
            kids = list(c.get_children())
            if kids and kids[0].is_definition() and kids[0].kind == ci.CursorKind.STRUCT_DECL:
                td = c
    if td is not None:
        b, e = td.extent.start.offset, td.extent.end.offset
        tag = list(td.get_children())[0].spelling
        head = 'typedef struct %s' % tag if tag and not tag.startswith('(') and 'unnamed' not in tag else 'typedef struct'
        tail = ' %s;' % a.view
    else:
        b, e = vm.extent.start.offset, vm.extent.end.offset
        head = 'struct %s' % vm.spelling
        tail = ';'
    while text[e] != ';':
        e += 1
    e += 1
    orig = text[b:e]
    host = '#ifdef TARGET_PORT\n%s {\n%s\n}%s\n#else\n%s\n#endif' % (head, '\n'.join(out), tail, orig)
    open(a.file, 'w').write(text[:b] + host + text[e:])
    print('%s: %s rewritten (%d fields)' % (a.file, a.view, len(lines)))


if __name__ == '__main__':
    main()
