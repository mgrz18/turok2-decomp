/* 0x00428A18-0x00428A20: 2 words splat took for the start of a
 * function. Never executed: two junk words before osPfsIsPlug, 16-aligned.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00428A18
.type func_00428A18, @object
func_00428A18:
    .word 0x00001821
    .word 0x02001821
.size func_00428A18, . - func_00428A18
