/* 0x00298DEC-0x00298DF0: 1 words splat took for the start of a
 * function. Never executed: lh $2,112($2).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00298DEC
.type func_00298DEC, @object
func_00298DEC:
    .word 0x84420070
.size func_00298DEC, . - func_00298DEC
