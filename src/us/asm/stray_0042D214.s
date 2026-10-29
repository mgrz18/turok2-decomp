/* 0x0042D214-0x0042D220: 3 words splat took for the start of a
 * function. Never executed: addiu $4,$29,32; move $5,$17; jal 20ef2c.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0042D214
.type func_0042D214, @object
func_0042D214:
    .word 0x27A40020
    .word 0x02202821
    .word 0x0C083BCB
.size func_0042D214, . - func_0042D214
