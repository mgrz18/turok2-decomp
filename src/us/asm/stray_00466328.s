/* 0x00466328-0x00466330: 2 words splat took for the start of a
 * function. Never executed: jal 291b00; move $6,$0.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00466328
.type func_00466328, @object
func_00466328:
    .word 0x0C0A46C0
    .word 0x00003021
.size func_00466328, . - func_00466328
