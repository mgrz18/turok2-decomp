/* 0x0020285C-0x00202860: 1 words splat took for the start of a
 * function. Never executed: "temD".
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0020285C
.type func_0020285C, @object
func_0020285C:
    .word 0x74656D44
.size func_0020285C, . - func_0020285C
