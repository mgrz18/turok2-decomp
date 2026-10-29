/* 0x00427178-0x00427180: 2 words splat took for the start of a
 * function. Never executed: sw $16,24($2); sw $17,20($2).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00427178
.type func_00427178, @object
func_00427178:
    .word 0xAC500018
    .word 0xAC510014
.size func_00427178, . - func_00427178
