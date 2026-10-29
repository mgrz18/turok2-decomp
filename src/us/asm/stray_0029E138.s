/* 0x0029E138-0x0029E140: 2 words splat took for the start of a
 * function. Never executed: j 26514c; nop.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029E138
.type func_0029E138, @object
func_0029E138:
    .word 0x08099453
    .word 0x00000000
.size func_0029E138, . - func_0029E138
