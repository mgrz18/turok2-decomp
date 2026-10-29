/* 0x0029B01C-0x0029B020: 1 words splat took for the start of a
 * function. Never executed: jal 246108.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029B01C
.type func_0029B01C, @object
func_0029B01C:
    .word 0x0C091842
.size func_0029B01C, . - func_0029B01C
