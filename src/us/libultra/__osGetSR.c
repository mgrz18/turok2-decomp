/*
 * src/us/libultra/__osGetSR.c — candidate match for `__osGetSR` at
 * T2 vram 0x800D6FE0 (ROM 0xD7BE0).
 *
 * STATUS: BYTE-EXACT MATCH (Round 4) — NOT YET LINKED.
 *         Compiled with `tools/sn64/cc1 -quiet -G0 -mips3 -O2 -mgas
 *         -meb -mcpu=VR4300 -mhard-float -mfp64` inside the
 *         `turok2-build` container; produced object diffs zero bytes
 *         against the docstring table below.
 *         Wiring blocked on Makefile `C_FILES` glob (see
 *         docs/LIBULTRA-MATCHING.md "Makefile diff request").
 *
 * Real T2 bytes at 0x800D6FE0 (3 instructions, 12 bytes):
 *   40026000   mfc0  v0, $12        # Status register -> v0
 *   03E00008   jr    ra
 *   00000000   nop
 *
 * This is a leaf accessor with no stack frame.  Public libultra sources
 * (sm64/papermario) implement it as 100% inline asm; we follow suit so
 * cc1 cannot reorder anything around the COP0 read.
 */

typedef unsigned int u32;

u32 __osGetSR(void) {
    unsigned long sr;
    __asm__ volatile("mfc0 %0, $12" : "=r"(sr));
    return (u32)sr;
}
