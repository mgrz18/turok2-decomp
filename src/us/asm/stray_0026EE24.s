/* 0x0026EE24-0x0026EE30: 3 words splat took for the start of a
 * function. Never executed: c.lt.s, nop, bc1f into func_0026EE40.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0026EE24
.type func_0026EE24, @object
func_0026EE24:
    .word 0x4600083C
    .word 0x00000000
    .word 0x45000011
.size func_0026EE24, . - func_0026EE24
