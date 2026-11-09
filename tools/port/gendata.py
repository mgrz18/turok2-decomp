#!/usr/bin/env python3
"""gendata.py: the game's data/rodata/bss as C for the native port (docs/port/DATA.md).

The 3,917 game C files reference ~1,600 data symbols that the matching build
defines in assembly (us/asm/data, splat output) or by absolute address in the
linker script. This tool defines them in C, under port/gen/:

  data_<region>.c   one C definition per symbol, laid out for the HOST
                    (typed from the C declarations the game uses), with
                    pointers as real host pointers (relocations);
  relocs.c          t2_n64_to_host(): N64 address -> host address, for the
                    integer slots that still carry an N64 address;
  data_abs.c        ROM offsets the code takes the address of (&D_14A000);
  data_report.txt   what was generated, conflicts, unresolved pointers.

Inputs (none are written):
  build/turok2.us.elf           the matching link: symbols, section bytes
                                (the data contents ARE the ROM bytes);
  <port build>/compile_commands.json + libturok2_game.a
                                which symbols the game C needs and how the
                                C declares them (libclang, parsed twice:
                                x86_64 for the host layout, mips for the
                                N64 layout of the same type).

How a symbol becomes C:
  * its declared type (most common declaration across the files that use it)
    is walked leaf by leaf in both layouts; each N64 leaf is read big-endian
    from the ELF and written little-endian at its host offset (ints/floats
    swapped by size, bitfields re-packed, chars as bytes);
  * a pointer leaf (any pointer type) becomes a host pointer: the N64 value is
    looked up in the symbol table (containing symbol + offset) and emitted as
    `(char *)&sym + host_offset`; the target is generated too (typed by the
    pointee type when it has no declaration of its own);
  * an integer leaf keeps its N64 value, even when it holds an address (m2c
    `s32` pointers): that value is the HANDLE; relocs.c maps it back with
    t2_n64_to_host() for code that has been taught to do so;
  * symbols nobody declares (reached only through `void *`/`char *`) are
    copied as bytes in ROM order (strings stay strings);
  * .bss (no bytes in the ELF) becomes a zero array of the declared size;
  * a symbol inside another (splat put a label in the middle of a struct the
    C declares whole, e.g. gScene_m_nDrawActors) is an alias at the host
    offset (ELF `.set`; on other object formats a separate copy, reported).

Module 2 (.virtual_1) is dead in the port: its objects are left out of the
symbol scan and none of its data is generated.

Usage:
  python3 tools/port/gendata.py --build <port build dir> [--out port/gen] [-j 32]
Needs the python `clang` bindings matching the system libclang
(`pip install clang==19.1.7`; libclang path via --libclang or LIBCLANG).
"""
import argparse
import bisect
import collections
import json
import multiprocessing
import os
import re
import shlex
import struct
import subprocess
import sys

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..'))

# ---------------------------------------------------------------------------
# libclang worker: declared types, in both layouts
# ---------------------------------------------------------------------------

_ci = None
_index = None
_targets = None


def _init_worker(libclang, targets):
    global _ci, _index, _targets
    import clang.cindex as ci
    if libclang:
        ci.Config.set_library_file(libclang)
    _ci = ci
    _index = ci.Index.create()
    _targets = targets


def _scalar_kind(t):
    K = _ci.TypeKind
    k = t.kind
    if k in (K.FLOAT, K.DOUBLE, K.LONGDOUBLE):
        return 'f'
    if k in (K.CHAR_U, K.UCHAR, K.USHORT, K.UINT, K.ULONG, K.ULONGLONG, K.BOOL):
        return 'u'
    if k == K.ENUM:
        return 'i'
    return 'i'


_types = None  # per-TU memo: type key -> description


def _tkey(t):
    return t.spelling


def _desc(th, tm, depth, stack):
    """Describe a type in both layouts. th: host type, tm: mips type.

    Pointees are not inlined: a pointer is ['p', key] and the pointee's
    description goes once into the per-TU table _types[key] (bounded size).
    """
    K = _ci.TypeKind
    th = th.get_canonical()
    tm = tm.get_canonical()
    k = th.kind
    if k == K.POINTER:
        ph = th.get_pointee().get_canonical()
        pm = tm.get_pointee().get_canonical()
        if ph.kind in (K.FUNCTIONPROTO, K.FUNCTIONNOPROTO):
            return ['p', 'F']
        if ph.kind == K.VOID:
            return ['p', None]
        key = _tkey(ph)
        if key not in _types:
            _types[key] = None  # placeholder: breaks cycles
            _types[key] = _desc(ph, pm, depth + 1, frozenset())
        return ['p', key]
    if k in (K.CONSTANTARRAY, K.INCOMPLETEARRAY, K.VARIABLEARRAY, K.DEPENDENTSIZEDARRAY):
        n = th.element_count if k == K.CONSTANTARRAY else -1
        return ['a', n, _desc(th.element_type, tm.element_type, depth, stack)]
    if k == K.RECORD:
        decl = th.get_declaration()
        key = th.spelling
        hs, ms = th.get_size(), tm.get_size()
        if hs < 0 or ms < 0 or key in stack:
            return ['x', max(ms, 0), max(hs, 0)]
        union = decl.kind == _ci.CursorKind.UNION_DECL
        fh = list(th.get_fields())
        fm = list(tm.get_fields())
        if len(fh) != len(fm):
            return ['x', ms, hs]
        fields = []
        stack = stack | {key}
        for a, b in zip(fh, fm):
            bw = a.get_bitfield_width() if a.is_bitfield() else -1
            fields.append([b.get_field_offsetof(), a.get_field_offsetof(), bw,
                           _desc(a.type, b.type, depth, stack)])
        return ['r', ms, hs, tm.get_align(), th.get_align(), 1 if union else 0, fields]
    if k in (K.FUNCTIONPROTO, K.FUNCTIONNOPROTO):
        return ['F']
    hs, ms = th.get_size(), tm.get_size()
    if hs <= 0 or ms <= 0:
        return ['x', max(ms, 0), max(hs, 0)]
    return ['s', ms, hs, _scalar_kind(th)]


def _parse_args(cmd):
    args = shlex.split(cmd)[1:]
    out = []
    skip = 0
    for a in args:
        if skip:
            skip -= 1
            continue
        if a in ('-o', '-c'):
            skip = 1
            continue
        if a.startswith('-W'):
            continue
        out.append(a)
    return out


def _worker(job):
    path, cmd = job
    args = _parse_args(cmd) + ['-w']
    global _types
    _types = {}
    res = {}
    try:
        tus = []
        for tgt in ('x86_64-linux-gnu', 'mips-linux-gnu'):
            tus.append(_index.parse(path, args=args + ['-target', tgt]))
        decls = []
        for tu in tus:
            d = {}
            for c in tu.cursor.get_children():
                if c.kind == _ci.CursorKind.VAR_DECL and c.spelling in _targets:
                    d.setdefault(c.spelling, []).append(c)
            decls.append(d)
        for name, cs in decls[0].items():
            ms = decls[1].get(name)
            if not ms:
                continue
            # the last declaration is the most complete one (a later
            # `extern T x[N]` completes an earlier `x[]`)
            res[name] = _desc(cs[-1].type, ms[-1].type, 0, frozenset())
        res['__types__'] = {k: v for k, v in _types.items() if v is not None}
    except Exception as ex:  # noqa: BLE001
        res['__error__'] = '%s: %s' % (path, ex)
    return res


# ---------------------------------------------------------------------------
# ELF: symbols, bytes, address spaces
# ---------------------------------------------------------------------------

def phys(addr):
    """Canonical key of an N64 address: one key per byte of memory.

    KSEG0 0x80000000+ and the engine's useg window 0x00200000-0x003FFFFF are
    the same physical RAM (TLB entry 0x200000 -> phys 0). The VM modules at
    0x00400000+ have their own pages: kept apart (key 0x1_0000_0000 + addr).
    """
    if 0x80000000 <= addr < 0xA0000000:
        return addr - 0x80000000
    if 0x00200000 <= addr < 0x00400000:
        return addr - 0x00200000
    return 0x100000000 + addr


SKIP_SECTIONS = {'.virtual_1', '.audio_ctl', '.audio_wavetable', '.sound_data',
                 '.cartridge', '.trailer', '.header', '.boot'}


class Elf:
    def __init__(self, path):
        from elftools.elf.elffile import ELFFile
        self.f = open(path, 'rb')
        e = ELFFile(self.f)
        self.secs = list(e.iter_sections())
        self.regions = []  # (phys_lo, phys_hi, data, name)
        for s in self.secs:
            if s['sh_type'] != 'SHT_PROGBITS' or not s['sh_flags'] & 2:
                continue
            if s.name in SKIP_SECTIONS or s['sh_size'] == 0:
                continue
            lo = phys(s['sh_addr'])
            self.regions.append((lo, lo + s['sh_size'], s.data(), s.name))
        self.regions.sort()
        self.syms = {}       # name -> (addr, size, section, type)
        by_key = collections.defaultdict(list)
        for s in e.get_section_by_name('.symtab').iter_symbols():
            if not s.name or s['st_info']['type'] in ('STT_FILE', 'STT_SECTION'):
                continue
            sh = s['st_shndx']
            sec = self.secs[sh].name if isinstance(sh, int) else sh
            v = (s['st_value'], s['st_size'], sec, s['st_info']['type'])
            if s.name in self.syms and self.syms[s.name][2] != '.virtual_1':
                continue
            self.syms[s.name] = v
        for name, (a, sz, sec, typ) in self.syms.items():
            if sec == '.virtual_1' or name.endswith('_2') and sec == 'SHN_ABS':
                continue
            by_key[phys(a)].append(name)
        self.keys = sorted(by_key)
        self.at = by_key

    def section_of(self, key):
        for lo, hi, data, name in self.regions:
            if lo <= key < hi:
                return lo, hi, data, name
        return None

    def read(self, key, n):
        r = self.section_of(key)
        if r is None:
            return None
        lo, hi, data, _ = r
        n = min(n, hi - key)
        return data[key - lo:key - lo + n]

    def extent(self, key):
        """Bytes from key to the next symbol (or the end of its section)."""
        i = bisect.bisect_right(self.keys, key)
        nxt = self.keys[i] if i < len(self.keys) else key + 0x100000
        r = self.section_of(key)
        if r and r[1] < nxt:
            nxt = r[1]
        return nxt - key

    def containing(self, key):
        i = bisect.bisect_right(self.keys, key) - 1
        if i < 0:
            return None
        return self.keys[i]


# ---------------------------------------------------------------------------
# layout: leaves of a described type
# ---------------------------------------------------------------------------

def dsize(d):
    """(n64 size, host size) of a description."""
    t = d[0]
    if t == 's':
        return d[1], d[2]
    if t == 'p':
        return 4, 8
    if t == 'a':
        n, h = dsize(d[2])
        c = max(d[1], 0)
        return n * c, h * c
    if t == 'r':
        return d[1], d[2]
    if t == 'x':
        return d[1], d[2]
    return 0, 0


def dalign(d):
    t = d[0]
    if t == 's':
        return d[2]
    if t == 'p':
        return 8
    if t == 'a':
        return dalign(d[2])
    if t == 'r':
        return d[4]
    return 1


def leaves(d, noff, hoff, out):
    """Flatten into (n64 off, host off, kind, n64 size, host size, extra)."""
    t = d[0]
    if t == 's':
        out.append((noff, hoff, 's', d[1], d[2], d[3]))
    elif t == 'p':
        out.append((noff, hoff, 'p', 4, 8, d[1]))
    elif t == 'a':
        n, h = dsize(d[2])
        for i in range(max(d[1], 0)):
            leaves(d[2], noff + i * n, hoff + i * h, out)
    elif t == 'r':
        fields = d[6]
        if d[5]:  # union: the first member decides, except that a scalar member as large as
            # the union wins (libultra's du is { struct { u32 hi, lo; } w; f64 d; }: written by
            # its words, a little-endian double comes out with its halves swapped)
            whole = [f for f in fields if f[2] < 0 and f[3][0] == 's' and f[3][1] == d[1]]
            fields = whole[:1] if whole else fields[:1]
        for nb, hb, bw, fd in fields:
            if bw >= 0:
                out.append((noff * 8 + nb, hoff * 8 + hb, 'b', bw, dsize(fd)[1], None))
            else:
                leaves(fd, noff + nb // 8, hoff + hb // 8, out)
    elif t == 'x':
        out.append((noff, hoff, 'raw', d[1], d[2], None))


def dtranslate(d, noff):
    return _dtranslate(d, noff)[0]


def _dtranslate(d, noff):
    """(host offset, typed): typed is False when the offset only lands in a byte array (a
    `u8 pad[N]` view), so that a union prefers the member that types the offset: CGame's
    camera pool is `union { CCameraPool m_CameraPool[1]; struct { u8 pad[..]; u8 m_MenuPool[..]; }; }`
    and CEngineApp's game `union { CGame m_Game; struct { u8 pad[..]; GameSettings m_Settings; }; }`
    (structs/game.h). Ties go to the first member, as in leaves()."""
    t = d[0]
    if t == 'a':
        n, h = dsize(d[2])
        if n <= 0:
            return noff, False
        i = noff // n
        sub, typed = _dtranslate(d[2], noff - i * n)
        if d[2][0] == 's' and d[2][1] == 1 and d[1] > 4:
            typed = False
        return i * h + sub, typed
    if t == 'r':
        cands = [(nb // 8, hb // 8, fd) for nb, hb, bw, fd in d[6] if bw < 0 and nb // 8 <= noff]
        if not cands:
            return noff, True
        if d[5]:  # union: every member starts at 0
            best = None
            for c in cands:
                if noff - c[0] >= dsize(c[2])[0]:
                    continue
                sub, typed = _dtranslate(c[2], noff - c[0])
                if best is None or (typed and not best[1]):
                    best = (c[1] + sub, typed)
            if best is not None:
                return best
        c = cands[-1]
        sub, typed = _dtranslate(c[2], noff - c[0])
        return c[1] + sub, typed
    if t == 'p':
        return min(noff, 4), True
    return noff, True


def shape(d):
    """A description without pointee details (to compare declarations)."""
    t = d[0]
    if t == 'p':
        return ['p']
    if t == 'a':
        return ['a', d[1], shape(d[2])]
    if t == 'r':
        return ['r', d[1], d[2], [[f[0], f[2], shape(f[3])] for f in d[6]]]
    return d


# ---------------------------------------------------------------------------
# generator
# ---------------------------------------------------------------------------

class Sym:
    def __init__(self, name, key, addr):
        self.name = name
        self.key = key
        self.addr = addr
        self.desc = None       # type description or None (bytes)
        self.why = ''
        self.nsize = 0
        self.hsize = 0
        self.align = 4
        self.items = []        # (host off, 'b', bytes) | (host off, 'p', target expr)
        self.leafmap = []      # (n64 off, host off) for offset translation
        self.alias_of = None   # (Sym, host off)
        self.bss = False


def c_ident_ok(n):
    return re.match(r'^[A-Za-z_][A-Za-z0-9_]*$', n) is not None


def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--build', required=True, help='port build dir (compile_commands.json, libturok2_game.a)')
    ap.add_argument('--elf', default=os.path.join(ROOT, 'build/turok2.us.elf'))
    ap.add_argument('--out', default=os.path.join(ROOT, 'port/gen'))
    ap.add_argument('--libclang', default=os.environ.get('LIBCLANG', '/usr/lib/llvm-19/lib/libclang-19.so.1'))
    ap.add_argument('-j', type=int, default=4)
    ap.add_argument('--cache', help='json cache of the declared types (skip libclang)')
    a = ap.parse_args()

    elf = Elf(a.elf)
    lib = os.path.join(a.build, 'libturok2_game.a')
    nm = subprocess.run(['nm', '-A', lib], capture_output=True, text=True, check=True).stdout

    # objects, their defined/undefined symbols; module 2 objects are left out
    defined_by = collections.defaultdict(set)
    undef_by = collections.defaultdict(set)
    local = set()
    for line in nm.splitlines():
        p = line.split()
        obj = p[0].split(':')[1]
        if p[-2] == 'U':
            undef_by[obj].add(p[-1])
        elif p[-2] in 'TDBRC':
            defined_by[obj].add(p[-1])
        else:
            local.add(p[-1])
    mod2 = set()
    for obj, ds in defined_by.items():
        for d in ds:
            s = elf.syms.get(d)
            if s and s[2] == '.virtual_1':
                mod2.add(obj)
    defined = set()
    for obj, ds in defined_by.items():
        if obj not in mod2:
            defined |= ds
    funcs = defined  # the game lib defines functions only (no data)
    needed = set()
    for obj, us in undef_by.items():
        if obj not in mod2:
            needed |= us - defined

    # data the game names through port/include/t2_rename.h (t2_X: libultraship also defines X):
    # generated from X's N64 data, emitted under the t2_ name (#define X t2_X in every file)
    renamed = set()
    for n in sorted(needed):
        if n.startswith('t2_') and n not in elf.syms:
            s = elf.syms.get(n[3:])
            if s and s[3] != 'STT_FUNC':
                renamed.add(n[3:])
    needed = (needed - {'t2_' + n for n in renamed}) | renamed

    # which needed names are data, which are functions/other
    data_names, func_names, abs_rom, missing = [], [], [], []
    for n in sorted(needed):
        s = elf.syms.get(n)
        if s is None:
            missing.append(n)
            continue
        addr, size, sec, typ = s
        if typ == 'STT_FUNC':
            func_names.append(n)
        elif sec == 'SHN_ABS' and addr < 0x200000:
            abs_rom.append(n)
        elif sec == 'SHN_ABS' and 0x400000 <= addr < 0x80000000 and elf.section_of(phys(addr)) is None:
            func_names.append(n)  # VM label by address (module-2 cross references)
        else:
            data_names.append(n)

    # declared types (libclang over the module-1 files that use them)
    if a.cache and os.path.exists(a.cache):
        decls = json.load(open(a.cache))
        types = decls['__types__']
    else:
        cc = json.load(open(os.path.join(a.build, 'compile_commands.json')))
        want = set(data_names)
        jobs = []
        for d in cc:
            obj = os.path.basename(d['output'] if 'output' in d else d['command'].split(' -o ')[1].split()[0])
            if obj in mod2 or not (undef_by.get(obj, set()) & want):
                continue
            jobs.append((d['file'], d['command']))
        decls = collections.defaultdict(list)
        types = {}
        decls['__types__'] = types
        with multiprocessing.Pool(a.j, _init_worker, (a.libclang, want)) as pool:
            for res in pool.imap_unordered(_worker, jobs, chunksize=4):
                if '__error__' in res:
                    print('warning:', res.pop('__error__'), file=sys.stderr)
                for k, v in res.pop('__types__', {}).items():
                    types.setdefault(k, v)
                for k, v in res.items():
                    decls[k].append(v)
        if a.cache:
            json.dump(decls, open(a.cache, 'w'))

    gen = Generator(elf, funcs, local, types)
    gen.renamed = renamed
    gen.run(data_names, decls)
    gen.write(a.out, abs_rom, func_names, missing, mod2)


class Generator:
    def __init__(self, elf, funcs, local, types):
        self.types = types
        self.elf = elf
        self.funcs = funcs
        self.local = local
        self.syms = {}        # name -> Sym
        self.by_key = {}      # key -> Sym (primary definition)
        self.conflicts = []
        self.unresolved = collections.Counter()
        self.unresolved_ex = {}
        self.func_refs = set()
        self.ext_funcs = set()
        self.int_addr = collections.Counter()  # integer leaves that hold an address

    # -- choosing a type --------------------------------------------------
    def pick(self, name, ds):
        cnt = collections.Counter(json.dumps(shape(d)) for d in ds)
        if len(cnt) > 1:
            self.conflicts.append((name, [(c, s[:120]) for s, c in cnt.most_common()]))
        best = cnt.most_common(1)[0][0]
        # among the declarations of that shape, prefer the one that knows the
        # most about its pointees and has a size
        cands = [d for d in ds if json.dumps(shape(d)) == best]
        cands.sort(key=lambda d: -len(json.dumps(d)))
        return cands[0]

    def add(self, name, desc, why):
        if name in self.syms:
            s = self.syms[name]
            if s.desc is None and desc is not None and not s.items and s.alias_of is None:
                s.desc = desc
            return s
        addr = self.elf.syms[name][0]
        s = Sym(name, phys(addr), addr)
        s.desc = desc
        s.why = why
        self.syms[name] = s
        self.queue.append(s)
        return s

    def run(self, data_names, decls):
        self.queue = []
        for n in data_names:
            ds = decls.get(n)
            self.add(n, self.pick(n, ds) if ds else None, 'C')
        # lay out (this can add pointer targets to the queue)
        i = 0
        while i < len(self.queue):
            self.layout(self.queue[i])
            i += 1
        self.resolve_aliases()

    # -- layout of one symbol --------------------------------------------
    def is_bss(self, s):
        """Linker-script symbols (SHN_ABS: placed by address, no .s data
        behind them) are .bss: the ELF bytes at their address belong to
        whatever splat put there (libultra code, strings), not to them."""
        info = self.elf.syms[s.name]
        return info[2] == 'SHN_ABS' or self.elf.section_of(s.key) is None

    def size_of(self, s):
        ext = self.elf.extent(s.key)
        if self.is_bss(s):
            # outside the 8 MB of RAM there is nothing to measure against
            cap = 0x100000 if 0x80000000 <= s.addr < 0x80800000 else 0x1000
            ext = min(ext, cap)
        d = s.desc
        if d is None:
            return None, ext
        if d[0] == 'a' and d[1] < 0:
            n, _ = dsize(d[2])
            if n <= 0:
                return None, ext
            cnt = max(1, ext // n)
            d = ['a', cnt, d[2]]
        elif d[0] in ('s', 'p'):
            n, _ = dsize(d)
            if ext > n and ext % n == 0 and ext // n <= 4096 and not self.is_bss(s):
                d = ['a', ext // n, d]  # m2c `extern s32 D_x;` indexed as an array
        elif d[0] in ('x', 'F'):
            return None, ext
        return d, ext

    def layout(self, s):
        if s.items or s.alias_of:
            return
        d, ext = self.size_of(s)
        data = None
        if d is not None:
            n, h = dsize(d)
            if n <= 0:
                d = None
        if d is None:
            n = min(ext, 0x100000)
            h = n
        s.nsize, s.hsize = n, h
        s.align = max(4, dalign(d)) if d is not None else 4
        s.layout_desc = d
        raw = None if self.is_bss(s) else self.elf.read(s.key, n)
        if raw is None:
            s.bss = True
            return
        raw = raw.ljust(n, b'\0')
        if d is None:
            s.items = [(0, 'b', raw)]
            return
        lv = []
        leaves(d, 0, 0, lv)
        img = bytearray(h)
        ptrs = []
        for noff, hoff, kind, ns, hs, extra in lv:
            if kind == 's':
                v = raw[noff:noff + ns]
                if len(v) < ns:
                    continue
                if ns == 4 and extra != 'f':
                    iv = int.from_bytes(v, 'big')
                    if (0x80000400 <= iv < 0x80400000 or 0x00200500 <= iv < 0x002C3000
                            or 0x00400000 <= iv < 0x0043CC14) and self.elf.containing(phys(iv)) is not None:
                        self.int_addr[s.name] += 1
                if extra == 'f' or ns == hs:
                    img[hoff:hoff + hs] = v[::-1] if ns > 1 else v
                else:  # int widened (long): sign/zero extend
                    iv = int.from_bytes(v, 'big', signed=(extra == 'i'))
                    img[hoff:hoff + hs] = iv.to_bytes(hs, 'little', signed=(extra == 'i'))
            elif kind == 'raw':
                img[hoff:hoff + min(ns, hs)] = raw[noff:noff + min(ns, hs)]
            elif kind == 'b':
                # noff/hoff are BIT offsets from the symbol start; ns = width,
                # hs = storage unit size (host). N64 is big-endian: bit 0 is
                # the MSB of byte 0; host little-endian: bit 0 is the LSB.
                nbyte = noff // 8
                unit = raw[nbyte:nbyte + 8].ljust(8, b'\0')
                big = int.from_bytes(unit, 'big')
                shift = 64 - (noff % 8) - ns
                val = (big >> shift) & ((1 << ns) - 1)
                hbyte = hoff // 8
                hb = hoff % 8
                cur = int.from_bytes(img[hbyte:hbyte + 8].ljust(8, b'\0'), 'little')
                cur |= val << hb
                nb = (hb + ns + 7) // 8
                img[hbyte:hbyte + nb] = cur.to_bytes(8, 'little')[:nb]
            elif kind == 'p':
                v = int.from_bytes(raw[noff:noff + 4], 'big')
                ptrs.append((hoff, v, extra))
        items = []
        last = 0
        for hoff, v, pointee in ptrs:
            if hoff > last:
                items.append((last, 'b', bytes(img[last:hoff])))
            r = self.resolve_ptr(v, pointee)
            if r is False:
                self.unresolved[s.name] += 1
                self.unresolved_ex.setdefault(s.name, hex(v))
                r = None
            items.append((hoff, 'P', r))
            last = hoff + 8
        if last < h:
            items.append((last, 'b', bytes(img[last:h])))
        s.items = items
        s.trailing = ext - n if ext > n else 0

    # -- pointers ------------------------------------------------------------
    def resolve_ptr(self, v, pointee):
        """N64 pointer value -> C expression, or None."""
        if v == 0:
            return None
        key = phys(v)
        base = self.elf.containing(key)
        if base is None:
            return False
        names = self.elf.at[base]
        off = key - base
        # prefer a function the link defines, then data we generate
        for n in names:
            if n in self.funcs:
                if off == 0:
                    self.func_refs.add(n)
                    return ('f', n, 0)
        for n in names:
            info = self.elf.syms[n]
            if info[3] == 'STT_FUNC':
                if n in self.local or n.endswith('_2'):
                    continue
                if off == 0:
                    self.func_refs.add(n)
                    self.ext_funcs.add(n)
                    return ('f', n, 0)
        if self.elf.section_of(key) is None and not (0x80000000 <= v < 0x80800000):
            return False
        for n in sorted(names, key=lambda x: (x not in self.syms, x)):
            if not c_ident_ok(n):
                continue
            info = self.elf.syms[n]
            if info[3] == 'STT_FUNC':
                continue
            pd = self.types.get(pointee) if (off == 0 and isinstance(pointee, str) and pointee != 'F') else None
            if pd is not None and pd[0] in ('x', 'F'):
                pd = None
            t = self.add(n, pd, 'pointer target')
            return ('d', t, off)
        return False

    def resolve_aliases(self):
        """Symbols that lie inside a typed symbol become aliases of it."""
        order = sorted(self.syms.values(), key=lambda s: s.key)
        cur = None
        for s in order:
            if cur and s.key < cur.key + max(cur.nsize, 1) and s is not cur:
                s.alias_of = (cur, self.translate(cur, s.key - cur.key))
                continue
            cur = s

    def translate(self, s, noff):
        """N64 offset inside s -> host offset (walks the layout description)."""
        d = getattr(s, 'layout_desc', None)
        return dtranslate(d, noff) if d is not None else noff

    def expr(self, r):
        kind, t, off = r
        if kind == 'f':
            return '(void *)%s' % t
        tgt = t
        if tgt.alias_of:
            base, _ = tgt.alias_of
            hoff = self.translate(base, (tgt.key - base.key) + off)
            tgt = base
        else:
            hoff = self.translate(tgt, off)
        if hoff:
            return '(void *)((char *)&%s + 0x%X)' % (tgt.name, hoff)
        return '(void *)&%s' % tgt.name

    # -- output ----------------------------------------------------------------
    def write(self, out, abs_rom, func_names, missing, mod2):
        os.makedirs(out, exist_ok=True)
        regions = collections.defaultdict(list)
        for s in sorted(self.syms.values(), key=lambda s: s.key):
            # an alias goes in its base's file (`.set` needs the base defined there)
            b = s.alias_of[0] if s.alias_of else s
            r = None if b.bss else self.elf.section_of(b.key)
            reg = r[3].lstrip('.') if r else 'bss'
            if reg.startswith('code_rodata'):
                reg = 'rodata'
            elif reg.startswith('libultra'):
                reg = 'libultra'
            elif reg in ('virtual', 'virtual_tail', 'vm_dup_engine'):
                reg = 'virtual'
            elif reg in ('main', 'code', 'pre_virtual', 'lnk_debug'):
                reg = 'misc'
            regions[reg].append(s)

        hdr = ('/* GENERATED by tools/port/gendata.py from build/turok2.us.elf: do not edit.\n'
               ' * docs/port/DATA.md. Host layout, little-endian, pointers relocated. */\n'
               '#include <stdint.h>\n#include <stddef.h>\n'
               '#if defined(_MSC_VER)\n#define T2_PACKED_BEGIN __pragma(pack(push, 1))\n'
               '#define T2_PACKED_END __pragma(pack(pop))\n#define T2_PACKED\n#define T2_ALIGN(n) __declspec(align(n))\n'
               '#else\n#define T2_PACKED_BEGIN\n#define T2_PACKED_END\n#define T2_PACKED __attribute__((packed))\n'
               '#define T2_ALIGN(n) __attribute__((aligned(n)))\n#endif\n'
               '#define T2_STR_(x) #x\n#define T2_STR(x) T2_STR_(x)\n'
               '#define T2_SYM(x) T2_STR(__USER_LABEL_PREFIX__) #x\n\n')
        hdr += self.rename_defines()
        self.alias_nonelf = []
        written = []
        for reg, ss in sorted(regions.items()):
            fn = os.path.join(out, 'data_%s.c' % reg)
            written.append(fn)
            with open(fn, 'w') as f:
                f.write(hdr)
                # externs
                ext = set()
                for s in ss:
                    for it in s.items:
                        if it[1] == 'P' and it[2]:
                            k, t, _ = it[2]
                            if k == 'f':
                                ext.add(('f', t))
                            else:
                                tt = t.alias_of[0] if t.alias_of else t
                                if tt not in ss:
                                    ext.add(('d', tt.name))
                for k, n in sorted(ext):
                    f.write('extern void %s(void);\n' % n if k == 'f' else 'extern char %s[];\n' % n)
                f.write('\n')
                for s in ss:
                    self.emit(f, s, 'decl')
                f.write('\n')
                for s in ss:
                    self.emit(f, s, 'def')
        self.written = written
        self.write_relocs(out)
        self.write_abs(out, abs_rom)
        self.write_report(out, abs_rom, func_names, missing, mod2)

    def emit(self, f, s, phase):
        """phase 'decl': the struct type and an extern declaration (so that
        any definition can point at any other in the same file); 'def': the
        definition."""
        if not c_ident_ok(s.name):
            if phase == 'def':
                f.write('/* skipped %s: not a C identifier */\n' % s.name)
            return
        if s.alias_of:
            if phase == 'decl':
                return
            base, hoff = s.alias_of
            f.write('/* %s = %s + 0x%X (N64 0x%08X) */\n' % (s.name, base.name, hoff, s.addr))
            f.write('#if defined(__ELF__)\n__asm__(".globl " T2_SYM(%s) "\\n.set " T2_SYM(%s) ", " T2_SYM(%s) " + %d");\n'
                    % (s.name, s.name, base.name, hoff))
            f.write('#else\nchar %s[1]; /* FIXME: alias not supported here */\n#endif\n' % s.name)
            self.alias_nonelf.append(s.name)
            return
        al = min(max(s.align, 4), 16)
        what = 'typed' if s.layout_desc is not None else 'bytes'
        if s.bss:
            if phase == 'decl':
                f.write('extern unsigned char %s[0x%X];\n' % (s.name, max(s.hsize, 1)))
            else:
                f.write('/* 0x%08X bss %s n64 0x%X */\nT2_ALIGN(%d) unsigned char %s[0x%X];\n'
                        % (s.addr, what, s.nsize, al, s.name, max(s.hsize, 1)))
            return
        items = [it for it in s.items if not (it[1] == 'b' and len(it[2]) == 0)]
        trailing = getattr(s, 'trailing', 0)
        if trailing:
            items.append((s.hsize, 'b', self.elf.read(s.key + s.nsize, trailing) or b''))
        if len(items) == 1 and items[0][1] == 'b':
            b = items[0][2]
            if phase == 'decl':
                f.write('extern unsigned char %s[0x%X];\n' % (s.name, max(len(b), 1)))
            else:
                f.write('/* 0x%08X %s n64 0x%X host 0x%X */\n' % (s.addr, what, s.nsize, s.hsize + trailing))
                f.write('T2_ALIGN(%d) unsigned char %s[0x%X] = {%s};\n' % (al, s.name, max(len(b), 1), self.bytes_c(b)))
            return
        if phase == 'decl':
            f.write('T2_PACKED_BEGIN struct T2_PACKED t2s_%s {' % s.name)
            for it in items:
                if it[1] == 'b':
                    f.write(' unsigned char b%X[%d];' % (it[0], len(it[2])))
                else:
                    f.write(' void *p%X;' % it[0])
            f.write(' }; T2_PACKED_END\nextern struct t2s_%s %s;\n' % (s.name, s.name))
            return
        f.write('/* 0x%08X %s n64 0x%X host 0x%X */\n' % (s.addr, what, s.nsize, s.hsize + trailing))
        f.write('T2_ALIGN(%d) struct t2s_%s %s = {\n' % (al, s.name, s.name))
        for it in items:
            if it[1] == 'b':
                f.write('  {%s},\n' % self.bytes_c(it[2]))
            else:
                f.write('  %s,\n' % (self.expr(it[2]) if it[2] else '0'))
        f.write('};\n')

    @staticmethod
    def bytes_c(b):
        if not any(b):
            return '0'
        parts = ['0x%X' % x for x in b]
        lines = [','.join(parts[i:i + 24]) for i in range(0, len(parts), 24)]
        return ',\n'.join(lines)

    def rename_defines(self):
        """#define X t2_X for the data port/include/t2_rename.h renames (not aliases: .set
        would stringify the unexpanded name)."""
        rn = sorted(getattr(self, 'renamed', ()))
        if not rn:
            return ''
        return ('/* renamed by port/include/t2_rename.h (the host libraries define these names) */\n'
                + ''.join('#define %s t2_%s\n' % (n, n) for n in rn) + '\n')

    def write_relocs(self, out):
        rows = []
        for s in self.syms.values():
            if not c_ident_ok(s.name):
                continue
            base, hoff = (s.alias_of if s.alias_of else (s, 0))
            rows.append((s.addr, max(s.nsize, 1), base.name, hoff))
        rows.sort()
        names = sorted({r[2] for r in rows})
        with open(os.path.join(out, 'relocs.c'), 'w') as f:
            f.write('/* GENERATED by tools/port/gendata.py: do not edit (docs/port/DATA.md).\n'
                    ' * N64 address -> host address of the generated data, for integer slots\n'
                    ' * that still carry an N64 address (the "handle" is that address). */\n'
                    '#include <stdint.h>\n#include <stddef.h>\n\n')
            f.write(self.rename_defines())
            for n in names:
                f.write('extern char %s[];\n' % n)
            f.write('\ntypedef struct { uint32_t n64; uint32_t size; char *host; } T2DataSym;\n')
            f.write('const T2DataSym t2_data_syms[] = {\n')
            for addr, size, n, hoff in rows:
                f.write('  {0x%08X, 0x%X, %s + 0x%X},\n' % (addr, size, n, hoff))
            f.write('};\nconst size_t t2_data_nsyms = %d;\n\n' % len(rows))
            f.write('/* Linear offset inside the symbol: exact for byte/int data, approximate\n'
                    ' * past a widened pointer slot. NULL if the address is not generated data. */\n'
                    'void *t2_n64_to_host(uint32_t addr)\n{\n'
                    '    size_t lo = 0, hi = t2_data_nsyms;\n'
                    '    if (addr - 0x80000000u < 0x00800000u || 1) {}\n'
                    '    while (lo < hi) {\n'
                    '        size_t mid = (lo + hi) / 2;\n'
                    '        if (t2_data_syms[mid].n64 <= addr) lo = mid + 1; else hi = mid;\n'
                    '    }\n'
                    '    if (lo == 0) return NULL;\n'
                    '    lo--;\n'
                    '    if (addr - t2_data_syms[lo].n64 >= t2_data_syms[lo].size) return NULL;\n'
                    '    return t2_data_syms[lo].host + (addr - t2_data_syms[lo].n64);\n}\n')

    def write_abs(self, out, abs_rom):
        with open(os.path.join(out, 'data_abs.c'), 'w') as f:
            f.write('/* GENERATED by tools/port/gendata.py: do not edit (docs/port/DATA.md).\n'
                    ' * Absolute symbols the C takes the address of: ROM offsets\n'
                    ' * (`(u32)&D_14A000`). On the host they must become archive keys\n'
                    ' * (docs/assets/PORT.md); defined here as absolute symbols so the\n'
                    ' * game links (link with -no-pie: a PC-relative reference cannot\n'
                    ' * reach an absolute symbol in a PIE). */\n')
            f.write('#define T2_STR_(x) #x\n#define T2_STR(x) T2_STR_(x)\n'
                    '#define T2_SYM(x) T2_STR(__USER_LABEL_PREFIX__) #x\n')
            for n in abs_rom:
                f.write('__asm__(".globl " T2_SYM(%s) "\\n.set " T2_SYM(%s) ", 0x%X");\n'
                        % (n, n, self.elf.syms[n][0]))

    def write_report(self, out, abs_rom, func_names, missing, mod2):
        ss = list(self.syms.values())
        from_c = [s for s in ss if s.why == 'C']
        lines = []
        P = lines.append
        P('gendata report (tools/port/gendata.py)')
        P('module-2 objects left out: %d' % len(mod2))
        P('data symbols the module-1 C needs: %d' % len(from_c))
        P('  typed from a C declaration: %d' % sum(1 for s in from_c if s.layout_desc is not None))
        P('  bytes (no usable declaration): %d' % sum(1 for s in from_c if s.layout_desc is None))
        P('pointer targets generated in addition: %d' % (len(ss) - len(from_c)))
        P('bss (zero) symbols: %d' % sum(1 for s in ss if s.bss))
        P('aliases inside another symbol: %d' % sum(1 for s in ss if s.alias_of))
        np = sum(1 for s in ss for it in s.items if it[1] == 'P')
        P('pointer slots: %d (relocated %d, null %d, unresolved %d)' % (
            np, sum(1 for s in ss for it in s.items if it[1] == 'P' and it[2]),
            sum(1 for s in ss for it in s.items if it[1] == 'P' and not it[2]) - sum(self.unresolved.values()),
            sum(self.unresolved.values())))
        P('functions referenced from data: %d (%d not defined by the game C)' % (
            len(self.func_refs), len(self.ext_funcs)))
        P('host size: %d bytes (n64 %d)' % (sum(s.hsize for s in ss if not s.alias_of),
                                          sum(s.nsize for s in ss if not s.alias_of)))
        P('')
        P('== absolute ROM-offset symbols (data_abs.c): %d' % len(abs_rom))
        P('  ' + ' '.join(abs_rom))
        P('== not data (function side, not generated): %d' % len(func_names))
        P('  ' + ' '.join(func_names))
        P('== not in the ELF: %s' % ' '.join(missing))
        P('== functions referenced from data that the game C does not define: %d' % len(self.ext_funcs))
        P('  ' + ' '.join(sorted(self.ext_funcs)))
        P('== declaration conflicts (different layouts across files; the most common wins): %d' % len(self.conflicts))
        for n, c in sorted(self.conflicts):
            P('  %s: %s' % (n, '; '.join('%dx %s' % (v, k) for v, k in c)))
        P('== pointer slots whose value is not a known address: %d in %d symbols' % (
            sum(self.unresolved.values()), len(self.unresolved)))
        for n, c in self.unresolved.most_common():
            P('  %s: %d (e.g. %s)' % (n, c, self.unresolved_ex[n]))
        P('== aliases (ELF .set; a separate copy elsewhere): %d' % len(self.alias_nonelf))
        P('== integer slots whose value looks like an address (kept as the N64 handle;'
          ' candidates for pointer typing in src/): %d in %d symbols' % (
              sum(self.int_addr.values()), len(self.int_addr)))
        for n, c in self.int_addr.most_common():
            P('  %s: %d' % (n, c))
        with open(os.path.join(out, 'data_report.txt'), 'w') as f:
            f.write('\n'.join(lines) + '\n')
        print('\n'.join(lines[:12]))


if __name__ == '__main__':
    main()
