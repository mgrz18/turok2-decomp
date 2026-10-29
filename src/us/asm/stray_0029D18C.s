/* 0x0029D18C-0x0029D190: 1 words splat took for the start of a
 * function. Never executed: lw $31,16($29).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029D18C
.type func_0029D18C, @object
func_0029D18C:
    .word 0x8FBF0010
.size func_0029D18C, . - func_0029D18C
