/* 0x00269A8C-0x00269A90: 1 words splat took for the start of a
 * function. Never executed: lw $19,684($29).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00269A8C
.type func_00269A8C, @object
func_00269A8C:
    .word 0x8FB302AC
.size func_00269A8C, . - func_00269A8C
