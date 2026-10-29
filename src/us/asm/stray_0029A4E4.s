/* 0x0029A4E4-0x0029A4F0: 3 words splat took for the start of a
 * function. Never executed: c.lt.s $f14,$f0; nop; bc1f 29a4f8.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029A4E4
.type func_0029A4E4, @object
func_0029A4E4:
    .word 0x4600703C
    .word 0x00000000
    .word 0x45000002
.size func_0029A4E4, . - func_0029A4E4
