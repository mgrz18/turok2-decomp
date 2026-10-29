/* 0x0029E208-0x0029E210: 2 words splat took for the start of a
 * function. Never executed: move $16,$4; sw $31,52($29).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029E208
.type func_0029E208, @object
func_0029E208:
    .word 0x00808021
    .word 0xAFBF0034
.size func_0029E208, . - func_0029E208
