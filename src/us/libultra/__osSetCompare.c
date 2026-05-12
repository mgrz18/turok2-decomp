/*
 * src/us/libultra/__osSetCompare.c — candidate match for `__osSetCompare`
 * at T2 vram 0x800D7210 (ROM 0xD7E10).
 *
 * STATUS: NOT YET BYTE-EXACT, NOT LINKED INTO THE ROM.
 *         Pending SN64 cc1 in the container (see blocker #4 in
 *         docs/LIBULTRA-MATCHING.md).
 *
 * Real T2 bytes at 0x800D7210 (3 instructions, 12 bytes):
 *   40845800   mtc0  a0, $11        # write Compare register
 *   03E00008   jr    ra
 *   00000000   nop
 *
 * Unlike __osSetSR, there is NO leading hazard nop here — the original
 * libultra accepts the COP0 latency on Compare since the value is read
 * by the timer interrupt at the next tick anyway.
 */

typedef unsigned int u32;

void __osSetCompare(u32 value) {
    __asm__ volatile("mtc0 %0, $11" : : "r"(value));
}
