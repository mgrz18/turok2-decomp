/* 0x0044B998-0x0044B9A0: 2 words splat took for the start of a
 * function. Never executed: lui $3,0x800c; lw $3,7088($3).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0044B998
.type func_0044B998, @object
func_0044B998:
    .word 0x3C03800C
    .word 0x8C631BB0
.size func_0044B998, . - func_0044B998
