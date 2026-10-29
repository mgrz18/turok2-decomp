/* 0x002847C8-0x002847D0: 2 words splat took for the start of a
 * function. Never executed: nop, bc1fl into func_002847E0.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_002847C8
.type func_002847C8, @object
func_002847C8:
    .word 0x00000000
    .word 0x45020008
.size func_002847C8, . - func_002847C8
