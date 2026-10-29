/* 0x0041BA48-0x0041BA50: 2 words splat took for the start of a
 * function. Never executed: li $5,16; lui $4,0x800f.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0041BA48
.type func_0041BA48, @object
func_0041BA48:
    .word 0x24050010
    .word 0x3C04800F
.size func_0041BA48, . - func_0041BA48
