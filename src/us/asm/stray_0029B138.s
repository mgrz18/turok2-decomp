/* 0x0029B138-0x0029B140: 2 words splat took for the start of a
 * function. Never executed: lw $18,24($29); lw $17,20($29).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029B138
.type func_0029B138, @object
func_0029B138:
    .word 0x8FB20018
    .word 0x8FB10014
.size func_0029B138, . - func_0029B138
