/* 0x0029E21C-0x0029E220: 1 words splat took for the start of a
 * function. Never executed: addiu $18,$2,20.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029E21C
.type func_0029E21C, @object
func_0029E21C:
    .word 0x24520014
.size func_0029E21C, . - func_0029E21C
