/* 0x0028FA0C-0x0028FA10: 1 words splat took for the start of a
 * function. Never executed: lhu $3,2($3).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0028FA0C
.type func_0028FA0C, @object
func_0028FA0C:
    .word 0x94630002
.size func_0028FA0C, . - func_0028FA0C
