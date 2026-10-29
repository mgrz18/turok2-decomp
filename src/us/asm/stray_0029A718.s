/* 0x0029A718-0x0029A720: 2 words splat took for the start of a
 * function. Never executed: bne into osCreateViManager.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029A718
.type func_0029A718, @object
func_0029A718:
    .word 0x1462000C
    .word 0x00000000
.size func_0029A718, . - func_0029A718
