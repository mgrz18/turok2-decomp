/* 0x0044C22C-0x0044C230: 1 words splat took for the start of a
 * function. Never executed: lui $7,0x4270.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0044C22C
.type func_0044C22C, @object
func_0044C22C:
    .word 0x3C074270
.size func_0044C22C, . - func_0044C22C
