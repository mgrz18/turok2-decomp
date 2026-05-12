/*
 * src/us/libultra/__osSetSR.c — candidate match for `__osSetSR` at
 * T2 vram 0x800D72E0 (ROM 0xD7EE0).
 *
 * STATUS: NOT YET BYTE-EXACT, NOT LINKED INTO THE ROM.
 *         Pending SN64 cc1 in the container (see blocker #4 in
 *         docs/LIBULTRA-MATCHING.md).
 *
 * Real T2 bytes at 0x800D72E0 (4 instructions, 16 bytes):
 *   40846000   mtc0  a0, $12        # write Status register
 *   00000000   nop
 *   03E00008   jr    ra
 *   00000000   nop
 *
 * Note the trailing nop in the branch delay slot AND a leading nop
 * after mtc0 — the original libultra coding convention inserts a
 * COP0 hazard nop between mtc0/$12 and the next instruction.
 */

typedef unsigned int u32;

void __osSetSR(u32 sr) {
#ifdef __mips__
    unsigned long w = sr;
    __asm__ volatile("mtc0 %0, $12\nnop" : : "r"(w));
#else
    (void)sr; /* host stub */
#endif
}
