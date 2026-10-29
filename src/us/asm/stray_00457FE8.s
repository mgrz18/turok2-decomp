/* 0x00457FE8-0x00457FF0: 2 words splat took for the start of a
 * function. Never executed: lui $2,0x8013; lw $2,2448($2).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00457FE8
.type func_00457FE8, @object
func_00457FE8:
    .word 0x3C028013
    .word 0x8C420990
.size func_00457FE8, . - func_00457FE8
