/*
 * src/us/libultra/__osGetSR.c — candidate match for `__osGetSR` at
 * T2 vram 0x800D6FE0 (ROM 0xD7BE0).
 *
 * STATUS: NOT YET BYTE-EXACT, NOT LINKED INTO THE ROM.
 *         Container is missing SN64 cc1 (tools/sn64/), so we cannot
 *         compile and diff against the real bytes yet. See blocker #4
 *         in docs/LIBULTRA-MATCHING.md.
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
#ifdef __mips__
    unsigned long sr;
    __asm__ volatile("mfc0 %0, $12" : "=r"(sr));
    return (u32)sr;
#else
    return 0; /* host stub — real impl compiled inside container */
#endif
}
