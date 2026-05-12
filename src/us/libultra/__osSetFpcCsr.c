/*
 * src/us/libultra/__osSetFpcCsr.c — candidate match for `__osSetFpcCsr`
 * at T2 vram 0x800D7220 (ROM 0xD7E20).
 *
 * Real T2 bytes (4 instructions, 16 bytes):
 *   44425800   cfc1  v0, $31        # return old fpcsr
 *   44845800   ctc1  a0, $31        # set new fpcsr
 *   03E00008   jr    ra
 *   00000000   nop
 */

typedef unsigned int u32;

#ifdef __APPLE__
u32 __osSetFpcCsr(u32 value) { (void)value; return 0; }
#else
u32 __osSetFpcCsr(u32 value) {
    register unsigned long ret __asm__("$2");
    __asm__ volatile(
        "cfc1 $2, $31\n\t"
        "ctc1 $4, $31"
        : "=r"(ret));
    return (u32)ret;
}
#endif
