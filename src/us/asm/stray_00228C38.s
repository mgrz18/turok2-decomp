/* 0x00228C38-0x00228C40: 2 words splat took for the start of a
 * function. Never executed: two data words ahead of func_00228C40.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00228C38
.type func_00228C38, @object
func_00228C38:
    .word 0x188719FF
    .word 0x1921160B
.size func_00228C38, . - func_00228C38
