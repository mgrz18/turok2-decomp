#!/usr/bin/env python3
"""Wire hand-matched C files into the build, with their .rodata slices.

usage: MATCH_ELF=build/ref.elf tools/accept_manual.py path/to/func_X.c [...]
Run before `make setup` (it reads the target asm from us/asm). Compiles each
file, and when the object has .rodata (float literals, jump tables) finds its
slice in the ROM, the way auto_match --rdata does; a file whose .rodata does
not match the ROM is skipped.
"""
import shutil, subprocess, sys
from pathlib import Path
sys.path.insert(0, 'tools'); import auto_match as A
from elftools.elf.elffile import ELFFile
W = Path('build/manual_obj'); W.mkdir(parents=True, exist_ok=True)
files = [Path(p) for p in sys.argv[1:]]
names = [p.name.split('__')[0].removesuffix('.c') for p in files]
for p, n in zip(files, names):
    shutil.copy(p, W / f'{n}.c')
    (W / f'{n}.o').unlink(missing_ok=True)
subprocess.run(['docker', 'run', '--platform=linux/amd64', '--rm', '-v', f'{Path.cwd()}:/work', '-w', '/work',
                'turok2-build', 'bash', '-c',
                ' '.join(f'tools/cc_func.sh {W}/{n}.c {W}/{n}.o cc1 -O2 2>/dev/null;' for n in names)], check=True)
bodies = A.extract_asm(); rom = open('baserom.us.z64', 'rb').read()
src, ro = {}, {}
for n in names:
    obj = W / f'{n}.o'
    if not obj.exists():
        print(f'  skip {n}: did not compile (a qemu crash? run it again)'); continue
    with open(obj, 'rb') as fh:
        s = ELFFile(fh).get_section_by_name('.rodata'); sz = s['sh_size'] if s else 0
    sl = A.rodata_slice(str(obj), n, bodies[n], rom) if sz else None
    if sz and not sl:
        print(f'  skip {n}: its .rodata is not in the ROM where the code reads it'); continue
    src[n] = (W / f'{n}.c').read_text()
    if sl: ro[n] = sl
A.accept(src, ro)
