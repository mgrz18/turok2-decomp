/* 0x0029BC24-0x0029BC30: 3 words splat took for the start of a
 * function. Never executed: beq, slti, beqz into func_0029BC70.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029BC24
.type func_0029BC24, @object
func_0029BC24:
    .word 0x1082004D
    .word 0x288205F4
    .word 0x10400016
.size func_0029BC24, . - func_0029BC24
