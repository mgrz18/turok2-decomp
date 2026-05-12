/*
 * src/us/libultra/__osSetSR.c — candidate match for `__osSetSR` at
 * T2 vram 0x800D72E0 (ROM 0xD7EE0).
 *
 * STATUS: BYTE-EXACT MATCH (Round 4) — NOT YET LINKED.
 *         Compiled with SN64 cc1 -O2 (full flags in
 *         docs/LIBULTRA-MATCHING.md). Wiring blocked on Makefile
 *         `C_FILES` glob.
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
    unsigned long w = sr;
    __asm__ volatile("mtc0 %0, $12\n\tnop" : : "r"(w));
}
