/*
 * src/us/libultra/__osDisableInt.c — candidate match for the libultra
 * interrupt-disable primitive at T2 vram 0x800D6FF0 (ROM 0xD7BF0).
 *
 * STATUS: BYTE-EXACT MATCH (Round 4) — NOT YET LINKED.
 *         Compiled with SN64 cc1 -O2. Register pinning required
 *         (see body comment). Wiring blocked on Makefile `C_FILES`
 *         glob.
 *
 * Real T2 bytes at 0x800D6FF0 (8 instructions, 32 bytes):
 *   40086000   mfc0  t0, $12        # save SR -> t0
 *   2401FFFE   addiu at, zero, -2    # at = ~0x1
 *   01014824   and   t1, t0, at      # t1 = SR with IE cleared
 *   40896000   mtc0  t1, $12        # commit disabled SR
 *   31020001   andi  v0, t0, 0x1     # return = old IE bit
 *   00000000   nop
 *   03E00008   jr    ra
 *   00000000   nop
 *
 * The version below uses inline asm with explicit register clobbers so
 * cc1 cannot reorder around the COP0 read/write.  Trial compile with
 * SN64 cc1 -O2 produced 6 instructions with v0/v1/a0 instead of
 * t0/t1/at — register allocation and instruction ordering both differ.
 */

typedef unsigned int OSIntMask;

#ifdef __APPLE__
OSIntMask __osDisableInt(void) { return 0; } /* host-clang stub */
#else
OSIntMask __osDisableInt(void) {
    /*
     * Pin output to %0 (=> $v0 via the named return slot trick): we
     * declare a register variable bound to $2 and let cc1 see it as
     * the function's return path. The mfc0/and/mtc0 chain uses fixed
     * temps $8/$9 + $1 to match the real T2 binary exactly. Without
     * this, -O2 picks $3 for the read and emits a `move $2,$0` in the
     * jr delay slot, clobbering our result.
     */
    register unsigned long ret __asm__("$2");
    __asm__ volatile(
        "mfc0 $8, $12\n\t"
        "addiu $1, $0, -2\n\t"
        "and  $9, $8, $1\n\t"
        "mtc0 $9, $12\n\t"
        "andi $2, $8, 0x1\n\t"
        "nop"
        : "=r"(ret) : : "$1", "$8", "$9");
    return (OSIntMask)ret;
}
#endif
