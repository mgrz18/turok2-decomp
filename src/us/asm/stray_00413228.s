/* 0x00413228-0x00413230: 2 words splat took for the start of a
 * function. Never executed: a padding nop and a junk word between func_004131B0 and func_00413230.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00413228
.type func_00413228, @object
func_00413228:
    .word 0x00000000
    .word 0x45020046
.size func_00413228, . - func_00413228
