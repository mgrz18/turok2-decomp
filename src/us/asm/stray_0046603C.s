/* 0x0046603C-0x00466040: 1 words splat took for the start of a
 * function. Never executed: li $2,2.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0046603C
.type func_0046603C, @object
func_0046603C:
    .word 0x24020002
.size func_0046603C, . - func_0046603C
