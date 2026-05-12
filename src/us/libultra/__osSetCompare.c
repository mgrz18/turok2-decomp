/*
 * src/us/libultra/__osSetCompare.c — candidate match for `__osSetCompare`
 * at T2 vram 0x800D7210 (ROM 0xD7E10).
 *
 * STATUS: BYTE-EXACT MATCH (Round 4) — NOT YET LINKED.
 *         Compiled with SN64 cc1 -O2. Wiring blocked on Makefile
 *         `C_FILES` glob.
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

#ifdef __APPLE__
void __osSetCompare(u32 value) { (void)value; } /* host-clang stub */
#else
void __osSetCompare(u32 value) {
    unsigned long w = value;
    __asm__ volatile("mtc0 %0, $11" : : "r"(w));
}
#endif
