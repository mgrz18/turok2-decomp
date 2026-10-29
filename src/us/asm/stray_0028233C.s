/* 0x0028233C-0x00282340: 1 words splat took for the start of a
 * function. Never executed: lw $31,40($29).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0028233C
.type func_0028233C, @object
func_0028233C:
    .word 0x8FBF0028
.size func_0028233C, . - func_0028233C
