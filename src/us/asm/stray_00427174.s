/* 0x00427174-0x00427180: 3 words splat took for the start of a
 * function. Never executed: nop; sw $16,24($2); sw $17,20($2).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00427174
.type func_00427174, @object
func_00427174:
    .word 0x00000000
    .word 0xAC500018
    .word 0xAC510014
.size func_00427174, . - func_00427174
