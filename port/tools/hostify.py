#!/usr/bin/env python3
"""Host copy of a game C file for the port build (src/ stays untouched).

The matching build pins some locals to MIPS registers to reproduce SN64's
register allocation:

    register f32 s asm("$f4") = depth;

A host compiler rejects the MIPS register name. The pin has no meaning off
the N64, so the host copy drops it (the declaration stays a plain local).
Empty asm statements (`__asm__("" : "+f"(tmp));`) are the same trick: a
barrier that keeps a value in a register for SN64's scheduler. They emit no
code, but a host backend still has to honour the MIPS constraints ("f" is an
x87 stack register on x86-64 and crashes clang 19), so the host copy drops
them too.

`__attribute__((section(".text_vm")))` places a function in a VM-module
section of the N64 link. A host has one copy of every function and Mach-O
rejects a section name without a segment ("__TEXT,..."), so the host copy
drops the attribute.

Function labels (`extern void f(void) __asm__("Name");`, a second local name
for a global symbol) are kept, but Mach-O prefixes C symbols with `_` and an
asm label is taken verbatim, so the host copy writes them as
`__asm__(T2_LABEL_PREFIX "Name")` (port_shim.h: "_" on Apple, "" elsewhere).

Usage: hostify.py <in.c> <out.c>
"""
import re
import sys

PIN = re.compile(r'\b(?:asm|__asm__)\s*\(\s*"\$[^"]*"\s*\)')
SECTION = re.compile(r'__attribute__\s*\(\(\s*section\s*\(\s*"[^"]*"\s*\)\s*\)\)')
LABEL = re.compile(r'(?<=[\w)\]])(\s*)\b(?:asm|__asm__)\s*\(\s*"([A-Za-z_][A-Za-z0-9_.$]*)"\s*\)')
BARRIER = re.compile(r'\b(?:asm|__asm__)(?:\s+(?:volatile|__volatile__))?\s*\(\s*""[^;]*\)\s*;')


def main():
    src, dst = sys.argv[1], sys.argv[2]
    text = open(src, encoding="utf-8", errors="surrogateescape").read()
    text = PIN.sub("", text)
    text = BARRIER.sub(";", text)
    text = SECTION.sub("", text)
    text = LABEL.sub(r'\1__asm__(T2_LABEL_PREFIX "\2")', text)
    with open(dst, "w", encoding="utf-8", errors="surrogateescape") as fh:
        fh.write(f'#line 1 "{src}"\n')
        fh.write(text)


if __name__ == "__main__":
    main()
