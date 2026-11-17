#!/usr/bin/env python3
"""mklayout.py: the matching build's data layout without the ELF (docs/port/QUICKSTART.md).

gendata.py needs two things from build/turok2.us.elf: the symbol table and the bytes of the
loaded sections. The bytes are the ROM's own (the build is byte-exact: every loaded section
sits at its load address in baserom.us.z64), so only the table is the build's. This tool
writes it to versions/port_layout.us.json (names, addresses, sizes: no game data), which is
committed; gendata.py reads it with the ROM when there is no ELF, so building the port does
not need the N64 toolchain.

  python3 tools/port/mklayout.py                 # write the layout from build/turok2.us.elf
  python3 tools/port/mklayout.py --check         # exit 1 if the committed layout is stale

Regenerate (on a box with the N64 build) whenever a commit renames, adds or moves a data
symbol: `make build` (or build/ref.elf), then this tool, and commit the json with the change.
"""
import argparse
import hashlib
import json
import os
import sys

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..'))
DEFAULT_OUT = os.path.join(ROOT, 'versions', 'port_layout.us.json')


def layout_from_elf(path, rom):
    from elftools.elf.elffile import ELFFile
    with open(path, 'rb') as f:
        e = ELFFile(f)
        loads = [(p['p_paddr'], p['p_filesz'], p['p_offset'])
                 for p in e.iter_segments() if p['p_type'] == 'PT_LOAD']
        secs = []
        for s in e.iter_sections():
            ent = {'name': s.name, 'type': s['sh_type'], 'flags': s['sh_flags'],
                   'addr': s['sh_addr'], 'size': s['sh_size']}
            if s['sh_type'] == 'SHT_PROGBITS' and s['sh_flags'] & 2 and s['sh_size']:
                lma = None
                for pa, fs, off in loads:
                    if off <= s['sh_offset'] < off + fs:
                        lma = pa + s['sh_offset'] - off
                d = s.data()
                if lma is None or rom[lma:lma + len(d)] != d:
                    sys.exit('mklayout: section %s is not the ROM bytes at its load address'
                             % s.name)
                ent['rom'] = lma
                ent['sha1'] = hashlib.sha1(d).hexdigest()
            secs.append(ent)
        syms = []
        for s in e.get_section_by_name('.symtab').iter_symbols():
            sh = s['st_shndx']
            syms.append([s.name, s['st_value'], s['st_size'],
                         sh if isinstance(sh, int) else str(sh), s['st_info']['type']])
    return {'format': 1, 'sections': secs, 'symbols': syms}


def dump(lay):
    # one section / one symbol per line: small diffs when a name changes
    out = ['{"format": %d,' % lay['format'], '"sections": [']
    out.append(',\n'.join(json.dumps(s, sort_keys=True) for s in lay['sections']))
    out.append('],\n"symbols": [')
    out.append(',\n'.join(json.dumps(s) for s in lay['symbols']))
    out.append(']}\n')
    return '\n'.join(out)


def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--elf', default=os.path.join(ROOT, 'build/turok2.us.elf'))
    ap.add_argument('--rom', default=os.path.join(ROOT, 'baserom.us.z64'))
    ap.add_argument('-o', '--out', default=DEFAULT_OUT)
    ap.add_argument('--check', action='store_true', help='compare with --out instead of writing')
    a = ap.parse_args()
    with open(a.rom, 'rb') as f:
        rom = f.read()
    text = dump(layout_from_elf(a.elf, rom))
    if a.check:
        old = open(a.out).read() if os.path.exists(a.out) else ''
        if old != text:
            print('mklayout: %s is stale: run tools/port/mklayout.py and commit it' % a.out)
            return 1
        print('mklayout: %s is up to date' % a.out)
        return 0
    with open(a.out, 'w') as f:
        f.write(text)
    print('mklayout: %s: %d sections, %d symbols' % (a.out, len(json.loads(text)['sections']),
                                                    len(json.loads(text)['symbols'])))
    return 0


if __name__ == '__main__':
    sys.exit(main())
