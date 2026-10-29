/* 0x00295688-0x00295690: 2 words splat took for the start of a
 * function. Never executed: sw $2,0($16); lb $2,203($16).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00295688
.type func_00295688, @object
func_00295688:
    .word 0xAE020000
    .word 0x820200CB
.size func_00295688, . - func_00295688
