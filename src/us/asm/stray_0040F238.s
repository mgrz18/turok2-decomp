/* 0x0040F238-0x0040F240: 2 words splat took for the start of a
 * function. Never executed: addiu $2,$3,648; beq $2,$16,40f200.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0040F238
.type func_0040F238, @object
func_0040F238:
    .word 0x24620288
    .word 0x1050FFF0
.size func_0040F238, . - func_0040F238
