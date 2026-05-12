/*
 * src/us/libultra/__osDisableInt.c — candidate match for the libultra
 * interrupt-disable primitive at T2 vram 0x800D6BF0 (ROM 0xD77F0).
 *
 * Public source reference: pmret/papermario, n64decomp/sm64 lib/src/os/
 * (BSD/CC0). Adapted for SN64 cc1.
 *
 * STATUS: NOT YET BYTE-EXACT, NOT LINKED INTO THE ROM.
 *
 * Real T2 bytes (from build/scratch/libultra.dis):
 *   mfc0  t0,$12              # save SR into t0
 *   li    at,-2               # mask = ~0x1
 *   and   t1,t0,at             # cleared SR in t1
 *   mtc0  t1,$12              # write back disabled SR
 *   andi  v0,t0,0x1            # return = old IE bit
 *   nop
 *   jr    ra
 *   nop
 *
 * The version below, compiled with SN64 cc1 -O2, currently produces 6
 * instructions using v0/v1/a0 instead of t0/t1/at — register allocation
 * and instruction ordering both differ from the original. See
 * docs/LIBULTRA-MATCHING.md for the diff and notes on how to coax the
 * matching codegen out of cc1.
 */

typedef unsigned int OSIntMask;

OSIntMask __osDisableInt(void) {
    register unsigned int sr;
    register unsigned int prev;

    __asm__ volatile("mfc0 %0, $12" : "=r"(sr));
    prev = sr & 1;
    __asm__ volatile("mtc0 %0, $12" : : "r"(sr & ~1));

    return prev;
}
