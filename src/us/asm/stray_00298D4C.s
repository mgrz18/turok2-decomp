/* 0x00298D4C-0x00298D50: 1 words splat took for the start of a
 * function. Never executed: lw $31,16($29).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00298D4C
.type func_00298D4C, @object
func_00298D4C:
    .word 0x8FBF0010
.size func_00298D4C, . - func_00298D4C
