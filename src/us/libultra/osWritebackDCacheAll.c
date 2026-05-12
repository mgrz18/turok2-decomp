/*
 * src/us/libultra/osWritebackDCacheAll.c — candidate match for
 * `osWritebackDCacheAll` at T2 vram 0x800D73B0 (ROM 0xD7FB0).
 *
 * Real T2 bytes (10 instructions, 40 bytes):
 *   3C088000   lui   $8, 0x8000
 *   240A2000   addiu $10, $0, 0x2000
 *   010A4821   addu  $9, $8, $10
 *   2529FFF0   addiu $9, $9, -0x10
 *   BD010000   cache 0x01, 0($8)        # index writeback inv D
 *   0109082B   sltu  $1, $8, $9
 *   1420FFFD   bnez  $1, .loop
 *   25080010    addiu $8, $8, 0x10
 *   03E00008   jr    ra
 *   00000000   nop
 *
 * Writeback entire 8KiB D-cache (256 lines * 16 bytes index step / 32B line).
 */

#ifdef __APPLE__
void osWritebackDCacheAll(void) {}
#else
void osWritebackDCacheAll(void) {
    __asm__ volatile(
        ".set noreorder\n\t"
        "lui   $8, 0x8000\n\t"
        "addiu $10, $0, 0x2000\n\t"
        "addu  $9, $8, $10\n\t"
        "addiu $9, $9, -0x10\n"
        "1:\n\t"
        "cache 0x01, 0x0($8)\n\t"
        "sltu  $1, $8, $9\n\t"
        "bnez  $1, 1b\n\t"
        " addiu $8, $8, 0x10\n\t"
        ".set reorder"
        : : : "$1", "$8", "$9", "$10");
}
#endif
