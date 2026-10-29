/* 0x0029A138-0x0029A140: 2 words splat took for the start of a
 * function. Never executed: lui $7,0x41f0; sw $31,20($29).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029A138
.type func_0029A138, @object
func_0029A138:
    .word 0x3C0741F0
    .word 0xAFBF0014
.size func_0029A138, . - func_0029A138
