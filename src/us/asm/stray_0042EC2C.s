/* 0x0042EC2C-0x0042EC30: 1 words splat took for the start of a
 * function. Never executed: a padding nop before the 16-aligned func_0042EC30.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0042EC2C
.type func_0042EC2C, @object
func_0042EC2C:
    .word 0x00000000
.size func_0042EC2C, . - func_0042EC2C
