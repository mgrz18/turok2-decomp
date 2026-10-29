/* 0x00452EA8-0x00452EB0: 2 words splat took for the start of a
 * function. Never executed: lui $2,0x8012; lbu $2,-21779($2).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00452EA8
.type func_00452EA8, @object
func_00452EA8:
    .word 0x3C028012
    .word 0x9042AAED
.size func_00452EA8, . - func_00452EA8
