/* 0x0044C228-0x0044C230: 2 words splat took for the start of a
 * function. Never executed: nop; lui $7,0x4270.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0044C228
.type func_0044C228, @object
func_0044C228:
    .word 0x00000000
    .word 0x3C074270
.size func_0044C228, . - func_0044C228
