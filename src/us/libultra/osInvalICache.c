/*
 * src/us/libultra/osInvalICache.c — candidate match for `osInvalICache`
 * at T2 vram 0x800D7340 (ROM 0xD7F40).
 *
 * Real T2 bytes (26 instructions, 104 bytes):
 *   18A0000E   blez   a1, ret
 *   240B2000   addiu  t3, zero, 0x2000
 *   00AB082B   sltu   at, a1, t3
 *   1020000D   beqz   at, full_path
 *   00804021    addu   t0, a0, zero  [delay]
 *   00854821   addu   t1, a0, a1
 *   0109082B   sltu   at, t0, t1
 *   10200007   beqz   at, ret
 *   2529FFF0    addiu  t1, t1, -0x10 [delay]
 *   310A000F   andi   t2, t0, 0xF
 *   010A4023   subu   t0, t0, t2
 * loop1:
 *   BD190000   cache  0x19, 0(t0)
 *   0109082B   sltu   at, t0, t1
 *   1420FFFD   bnez   at, loop1
 *   25080010    addiu  t0, t0, 0x10 [delay]
 *   03E00008   jr     ra
 *   00000000   nop
 * full_path:
 *   3C088000   lui    t0, 0x8000
 *   010B4821   addu   t1, t0, t3
 *   2529FFF0   addiu  t1, t1, -0x10
 * loop2:
 *   BD010000   cache  0x01, 0(t0)
 *   0109082B   sltu   at, t0, t1
 *   1420FFFD   bnez   at, loop2
 *   25080010    addiu  t0, t0, 0x10 [delay]
 *   (epilogue: jr ra; nop from cc1)
 */

#ifdef __APPLE__
void osInvalICache(void *vaddr, int nbytes) { (void)vaddr; (void)nbytes; }
#else
void osInvalICache(void *vaddr, int nbytes) {
    (void)vaddr; (void)nbytes;
    __asm__ volatile(
        ".set noreorder\n\t"
        "blez  $5, 8f\n\t"
        " addiu $11, $0, 0x2000\n\t"
        "sltu  $1, $5, $11\n\t"
        "beqz  $1, 2f\n\t"
        " addu  $8, $4, $0\n\t"
        "addu  $9, $4, $5\n\t"
        "sltu  $1, $8, $9\n\t"
        "beqz  $1, 8f\n\t"
        " addiu $9, $9, -0x10\n\t"
        "andi  $10, $8, 0xF\n\t"
        "subu  $8, $8, $10\n"
        "1:\n\t"
        "cache 0x19, 0x0($8)\n\t"
        "sltu  $1, $8, $9\n\t"
        "bnez  $1, 1b\n\t"
        " addiu $8, $8, 0x10\n"
        "8:\n\t"
        "jr    $31\n\t"
        " nop\n"
        "2:\n\t"
        "lui   $8, 0x8000\n\t"
        "addu  $9, $8, $11\n\t"
        "addiu $9, $9, -0x10\n"
        "3:\n\t"
        "cache 0x01, 0x0($8)\n\t"
        "sltu  $1, $8, $9\n\t"
        "bnez  $1, 3b\n\t"
        " addiu $8, $8, 0x10\n\t"
        ".set reorder"
        : : : "$1", "$8", "$9", "$10", "$11");
}
#endif
