/* 0x00201AC8-0x00201AD0: 2 words splat took for the start of a
 * function. Never executed: "ObjectIn".
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00201AC8
.type func_00201AC8, @object
func_00201AC8:
    .word 0x4F626A65
    .word 0x6374496E
.size func_00201AC8, . - func_00201AC8
