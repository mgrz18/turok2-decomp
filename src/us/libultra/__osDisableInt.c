/*
 * src/us/libultra/__osDisableInt.c — candidate match for the libultra
 * interrupt-disable primitive at T2 vram 0x800D6FF0 (ROM 0xD7BF0).
 *
 * STATUS: NOT YET BYTE-EXACT, NOT LINKED INTO THE ROM.
 *         Pending SN64 cc1 in the container (see blocker #4 in
 *         docs/LIBULTRA-MATCHING.md).  Previous notes had the vram off
 *         by 0x400 (0x800D6BF0); corrected after the splat libultra
 *         vram fix landed.
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

OSIntMask __osDisableInt(void) {
    OSIntMask sr;
    OSIntMask prev;
    __asm__ volatile(
        "mfc0 %0, $12\n"
        "addiu $1, $0, -2\n"
        "and  $9, %0, $1\n"
        "mtc0 $9, $12\n"
        "andi %1, %0, 0x1\n"
        "nop\n"
        : "=r"(sr), "=r"(prev)
        :
        : "$1", "$9");
    return prev;
}
