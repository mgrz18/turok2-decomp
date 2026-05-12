/*
 * src/us/libultra/osUnmapTLB.c — candidate match for `osUnmapTLB`
 * at T2 vram 0x800C7EC0 (ROM 0xC8AC0).
 *
 * Real T2 bytes (15 instructions, 60 bytes):
 *   40085000   mfc0  t0, $10         # save PageMask
 *   40840000   mtc0  a0, $0          # Index = index
 *   3C098000   lui   t1, 0x8000      # PageMask reset value
 *   40895000   mtc0  t1, $10
 *   40801000   mtc0  zero, $2        # EntryLo0 = 0
 *   40801800   mtc0  zero, $3        # EntryLo1 = 0
 *   00000000   nop
 *   42000002   tlbwi
 *   00000000   nop x4
 *   40885000   mtc0  t0, $10         # restore PageMask
 *   (epilogue jr ra; nop from cc1)
 */

typedef int s32;

#ifdef __APPLE__
void osUnmapTLB(s32 index) { (void)index; }
#else
void osUnmapTLB(s32 index) {
    (void)index;
    __asm__ volatile(
        ".set noreorder\n\t"
        "mfc0  $8, $10\n\t"
        "mtc0  $4, $0\n\t"
        "lui   $9, 0x8000\n\t"
        "mtc0  $9, $10\n\t"
        "mtc0  $0, $2\n\t"
        "mtc0  $0, $3\n\t"
        "nop\n\t"
        "tlbwi\n\t"
        "nop\n\t"
        "nop\n\t"
        "nop\n\t"
        "nop\n\t"
        "mtc0  $8, $10\n\t"
        ".set reorder"
        : : : "$8", "$9");
}
#endif
