/* 0x00293574-0x00293580: 3 words splat took for the start of a
 * function. Never executed: jr ra, addiu $sp (an epilogue), addiu $29,$29,-24.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00293574
.type func_00293574, @object
func_00293574:
    .word 0x03E00008
    .word 0x27BD0018
    .word 0x27BDFFE8
.size func_00293574, . - func_00293574
