/* 0x00467A6C-0x00467A70: 1 words splat took for the start of a
 * function. Never executed: the same fill in the second-module copy.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00467A6C
.type func_00467A6C, @object
func_00467A6C:
    .word 0x24630001
.size func_00467A6C, . - func_00467A6C
