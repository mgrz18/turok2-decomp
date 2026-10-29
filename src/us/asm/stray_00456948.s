/* 0x00456948-0x00456950: 2 words splat took for the start of a
 * function. Never executed: lui $2,0x8013; lw $2,2448($2).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00456948
.type func_00456948, @object
func_00456948:
    .word 0x3C028013
    .word 0x8C420990
.size func_00456948, . - func_00456948
