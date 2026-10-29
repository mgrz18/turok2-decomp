/* 0x00226F8C-0x00226F90: 1 words splat took for the start of a
 * function. Never executed: j 9f029f8.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00226F8C
.type func_00226F8C, @object
func_00226F8C:
    .word 0x0A7C0A7E
.size func_00226F8C, . - func_00226F8C
