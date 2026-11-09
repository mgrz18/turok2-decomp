#!/usr/bin/env python3
"""linkcheck.py: link the game (module 1) + the generated data with a stub main
and list what is still unresolved, by category (docs/port/DATA.md).

  python3 tools/port/linkcheck.py --build <port build dir> [--gen port/gen] [--work DIR]

Steps: compile port/gen/*.c, extract libturok2_game.a, drop the module-2
objects (.virtual_1), link everything with a stub main using
--unresolved-symbols=ignore-all (so a real linker checks duplicates and
relocations), then compute the unresolved set from nm and classify it against
build/turok2.us.elf.
"""
import argparse
import collections
import glob
import os
import subprocess
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import gendata  # noqa: E402

ROOT = gendata.ROOT


def run(cmd, **kw):
    return subprocess.run(cmd, check=True, capture_output=True, text=True, **kw).stdout


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--build', required=True)
    ap.add_argument('--gen', default=os.path.join(ROOT, 'port/gen'))
    ap.add_argument('--elf', default=os.path.join(ROOT, 'build/turok2.us.elf'))
    ap.add_argument('--work', default=None)
    ap.add_argument('--cc', default='clang')
    a = ap.parse_args()
    work = a.work or os.path.join(a.build, 'linkcheck')
    objdir = os.path.join(work, 'game')
    gendir = os.path.join(work, 'gen')
    os.makedirs(objdir, exist_ok=True)
    os.makedirs(gendir, exist_ok=True)
    lib = os.path.abspath(os.path.join(a.build, 'libturok2_game.a'))
    if not glob.glob(os.path.join(objdir, '*.o')):
        run(['ar', 'x', lib], cwd=objdir)

    elf = gendata.Elf(a.elf)
    mod2 = set()
    game_objs = sorted(glob.glob(os.path.join(objdir, '*.o')))
    for o in game_objs:
        for line in run(['nm', '--defined-only', '-g', o]).splitlines():
            n = line.split()[-1]
            s = elf.syms.get(n)
            if s and s[2] == '.virtual_1':
                mod2.add(o)
    objs = [o for o in game_objs if o not in mod2]

    gen_objs = []
    for c in sorted(glob.glob(os.path.join(a.gen, '*.c'))):
        o = os.path.join(gendir, os.path.basename(c) + '.o')
        run([a.cc, '-c', '-O0', '-w', c, '-o', o])
        gen_objs.append(o)
    stub = os.path.join(gendir, 'stub_main.c')
    with open(stub, 'w') as f:
        f.write('int main(void) { return 0; }\n')
    run([a.cc, '-c', stub, '-o', stub + '.o'])

    exe = os.path.join(work, 'turok2_linkcheck')
    rsp = os.path.join(work, 'objs.rsp')
    with open(rsp, 'w') as f:
        f.write('\n'.join(objs + gen_objs + [stub + '.o']))
    link = subprocess.run([a.cc, '-no-pie', '-nostdlib', '-Wl,-e,main',
                           '-Wl,--unresolved-symbols=ignore-all', '@' + rsp, '-o', exe],
                          capture_output=True, text=True)
    print('link:', 'OK' if link.returncode == 0 else 'FAILED')
    if link.returncode:
        print(link.stderr[:4000])

    defined, undef = set(), collections.defaultdict(set)
    for o in objs + gen_objs + [stub + '.o']:
        for line in run(['nm', o]).splitlines():
            p = line.split()
            if p[-2] == 'U':
                undef[p[-1]].add(os.path.basename(o))
            elif p[-2] in 'TDBRCVWA':
                defined.add(p[-1])
    unres = sorted(set(undef) - defined)
    cat = collections.defaultdict(list)
    for n in unres:
        s = elf.syms.get(n)
        if s is None:
            cat['host libc / not in ELF'].append(n)
            continue
        addr, size, sec, typ = s
        if typ == 'STT_FUNC' and sec == '.virtual_1':
            cat['module-2 functions (should not be referenced)'].append(n)
        elif typ == 'STT_FUNC':
            cat['asm-only / excluded functions (%s)' % sec].append(n)
        elif sec == 'SHN_ABS' and 0x400000 <= addr < 0x80000000:
            cat['VM labels by address'].append(n)
        else:
            cat['data (%s)' % sec].append(n)
    print('objects: %d game (module 2 dropped: %d), %d generated' % (len(objs), len(mod2), len(gen_objs)))
    print('unresolved: %d' % len(unres))
    for k, v in sorted(cat.items(), key=lambda kv: -len(kv[1])):
        print('  %4d  %s' % (len(v), k))
        print('        ' + ' '.join(v))
    return 0 if link.returncode == 0 else 1


if __name__ == '__main__':
    sys.exit(main())
