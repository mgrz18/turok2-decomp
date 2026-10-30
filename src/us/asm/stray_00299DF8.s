/* 0x00299DF8-0x00299E00: 2 words splat took for the start of a
 * function. Never executed: c.lt.s, nop.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00299DF8
.type func_00299DF8, @object
func_00299DF8:
    .word 0x4600083C
    .word 0x00000000
.size func_00299DF8, . - func_00299DF8
