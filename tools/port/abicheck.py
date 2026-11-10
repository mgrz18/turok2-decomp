#!/usr/bin/env python3
"""ABI checker: calls whose types disagree with the callee's DEFINITION (docs/port/ABI.md).

On MIPS o32 matched C can get away with a caller and a callee that do not agree on
the types: a struct by value is "some words in a0..a3", a float through an int
prototype still lands where the callee looks often enough, a missing argument is
whatever the caller left in the register. On x86-64 SysV and AArch64 (AAPCS64 and
Apple's variant) they are passed in different registers, so the callee reads garbage.

This tool parses every C file of the port build (compile_commands.json, the linked
targets only: turok2_game, t2runtime, t2audio, t2placeholders) with libclang, records
each function DEFINITION (return/parameter ABI classes; a K&R definition's parameters
take the default promotions) and each direct CALL (the converted argument types and
the return type of the declaration the caller sees, and whether the result is used),
and compares them across translation units. Classes (most harmful first):

  struct_class    a struct by value whose x86-64 eightbyte classes or AArch64 class
                  (HFA / general registers / indirect) or size differ (Shared_Quad vs
                  CQuaternion, Triple vs CVector3)
  struct_scalar   a struct by value on one side, a scalar/pointer on the other (a
                  by-value return that a prototype turned into a hidden result pointer)
  float_int       float/double vs integer/pointer (rax vs xmm0, x0 vs s0)
  float_double    f32 vs f64 (an f32 through an unprototyped call arrives as a double)
  ptr_from_int    the callee takes/returns a pointer or 64-bit value, the caller passes
                  or reads a 32-bit int: the upper half is garbage / truncated
  int_from_ptr    the callee takes a 32-bit int, the caller passes a pointer (truncated;
                  harmless when the callee only uses it as an integer)
  void_used       the definition returns void, a caller uses the result
  noreturn_used   a non-void definition that falls off its end (-Wreturn-type) and whose
                  result some caller uses
  missing_args    fewer arguments than the definition's parameters (MIPS "pass-through")
  variadic        variadic declaration vs non-variadic definition or the reverse (Apple
                  arm64 passes variadic arguments on the stack)
  extra_args      more arguments than the definition takes (harmless on both ABIs)
  narrow_ext      s8/s16/u8/u16 vs a wider or other-signed int (low risk)
  struct_layout   same ABI classes, different member layout (a type pun, no ABI change),
                  or two integer structs of <= 8 bytes (one general register, same bytes)
  struct_word     an integer struct of <= 8 bytes vs a 32-bit int: the same register, the
                  int is the struct's first four bytes (what MIPS read as the first word;
                  mind the halfword order of a struct of two s16)

Indirect calls (calls through function pointers; section 5 of ABI.md) are checked in two
views: STORE compares each function whose address is converted to a prototyped
function-pointer type with that type; CALL matches each call through a pointer with the
functions stored where the pointer is loaded from (the same global, the same struct
field, a local/parameter followed back to its load in the same function) and compares the
call's arguments with their definitions.

  python3 tools/port/abicheck.py --build OUT/build [-j 4] [--json out.json]
         [--detail CLASS ...] [--files PATTERN ...] [--callee NAME ...] [--quiet]

"""
import argparse, collections, json, os, re, shlex, sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
TARGETS = ('turok2_game', 't2runtime', 't2audio', 't2placeholders', 'turok2_data')
if sys.platform == 'darwin':
    DEFAULT_LIBCLANG = '/Library/Developer/CommandLineTools/usr/lib/libclang.dylib'
else:
    DEFAULT_LIBCLANG = '/usr/lib/llvm-19/lib/libclang-19.so.1'

ORDER = ['struct_class', 'struct_scalar', 'float_int', 'float_double', 'ptr_from_int',
         'void_used', 'noreturn_used', 'missing_args', 'variadic', 'int_from_ptr',
         'extra_args', 'narrow_ext', 'struct_layout', 'struct_word']
HARMFUL = set(ORDER[:9])

# ---------------------------------------------------------------------------
# worker: one translation unit
# ---------------------------------------------------------------------------
_ci = _index = None


def _init(libclang):
    global _ci, _index
    import clang.cindex as ci
    if libclang:
        ci.Config.set_library_file(libclang)
    _ci = ci
    _index = ci.Index.create()


def _leaves(t, off, out, depth=0):
    """Scalar leaves (offset, size, 'i'|'f') of a record/array/scalar type."""
    K = _ci.TypeKind
    t = t.get_canonical()
    k = t.kind
    if depth > 8:
        out.append((off, max(t.get_size(), 1), 'i'))
        return
    if k == K.RECORD:
        for f in t.get_fields():
            fo = f.get_field_offsetof()
            if fo < 0:
                continue
            if f.is_bitfield():
                out.append((off + fo // 8, max(1, (f.get_bitfield_width() + 7) // 8), 'i'))
                continue
            _leaves(f.type, off + fo // 8, out, depth + 1)
        return
    if k == K.CONSTANTARRAY:
        et = t.element_type
        es = et.get_canonical().get_size()
        n = t.element_count
        if es <= 0:
            return
        for i in range(min(n, 64)):
            _leaves(et, off + i * es, out, depth + 1)
        return
    if k in (K.FLOAT, K.DOUBLE):
        out.append((off, t.get_size(), 'f'))
        return
    out.append((off, max(t.get_size(), 1), 'i'))


def _sig(t):
    """ABI signature of a canonical type: a JSON-able list."""
    K = _ci.TypeKind
    t = t.get_canonical()
    k = t.kind
    if k == K.VOID:
        return ['v']
    if k in (K.FLOAT, K.DOUBLE):
        return ['f', t.get_size()]
    if k == K.LONGDOUBLE:
        return ['x', t.get_size()]
    if k in (K.POINTER, K.BLOCKPOINTER, K.CONSTANTARRAY, K.INCOMPLETEARRAY,
             K.FUNCTIONPROTO, K.FUNCTIONNOPROTO):
        return ['i', 8, 1, 1]
    if k == K.RECORD:
        size = t.get_size()
        if size <= 0:
            return ['?', t.spelling]
        lv = []
        _leaves(t, 0, lv)
        lv.sort()
        layout = ','.join('%d%s%d' % (o, c, s) for o, s, c in lv)
        # x86-64 SysV
        if size > 16 or any(o % s for o, s, c in lv if s in (2, 4, 8)):
            sysv = 'M'
        else:
            cls = []
            for e in range(0, size, 8):
                inside = [c for o, s, c in lv if o < e + 8 and o + s > e]
                cls.append('I' if 'i' in inside or not inside else 'S')
            sysv = ''.join(cls)
        # AArch64: HFA (1..4 members, all the same float type), else <=16 in GPRs, else indirect
        fl = set((c, s) for o, s, c in lv)
        if lv and len(fl) == 1 and next(iter(fl))[0] == 'f' and len(lv) <= 4 \
                and size == len(lv) * lv[0][1]:
            a64 = 'H%d%s' % (len(lv), 's' if lv[0][1] == 4 else 'd')
        elif size > 16:
            a64 = 'M'
        else:
            a64 = 'G%d' % ((size + 7) // 8)
        return ['s', size, sysv, a64, layout]
    if k == K.ENUM:
        return ['i', t.get_size(), 1, 0]
    size = t.get_size()
    if size <= 0:
        return ['?', t.spelling]
    signed = 0 if k in (K.CHAR_U, K.UCHAR, K.USHORT, K.UINT, K.ULONG, K.ULONGLONG,
                        K.BOOL, K.UINT128) else 1
    return ['i', size, signed, 0]


def _promote(s):
    """Default argument promotions on a signature (K&R definitions, unprototyped calls)."""
    if s[0] == 'f' and s[1] == 4:
        return ['f', 8]
    if s[0] == 'i' and s[1] < 4:
        return ['i', 4, 1, 0]
    return s


def _parse_args(cmd):
    args = shlex.split(cmd)[1:]
    out, skip = [], 0
    for a in args:
        if skip:
            skip -= 1
            continue
        if a in ('-o', '-c'):
            skip = 1
            continue
        if a.startswith('-W') or a == '-w':
            continue
        out.append(a)
    return out


def _loc(c):
    f = c.location.file
    return '%s:%d' % (os.path.relpath(f.name, ROOT) if f else '?', c.location.line)


STMT_PARENTS = None


def _used(parent, child, ck):
    """Is the value of CALL_EXPR `child` used (not a statement on its own)?"""
    K = _ci.CursorKind
    if parent is None:
        return False
    pk = parent.kind
    if pk in (K.COMPOUND_STMT, K.LABEL_STMT, K.CASE_STMT, K.DEFAULT_STMT):
        return False
    if pk == K.CSTYLE_CAST_EXPR and parent.type.kind == _ci.TypeKind.VOID:
        return False
    if pk in (K.IF_STMT, K.WHILE_STMT, K.FOR_STMT, K.DO_STMT, K.SWITCH_STMT):
        kids = list(parent.get_children())
        if pk == K.IF_STMT or pk == K.WHILE_STMT or pk == K.SWITCH_STMT:
            return bool(kids) and kids[0] == child
        if pk == K.DO_STMT:
            return bool(kids) and kids[-1] == child
        # for(;;): init/cond/inc are all expressions; the body is the last child
        return not (kids and kids[-1] == child)
    return True


def _work(job):
    path, cmd = job
    K = _ci.CursorKind
    args = _parse_args(cmd) + ['-Wno-everything', '-Wreturn-type']
    res = {'file': os.path.relpath(path, ROOT), 'defs': [], 'calls': [], 'fptr': 0,
           'icalls': [], 'stores': [], 'noreturn': [], 'error': None}
    try:
        tu = _index.parse(path, args=args)
    except Exception as ex:  # noqa: BLE001
        res['error'] = str(ex)
        return res
    for d in tu.diagnostics:
        if d.severity >= 4:
            res['error'] = '%s:%d %s' % (os.path.basename(path), d.location.line, d.spelling)
        if 'does not return a value' in d.spelling or 'non-void function' in d.spelling:
            res['noreturn'].append([d.location.line, d.spelling])
    main = tu.cursor.spelling

    def fdef(c):
        ft = c.type.get_canonical()
        proto = ft.kind == _ci.TypeKind.FUNCTIONPROTO
        params = [_sig(a.type) for a in c.get_arguments()]
        if not proto:
            params = [_promote(p) for p in params]
        res['defs'].append({
            'name': c.spelling, 'static': c.linkage == _ci.LinkageKind.INTERNAL,
            'loc': _loc(c), 'line': c.extent.start.line, 'end': c.extent.end.line,
            'ret': _sig(c.result_type), 'params': params,
            'pspell': [a.type.spelling for a in c.get_arguments()],
            'rspell': c.result_type.spelling,
            'variadic': proto and ft.is_function_variadic(), 'proto': proto})

    def call(c, parent, anc):
        ref = c.referenced
        if ref is None or ref.kind != K.FUNCTION_DECL:
            res['fptr'] += 1
            icall(c, parent, anc)
            return
        name = ref.spelling
        if name.startswith('__builtin'):
            return
        # an asm label (`s32 f_alias(s32) __asm__("Real")`) calls the function it names
        try:
            mn = ref.mangled_name or ''
        except Exception:  # noqa: BLE001
            mn = ''
        mn = mn.lstrip('\x01')
        if sys.platform == 'darwin' and mn.startswith('_'):
            mn = mn[1:]
        if mn and mn != name and re.match(r'^[A-Za-z_]\w*$', mn):
            name = mn
        ft = ref.type.get_canonical()
        proto = ft.kind == _ci.TypeKind.FUNCTIONPROTO
        argsx = list(c.get_arguments())
        sigs = []
        for i, a in enumerate(argsx):
            s = _sig(a.type)
            if not proto or (ft.is_function_variadic() and i >= len(list(ft.argument_types()))):
                s = _promote(s)
            sigs.append(s)
        dl = ref.location
        res['calls'].append({
            'callee': name, 'static': ref.linkage == _ci.LinkageKind.INTERNAL,
            'loc': _loc(c), 'ret': _sig(c.type), 'args': sigs,
            'aspell': [a.type.spelling for a in argsx], 'rspell': c.type.spelling,
            'proto': proto, 'variadic': proto and ft.is_function_variadic(),
            'implicit': ref.location.file is None,
            'decl': ('%s:%d' % (os.path.relpath(dl.file.name, ROOT), dl.line)) if dl.file else 'implicit',
            'used': _used(parent, c, K)})

    def icall(c, parent, anc):
        # a call through a function pointer: the pointer's type and where it comes from
        kids = list(c.get_children())
        if not kids:
            return
        callee = kids[0]
        pt = callee.type.get_canonical()
        if pt.kind == _ci.TypeKind.POINTER:
            pt = pt.get_pointee().get_canonical()
        if pt.kind not in (_ci.TypeKind.FUNCTIONPROTO, _ci.TypeKind.FUNCTIONNOPROTO):
            return
        proto = pt.kind == _ci.TypeKind.FUNCTIONPROTO
        ptypes = list(pt.argument_types()) if proto else []
        argsx = list(c.get_arguments())
        sigs = []
        for i, a in enumerate(argsx):
            s_ = _sig(a.type)
            if not proto or (pt.is_function_variadic() and i >= len(ptypes)):
                s_ = _promote(s_)
            sigs.append(s_)
        res['icalls'].append({
            'loc': _loc(c), 'keys': _src_keys(callee), 'ftype': _fspell(pt),
            'proto': proto, 'variadic': proto and pt.is_function_variadic(),
            'ret': _sig(c.type), 'rspell': c.type.spelling, 'args': sigs,
            'aspell': [a.type.spelling for a in argsx],
            'tparams': [_sig(t) for t in ptypes], 'tspell': [t.spelling for t in ptypes],
            'expr': _text(callee), 'used': _used(parent, c, K),
            'func': anc[0].spelling if anc else '?'})

    def store(c, anc):
        # a function named outside a direct call: where does its address go?
        i = len(anc) - 1
        x = c
        while i >= 0 and anc[i].kind in (K.UNEXPOSED_EXPR, K.PAREN_EXPR, K.CSTYLE_CAST_EXPR) \
                or (i >= 0 and anc[i].kind == K.UNARY_OPERATOR and _optok(anc[i]) == '&'):
            x = anc[i]
            i -= 1
        p = anc[i] if i >= 0 else None
        if p is not None and p.kind == K.CALL_EXPR and next(p.get_children(), None) == x:
            return                                  # the direct call itself
        dt = x.type.get_canonical()
        dproto = None
        dsig = None
        if dt.kind == _ci.TypeKind.POINTER:
            pe = dt.get_pointee().get_canonical()
            if pe.kind in (_ci.TypeKind.FUNCTIONPROTO, _ci.TypeKind.FUNCTIONNOPROTO):
                dproto = pe.kind == _ci.TypeKind.FUNCTIONPROTO
                dsig = {'ret': _sig(pe.get_result()),
                        'params': [_sig(t) for t in pe.argument_types()] if dproto else [],
                        'variadic': dproto and pe.is_function_variadic(),
                        'pspell': [t.spelling for t in pe.argument_types()] if dproto else [],
                        'rspell': pe.get_result().spelling}
        keys = []
        ctx = '?'
        if p is not None and p.kind == K.BINARY_OPERATOR and _optok(p) == '=':
            lhs = next(p.get_children(), None)
            if lhs is not None and lhs != x:
                keys = _src_keys(lhs)
                ctx = 'assign ' + _text(lhs)
        elif p is not None and p.kind == K.CALL_EXPR:
            r = p.referenced
            ai = [a for a in p.get_arguments()]
            idx = ai.index(x) if x in ai else -1
            nm = r.spelling if r is not None else _text(next(p.get_children()))
            keys = ['arg:%s#%d' % (nm, idx)]
            ctx = 'argument %d of %s' % (idx + 1, nm)
        else:
            for a in reversed(anc):
                if a.kind == K.VAR_DECL:
                    keys = ['var:' + a.spelling]
                    et = a.type.get_canonical()
                    while et.kind in (_ci.TypeKind.CONSTANTARRAY, _ci.TypeKind.INCOMPLETEARRAY):
                        et = et.element_type.get_canonical()
                    if et.kind == _ci.TypeKind.RECORD:
                        keys.append('rec:' + et.spelling)
                    ctx = 'initializer of ' + a.spelling
                    break
                if a.kind == K.RETURN_STMT:
                    ctx = 'returned'
                    break
        res['stores'].append({
            'name': c.referenced.spelling, 'loc': _loc(c), 'keys': keys, 'ctx': ctx,
            'dtype': _fspell(dt.get_pointee()) if dsig else dt.spelling, 'dsig': dsig,
            'func': anc[0].spelling if anc and anc[0].kind == K.FUNCTION_DECL else None})

    locals_ = collections.defaultdict(list)    # local/param name -> source keys

    def walk(c, parent, anc):
        k = c.kind
        if k == K.VAR_DECL and anc:
            kids = list(c.get_children())
            if kids and kids[-1].kind not in (K.TYPE_REF, K.PARM_DECL):
                locals_[c.spelling].extend(_src_keys(kids[-1]))
        elif k == K.BINARY_OPERATOR and _optok(c) == '=':
            kids = list(c.get_children())
            if len(kids) == 2 and kids[0].kind == K.DECL_REF_EXPR and kids[0].referenced is not None \
                    and kids[0].referenced.kind in (K.VAR_DECL, K.PARM_DECL):
                locals_[kids[0].spelling].extend(_src_keys(kids[1]))
        if k == K.CALL_EXPR:
            # a call through a function pointer has no FUNCTION_DECL referenced
            call(c, parent, anc)
        elif k == K.DECL_REF_EXPR:
            r = c.referenced
            if r is not None and r.kind == K.FUNCTION_DECL and not r.spelling.startswith('__builtin'):
                store(c, anc)
        anc.append(c)
        for ch in c.get_children():
            walk(ch, c, anc)
        anc.pop()

    for c in tu.cursor.get_children():
        f = c.location.file
        if f is None or f.name != main:
            continue
        if c.kind == K.FUNCTION_DECL and c.is_definition():
            fdef(c)
            locals_.clear()
            n0 = len(res['icalls'])
            walk(c, None, [])
            for ic in res['icalls'][n0:]:
                # a local/parameter pointer: where it was loaded from in this function
                extra, todo, done = [], list(ic['keys']), set()
                while todo:
                    kk = todo.pop()
                    if kk in done:
                        continue
                    done.add(kk)
                    if kk.startswith(('local:', 'param:')):
                        for k2 in locals_.get(kk.split(':', 1)[1], []):
                            if k2 not in ic['keys'] and k2 not in extra:
                                extra.append(k2)
                                todo.append(k2)
                ic['keys'] = ic['keys'] + extra
        elif c.kind == K.VAR_DECL:
            walk(c, None, [])
    return res


def _text(c, n=12):
    try:
        return ' '.join(t.spelling for t in list(c.get_tokens())[:n])
    except Exception:  # noqa: BLE001
        return '?'


def _optok(c):
    # the operator token of a unary/binary operator cursor
    try:
        ts = [t.spelling for t in c.get_tokens()]
    except Exception:  # noqa: BLE001
        return '?'
    K = _ci.CursorKind
    if c.kind == K.UNARY_OPERATOR:
        return ts[0] if ts else '?'
    kids = list(c.get_children())
    if kids:
        n = len(list(kids[0].get_tokens()))
        return ts[n] if len(ts) > n else '?'
    return '?'


def _fspell(t):
    return t.get_canonical().spelling


def _src_keys(e):
    """Where a function-pointer value comes from: var:<global>, rec:<struct>.<field>, ..."""
    K = _ci.CursorKind
    keys = []
    depth = 0
    while e is not None and depth < 16:
        depth += 1
        k = e.kind
        if k in (K.UNEXPOSED_EXPR, K.PAREN_EXPR, K.CSTYLE_CAST_EXPR):
            kids = list(e.get_children())
            e = kids[-1] if kids else None
            continue
        if k == K.UNARY_OPERATOR:
            e = next(e.get_children(), None)
            continue
        if k == K.ARRAY_SUBSCRIPT_EXPR:
            e = next(e.get_children(), None)
            continue
        if k == K.MEMBER_REF_EXPR:
            r = e.referenced
            if r is not None and r.semantic_parent is not None:
                keys.append('rec:%s.%s' % (r.semantic_parent.type.get_canonical().spelling,
                                           r.spelling))
            e = next(e.get_children(), None)
            continue
        if k == K.DECL_REF_EXPR:
            r = e.referenced
            if r is not None and r.kind == K.VAR_DECL:
                if r.semantic_parent is not None and r.semantic_parent.kind == K.TRANSLATION_UNIT \
                        or r.storage_class == _ci.StorageClass.STATIC:
                    keys.append('var:' + r.spelling)
                else:
                    keys.append('local:' + r.spelling)
            elif r is not None and r.kind == K.PARM_DECL:
                keys.append('param:' + r.spelling)
            break
        if k == K.CALL_EXPR:
            r = e.referenced
            keys.append('ret:' + (r.spelling if r is not None else '?'))
            break
        break
    return keys


# ---------------------------------------------------------------------------
# comparison
# ---------------------------------------------------------------------------

def cmp_sig(a, b):
    """a: what the caller passes/reads, b: what the definition takes/returns."""
    if a == b:
        return None
    ka, kb = a[0], b[0]
    if '?' in (ka, kb):
        return None
    if ka == 's' and kb == 's':
        if a[1] != b[1] or a[2] != b[2] or a[3] != b[3]:
            if a[2] == b[2] == 'I' and a[3] == b[3] == 'G1':
                return 'struct_layout'      # both one general register, the same bytes
            return 'struct_class'
        return 'struct_layout' if a[4] != b[4] else None
    if 's' in (ka, kb):
        st, sc = (a, b) if ka == 's' else (b, a)
        if st[2] == 'I' and st[3] == 'G1' and sc[0] == 'i' and sc[1] <= 4:
            # an integer struct of <= 8 bytes and a 32-bit int share the register (x86-64 and
            # AArch64): the int is the struct's first four bytes, as MIPS read the first word
            return 'struct_word'
        return 'struct_scalar'
    if ka == 'i' and kb == 'i':
        if a[1] != b[1]:
            if a[1] >= 4 and b[1] >= 4:
                return 'ptr_from_int' if b[1] > a[1] else 'int_from_ptr'
            return 'narrow_ext'
        if a[1] < 4 and a[2] != b[2]:
            return 'narrow_ext'
        return None
    if ka == 'f' and kb == 'f':
        return 'float_double' if a[1] != b[1] else None
    if 'f' in (ka, kb) and 'i' in (ka, kb):
        return 'float_int'
    if 'x' in (ka, kb):
        return 'float_int'
    return None


def check(results):
    defs = {}          # (name, file or None) -> def
    dups = collections.defaultdict(list)
    noreturn = {}      # (file, line range) -> def name
    for r in results:
        for d in r['defs']:
            key = (d['name'], r['file'] if d['static'] else None)
            d['file'] = r['file']
            if key in defs:
                dups[d['name']].append(d['loc'])
                continue
            defs[key] = d
        for line, msg in r['noreturn']:
            for d in r['defs']:
                if d['line'] <= line <= d['end']:
                    d['noreturn'] = True
    findings = []      # dicts: cls, callee, loc, what
    stats = collections.Counter()
    nodef = collections.Counter()
    for r in results:
        stats['fptr_calls'] += r['fptr']
        for c in r['calls']:
            stats['calls'] += 1
            d = defs.get((c['callee'], r['file'] if c['static'] else None))
            if d is None:
                nodef[c['callee']] += 1
                continue
            stats['checked'] += 1
            if not c['proto']:
                stats['unprototyped_calls'] += 1
            if c['implicit']:
                stats['implicit_calls'] += 1
            base = {'callee': c['callee'], 'loc': c['loc'], 'decl': c['decl'],
                    'def': d['loc'], 'proto': c['proto']}

            def add(cls, what):
                f = dict(base)
                f['cls'] = cls
                f['what'] = what
                findings.append(f)

            # return
            if d['ret'][0] == 'v':
                if c['used'] and c['ret'][0] != 'v':
                    add('void_used', 'result (%s) used, the definition returns void' % c['rspell'])
            elif c['used'] or c['ret'][0] == 's' or d['ret'][0] == 's':
                # the definition produces the value, the caller consumes it
                cl = cmp_sig(d['ret'], c['ret']) if c['ret'][0] != 'v' else (
                    'struct_scalar' if d['ret'][0] == 's' else None)
                if cl:
                    add(cl, 'return: caller %s, definition %s' % (c['rspell'], d['rspell']))
                if c['used'] and d.get('noreturn'):
                    add('noreturn_used', 'result used, the definition can fall off its end')
            # variadic
            if c['variadic'] != d['variadic'] and (c['proto'] or d['variadic']):
                add('variadic', 'declaration %svariadic, definition %svariadic'
                    % ('' if c['variadic'] else 'not ', '' if d['variadic'] else 'not '))
            # arguments
            na, nd = len(c['args']), len(d['params'])
            for i in range(min(na, nd)):
                cl = cmp_sig(c['args'][i], d['params'][i])
                if cl:
                    add(cl, 'arg %d: caller %s, definition %s'
                        % (i + 1, c['aspell'][i], d['pspell'][i] if i < len(d['pspell']) else '?'))
            if na < nd and not d['variadic']:
                add('missing_args', '%d argument(s), the definition takes %d' % (na, nd))
            elif na > nd and not d['variadic'] and (d['proto'] or nd):
                add('extra_args', '%d argument(s), the definition takes %d' % (na, nd))
    return defs, dups, findings, stats, nodef


def _cmp_call(c, d, add):
    """Compare one call view c (args/ret/used/variadic/proto) with definition d."""
    if d['ret'][0] == 'v':
        if c['used'] and c['ret'][0] != 'v':
            add('void_used', 'result (%s) used, the definition returns void' % c['rspell'])
    elif c['used'] or c['ret'][0] == 's' or d['ret'][0] == 's':
        cl = cmp_sig(d['ret'], c['ret']) if c['ret'][0] != 'v' else (
            'struct_scalar' if d['ret'][0] == 's' else None)
        if cl:
            add(cl, 'return: caller %s, definition %s' % (c['rspell'], d['rspell']))
        if c['used'] and d.get('noreturn'):
            add('noreturn_used', 'result used, the definition can fall off its end')
    if c['variadic'] != d['variadic'] and (c['proto'] or d['variadic']):
        add('variadic', 'declaration %svariadic, definition %svariadic'
            % ('' if c['variadic'] else 'not ', '' if d['variadic'] else 'not '))
    na, nd = len(c['args']), len(d['params'])
    for i in range(min(na, nd)):
        cl = cmp_sig(c['args'][i], d['params'][i])
        if cl:
            add(cl, 'arg %d: caller %s, definition %s'
                % (i + 1, c['aspell'][i], d['pspell'][i] if i < len(d['pspell']) else '?'))
    if na < nd and not d['variadic']:
        add('missing_args', '%d argument(s), the definition takes %d' % (na, nd))
    elif na > nd and not d['variadic'] and (d['proto'] or nd):
        add('extra_args', '%d argument(s), the definition takes %d' % (na, nd))


def check_indirect(results, defs):
    """Calls through function pointers against the functions stored where they load from.

    Two views. STORE: a function whose address is converted to a prototyped function-pointer
    type (a table's element, a struct field, a parameter) is compared with that type, as if
    the type were a caller. CALL: each call through a pointer is matched with the functions
    stored where the pointer comes from (the same global table `var:`, the same struct field
    `rec:S.f`, or an initializer of a variable of struct S whose slot has the call's type;
    a parameter/local falls back to every function stored with the call's exact type) and
    compared argument by argument with their definitions (this catches the tables typed
    void*/u32 that the STORE view cannot see)."""
    stores = collections.defaultdict(list)
    allstores = []
    for r in results:
        for st in r.get('stores', []):
            st['file'] = r['file']
            allstores.append(st)
            for k in st['keys']:
                stores[k].append(st)
            stores['type:' + st['dtype']].append(st)

    def getdef(name, file):
        return defs.get((name, file)) or defs.get((name, None))

    sfind, cfind = [], []
    seen = set()
    for st in allstores:
        d = getdef(st['name'], st['file'])
        if d is None or not st['dsig']:
            continue
        key = (st['name'], st['dtype'])
        if key in seen:
            continue
        seen.add(key)
        ds = st['dsig']
        view = {'args': ds['params'], 'aspell': ds['pspell'], 'ret': ds['ret'],
                'rspell': ds['rspell'], 'used': ds['ret'][0] != 'v', 'proto': True,
                'variadic': ds['variadic']}
        if not ds['params'] and not ds['variadic'] and st['dtype'].endswith('()'):
            continue    # unprototyped pointer type: nothing to compare here
        def add(cls, what, st=st, d=d):
            sfind.append({'cls': cls, 'callee': st['name'], 'loc': st['loc'], 'what': what,
                          'def': d['loc'], 'decl': st['dtype'], 'ctx': st['ctx']})
        _cmp_call(view, d, add)

    unmatched = 0
    for r in results:
        for c in r.get('icalls', []):
            cands = {}
            how = None
            for k in c['keys']:
                if k.startswith('var:') or (k.startswith('rec:') and '.' in k):
                    for st in stores.get(k, []):
                        if st['dsig'] and st['dtype'] != c['ftype'] and k.startswith('var:') \
                                and any(kk.startswith('rec:') for kk in c['keys']):
                            continue
                        cands.setdefault(st['name'], st)
                    if k.startswith('rec:'):
                        rec = k[4:].rsplit('.', 1)[0]
                        for st in stores.get('rec:' + rec, []):
                            if st['dtype'] == c['ftype'] or not st['dsig']:
                                cands.setdefault(st['name'], st)
                    if cands:
                        how = k
                        break
            if not cands and c['proto'] and (c['args'] or c['ret'][0] != 'v'):
                # last resort: every function stored with exactly the call's pointer type
                for st in stores.get('type:' + c['ftype'], []):
                    cands.setdefault(st['name'], st)
                how = 'type'
            if not cands:
                unmatched += 1
            for name, st in cands.items():
                d = getdef(name, st['file'])
                if d is None:
                    continue
                def add(cls, what, c=c, d=d, name=name, how=how):
                    cfind.append({'cls': cls, 'callee': name, 'loc': c['loc'], 'what': what,
                                  'def': d['loc'], 'decl': '%s via %s' % (c['expr'], how),
                                  'ctx': c['func']})
                _cmp_call(c, d, add)
    return sfind, cfind, unmatched


def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--build', default=os.path.join(ROOT, 'OUT/build'))
    ap.add_argument('--libclang', default=os.environ.get('LIBCLANG', DEFAULT_LIBCLANG))
    ap.add_argument('-j', type=int, default=4)
    ap.add_argument('--json', help='write every finding (and the raw stats) here')
    ap.add_argument('--raw', help='cache of the parsed TUs (read if it exists, else written)')
    ap.add_argument('--detail', nargs='*', default=None,
                    help='list the findings of these classes (no argument: the harmful ones)')
    ap.add_argument('--files', nargs='*', help='only TUs whose path matches one of these regexes '
                    '(definitions from the rest are then missing: use with --raw)')
    ap.add_argument('--callee', nargs='*', help='only findings for these callees')
    ap.add_argument('--quiet', action='store_true')
    a = ap.parse_args()

    if a.raw and os.path.exists(a.raw):
        results = json.load(open(a.raw))
    else:
        cc = json.load(open(os.path.join(a.build, 'compile_commands.json')))
        jobs = []
        for e in cc:
            m = re.search(r'CMakeFiles/([^/]+)\.dir', e['command'])
            if not m or m.group(1) not in TARGETS or not e['file'].endswith('.c'):
                continue
            if a.files and not any(re.search(p, e['file']) for p in a.files):
                continue
            jobs.append((e['file'], e['command']))
        import multiprocessing as mp
        with mp.Pool(a.j, initializer=_init, initargs=(a.libclang,)) as pool:
            results = []
            for i, r in enumerate(pool.imap_unordered(_work, jobs, chunksize=8)):
                results.append(r)
                if not a.quiet and (i + 1) % 500 == 0:
                    print('  %d/%d TUs' % (i + 1, len(jobs)), file=sys.stderr)
        results.sort(key=lambda r: r['file'])
        if a.raw:
            json.dump(results, open(a.raw, 'w'))
    errs = [r for r in results if r['error']]
    defs, dups, findings, stats, nodef = check(results)
    if a.callee:
        findings = [f for f in findings if f['callee'] in a.callee]

    by = collections.Counter(f['cls'] for f in findings)
    callees = collections.defaultdict(set)
    for f in findings:
        callees[f['cls']].add(f['callee'])
    print('abicheck: %d TUs (%d with errors), %d definitions, %d calls (%d checked against a '
          'definition, %d unprototyped, %d implicit), %d function-pointer calls, %d calls to '
          '%d functions without a C definition'
          % (len(results), len(errs), len(defs), stats['calls'], stats['checked'],
             stats['unprototyped_calls'], stats['implicit_calls'], stats['fptr_calls'],
             sum(nodef.values()), len(nodef)))
    print('%-14s %6s %8s  %s' % ('class', 'sites', 'callees', ''))
    for cls in ORDER:
        print('%-14s %6d %8d  %s' % (cls, by[cls], len(callees[cls]),
                                     'harmful' if cls in HARMFUL else 'info'))
    print('%-14s %6d %8d' % ('total harmful', sum(by[c] for c in HARMFUL),
                             len(set().union(*[callees[c] for c in HARMFUL]))))
    if dups:
        print('%d functions defined twice (first kept): %s' % (
            len(dups), ' '.join(sorted(dups)[:20])))
    for r in errs[:10]:
        print('  error: %s %s' % (r['file'], r['error']))
    if a.detail is not None:
        want = set(a.detail) if a.detail else HARMFUL
        for cls in ORDER:
            if cls not in want:
                continue
            fs = [f for f in findings if f['cls'] == cls]
            if not fs:
                continue
            print('\n== %s (%d)' % (cls, len(fs)))
            for f in sorted(fs, key=lambda f: (f['callee'], f['loc'])):
                print('  %-36s %-34s %s   [decl %s, def %s]' % (
                    f['callee'], f['loc'], f['what'], f['decl'], f['def']))
    sfind, cfind, unmatched = check_indirect(results, defs)
    if a.callee:
        sfind = [f for f in sfind if f['callee'] in a.callee]
        cfind = [f for f in cfind if f['callee'] in a.callee]
    nic = sum(len(r.get('icalls', [])) for r in results)
    nst = sum(len(r.get('stores', [])) for r in results)
    print('\nindirect: %d function-pointer calls (%d with no stored function found), %d places '
          'that take a function\'s address' % (nic, unmatched, nst))
    bs = collections.Counter(f['cls'] for f in sfind)
    bc = collections.Counter(f['cls'] for f in cfind)
    print('%-14s %6s %6s %8s' % ('class', 'store', 'call', 'callees'))
    for cls in ORDER:
        cs = set(f['callee'] for f in sfind + cfind if f['cls'] == cls)
        print('%-14s %6d %6d %8d  %s' % (cls, bs[cls], bc[cls], len(cs),
                                          'harmful' if cls in HARMFUL else 'info'))
    print('%-14s %6d %6d' % ('total harmful', sum(bs[c] for c in HARMFUL),
                             sum(bc[c] for c in HARMFUL)))
    if a.detail is not None:
        want = set(a.detail) if a.detail else HARMFUL
        for title, fl in (('store', sfind), ('call', cfind)):
            for cls in ORDER:
                fs = [f for f in fl if f['cls'] == cls and cls in want]
                if not fs:
                    continue
                print('\n== indirect %s %s (%d)' % (title, cls, len(fs)))
                for f in sorted(fs, key=lambda f: (f['callee'], f['loc'])):
                    print('  %-36s %-34s %s   [%s; %s; def %s]' % (
                        f['callee'], f['loc'], f['what'], f['decl'], f['ctx'], f['def']))
    if a.json:
        json.dump({'stats': dict(stats), 'counts': dict(by), 'findings': findings,
                   'indirect_store': sfind, 'indirect_call': cfind,
                   'nodef': dict(nodef)}, open(a.json, 'w'), indent=1)


if __name__ == '__main__':
    main()
