# rnc.c

RNC ProPack methods 1 and 2 unpacker, copied unchanged from
`references/rw_junk/docs/hand_asm_c/rnc.c` (the Turok: Rage Wars decomp's
functionally equivalent C reconstruction of the game's hand-written
`rncUnpackMethod1/2`, differentially tested against the original machine code). Turok 2 uses the same
ProPack unpackers (`RNC_UnpackMethod1/2`, hand-written assembly called by
`ProPackDecompress`, `func_00200B00.c`).

Used by `port/assets/t2asset.c` (ROM backend: `T2Rnc_Unpack`, which adds
the header CRC16 check the game skips). The unpacker may read up to 16 bytes
past the end of the packed stream (method 1's bit-window prefetch): callers
must provide that slack.

## Licence: UNRESOLVED

Earlier notes called this file CC0. That is not supported: the rw_junk
repository (github.com/thejunkman9000/Turok-Rage-Wars-Decomp, now
JohnSams165/Turok-Rage-Wars-Decomp) has no LICENSE file and no licence
statement for its own code; its README grants CC0 only for files copied from
melalawi/ragewars-decomp (include/their, their_syms.ld). rnc.c was written by
the rw_junk author and carries no licence header, so by default it is all
rights reserved. Before the port is distributed, either obtain an explicit
licence from the author or replace this file with a decoder written from
Turok 2's own disassembly (func_00200C40..func_002010C0, docs/ASM_ORIGIN.md).
The same applies to tools/assets/propack.py (a Python port of this file).
See docs/port/RAGEWARS_PORT.md section 0.
