/* 0x0029A078-0x0029A080: 2 words splat took for the start of a
 * function. Never executed: sw $16,16($29); move $16,$5.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029A078
.type func_0029A078, @object
func_0029A078:
    .word 0xAFB00010
    .word 0x00A08021
.size func_0029A078, . - func_0029A078
