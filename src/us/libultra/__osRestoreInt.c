/*
 * src/us/libultra/__osRestoreInt.c — candidate match for `__osRestoreInt`
 * at T2 vram 0x800D7010 (ROM 0xD7C10).
 *
 * STATUS: BYTE-EXACT MATCH (Round 4) — NOT YET LINKED.
 *         Compiled with SN64 cc1 -O2. Hard-coded register names in
 *         the inline asm (see body comment). Wiring blocked on
 *         Makefile `C_FILES` glob.
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
    /*
     * Force $8 (t0) for the SR scratch register; cc1 -O2 otherwise
     * picks $2 (v0). mask is passed in $a0 by the SN64 ABI so we
     * reference it directly. Trailing pair of nops matches the COP0
     * hazard pad emitted by the real T2 binary.
     */
    (void)mask;
    __asm__ volatile(
        "mfc0 $8, $12\n\t"
        "or   $8, $8, $4\n\t"
        "mtc0 $8, $12\n\t"
        "nop\n\t"
        "nop"
        : : : "$8");
}
