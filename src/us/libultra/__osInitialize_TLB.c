/*
 * src/us/libultra/__osInitialize_TLB.c — candidate match for
 * `__osInitialize_TLB` at T2 vram 0x800D72F0 (ROM 0xD7EF0).
 *
 * Real T2 bytes (17 instructions, 68 bytes):
 *   40085000   mfc0  t0, $10           save PageMask
 *   2409001E   addiu t1, zero, 30      30 entries
 *   3C0A8000   lui   t2, 0x8000
 *   408A5000   mtc0  t2, $10           PageMask = 0x80000000
 *   40801000   mtc0  zero, $2          EntryLo0
 *   40801800   mtc0  zero, $3          EntryLo1
 * loop:
 *   40890000   mtc0  t1, $0            Index
 *   00000000   nop
 *   42000002   tlbwi
 *   00000000   nop
 *   00000000   nop
 *   2129FFFF   addi  t1, t1, -1
 *   0521FFF9   bgez  t1, loop
 *   00000000    nop                    [delay]
 *   40885000   mtc0  t0, $10           restore PageMask
 *   (epilogue jr ra; nop from cc1)
 */

#ifdef __APPLE__
void __osInitialize_TLB(void) {}
#else
void __osInitialize_TLB(void) {
    __asm__ volatile(
        ".set noreorder\n\t"
        "mfc0  $8, $10\n\t"
        "addiu $9, $0, 0x1E\n\t"
        "lui   $10, 0x8000\n\t"
        "mtc0  $10, $10\n\t"
        "mtc0  $0, $2\n\t"
        "mtc0  $0, $3\n"
        "1:\n\t"
        "mtc0  $9, $0\n\t"
        "nop\n\t"
        "tlbwi\n\t"
        "nop\n\t"
        "nop\n\t"
        "addi  $9, $9, -1\n\t"
        "bgez  $9, 1b\n\t"
        " nop\n\t"
        "mtc0  $8, $10\n\t"
        ".set reorder"
        : : : "$8", "$9", "$10");
}
#endif
