/* 0x004595D8-0x004595E0: 2 words splat took for the start of a
 * function. Never executed: lui $2,0x8013; lw $2,2448($2).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_004595D8
.type func_004595D8, @object
func_004595D8:
    .word 0x3C028013
    .word 0x8C420990
.size func_004595D8, . - func_004595D8
