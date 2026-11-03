/* 0x00448238-0x00448240: 2 words splat took for the start of a
 * function. Never executed: nop; lw $2,2660($4).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00448238
.type func_00448238, @object
func_00448238:
    .word 0x00000000
    .word 0x8C820A64
.size func_00448238, . - func_00448238
