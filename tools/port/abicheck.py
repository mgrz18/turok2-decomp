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
  struct_layout   same ABI classes, different member layout (a type pun, no ABI change)

  python3 tools/port/abicheck.py --build OUT/build [-j 4] [--json out.json]
         [--detail CLASS ...] [--files PATTERN ...] [--callee NAME ...] [--quiet]

Function-pointer calls are counted but not checked (no definition to compare).
"""
import argparse, collections, json, os, re, shlex, sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
TARGETS = ('turok2_game', 't2runtime', 't2audio', 't2placeholders')
if sys.platform == 'darwin':
    DEFAULT_LIBCLANG = '/Library/Developer/CommandLineTools/usr/lib/libclang.dylib'
else:
    DEFAULT_LIBCLANG = '/usr/lib/llvm-19/lib/libclang-19.so.1'

ORDER = ['struct_class', 'struct_scalar', 'float_int', 'float_double', 'ptr_from_int',
         'void_used', 'noreturn_used', 'missing_args', 'variadic', 'int_from_ptr',
         'extra_args', 'narrow_ext', 'struct_layout']
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
           'noreturn': [], 'error': None}
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

    def call(c, parent):
        ref = c.referenced
        if ref is None or ref.kind != K.FUNCTION_DECL:
            res['fptr'] += 1
            return
        name = ref.spelling
        if name.startswith('__builtin'):
            return
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

    def walk(c, parent):
        k = c.kind
        if k == K.CALL_EXPR:
            # a call through a function pointer has no FUNCTION_DECL referenced
            call(c, parent)
        for ch in c.get_children():
            walk(ch, c)

    for c in tu.cursor.get_children():
        f = c.location.file
        if f is None or f.name != main:
            continue
        if c.kind == K.FUNCTION_DECL and c.is_definition():
            fdef(c)
            walk(c, None)
        elif c.kind == K.VAR_DECL:
            walk(c, None)
    return res


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
            return 'struct_class'
        return 'struct_layout' if a[4] != b[4] else None
    if 's' in (ka, kb):
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
    if a.json:
        json.dump({'stats': dict(stats), 'counts': dict(by), 'findings': findings,
                   'nodef': dict(nodef)}, open(a.json, 'w'), indent=1)


if __name__ == '__main__':
    main()
