/* 0x0029B1EC-0x0029B1F0: 1 words splat took for the start of a
 * function. Never executed: j 262314.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029B1EC
.type func_0029B1EC, @object
func_0029B1EC:
    .word 0x080988C5
.size func_0029B1EC, . - func_0029B1EC
