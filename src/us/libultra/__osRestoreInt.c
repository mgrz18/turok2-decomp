/*
 * src/us/libultra/__osRestoreInt.c — candidate match for `__osRestoreInt`
 * at T2 vram 0x800D7010 (ROM 0xD7C10).
 *
 * STATUS: NOT YET BYTE-EXACT, NOT LINKED INTO THE ROM.
 *         Pending SN64 cc1 in the container (see blocker #4 in
 *         docs/LIBULTRA-MATCHING.md).
 *
 * Real T2 bytes at 0x800D7010 (7 instructions, 28 bytes):
 *   40086000   mfc0  t0, $12        # read Status
 *   01044025   or    t0, t0, a0      # OR in the saved mask
 *   40886000   mtc0  t0, $12        # write back Status
 *   00000000   nop
 *   00000000   nop
 *   03E00008   jr    ra
 *   00000000   nop
 *
 * The trailing two nops (after the mtc0 but before jr ra) match the
 * SN64 cc1 -O2 "two-cycle hazard" pattern.  Public libultra sources
 * usually emit only ONE post-mtc0 nop, so this function will require
 * a specific cc1 quirk or inline asm to reproduce.
 */

typedef unsigned int u32;

void __osRestoreInt(u32 mask) {
#ifdef __mips__
    unsigned long sr;
    unsigned long m = mask;
    __asm__ volatile(
        "mfc0 %0, $12\n"
        "or   %0, %0, %1\n"
        "mtc0 %0, $12\n"
        "nop\n"
        "nop\n"
        : "=&r"(sr)
        : "r"(m));
#else
    (void)mask; /* host stub */
#endif
}
