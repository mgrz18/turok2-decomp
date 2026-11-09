# rnc.c

RNC ProPack methods 1 and 2 unpacker, copied unchanged from
`references/rw_junk/docs/hand_asm_c/rnc.c` (the Turok: Rage Wars decomp's
functionally equivalent C reconstruction of the game's hand-written
`rncUnpackMethod1/2`, differentially tested against the original machine code;
CC0 as stated in docs/assets/PORT.md section 8). Turok 2 uses the same
ProPack unpackers (`RNC_UnpackMethod1/2`, hand-written assembly called by
`ProPackDecompress`, `func_00200B00.c`).

Used by `port/assets/t2asset.c` (ROM backend: `T2Rnc_Unpack`, which adds
the header CRC16 check the game skips). The unpacker may read up to 16 bytes
past the end of the packed stream (method 1's bit-window prefetch): callers
must provide that slack.
