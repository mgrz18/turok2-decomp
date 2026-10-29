/* 0x00462E38-0x00462E40: 2 words splat took for the start of a
 * function. Never executed: beq $5,$2,462e84; addiu $17,$18,96.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00462E38
.type func_00462E38, @object
func_00462E38:
    .word 0x10A20012
    .word 0x26510060
.size func_00462E38, . - func_00462E38
