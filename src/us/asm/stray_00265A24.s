/* 0x00265A24-0x00265A30: 3 words splat took for the start of a
 * function. Never executed: nop, beq, sll; the init callback's pointer in the data is 0x00265A30.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00265A24
.type func_00265A24, @object
func_00265A24:
    .word 0x00000000
    .word 0x1250000D
    .word 0x001210C0
.size func_00265A24, . - func_00265A24
