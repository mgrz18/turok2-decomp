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
import re
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
entered = A.entered_from_outside(bodies)
# A paged VM module is in the ROM twice, the second copy 0x39000 on, and the
# copy's absolute jumps and calls (`j`, `jal`) still land in the first: splat writes them as the
# first copy's labels. A function only `j`-entered that way can be C; its
# labels are then defined for the linker as plain addresses
# (versions/foreign_labels.us.txt).
owner = {lab: f for f, b in bodies.items() for lab in re.findall(r'^\s*\.L([0-9A-F]{8}):', b, re.M)}
jumps = {}
for f, b in bodies.items():
    for op, lab in re.findall(r'^\s+(jal|j|b\w*)\s+(?:[^,\n]*,\s*)*\.L([0-9A-F]{8})\b', b, re.M):
        o = owner.get(lab)
        if o and o != f:
            jumps.setdefault(o, []).append((op, lab))
labels = set()
src, ro = {}, {}
for n in names:
    if n in entered:
        if any(op not in ('j', 'jal') for op, _ in jumps.get(n, [])):
            print(f'  skip {n}: another function branches into it (a false boundary)'); continue
        labels |= {lab for _, lab in jumps[n]}
    if n not in bodies:
        print(f'  skip {n}: not a function of its own in the asm (joined or cut elsewhere)'); continue
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
if labels:
    fl = Path('versions/foreign_labels.us.txt')
    have = fl.read_text() if fl.exists() else ('/* Labels inside functions now in C that the second copy of a VM module\n'
                                               ' * jumps to (tools/accept_manual.py): plain addresses for ld. */\n')
    have += ''.join(f'".L{lab}" = 0x{lab};\n' for lab in sorted(labels) if f'".L{lab}"' not in have)
    fl.write_text(have)
A.accept(src, ro)
