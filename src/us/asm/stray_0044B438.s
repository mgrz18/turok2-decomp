/* 0x0044B438-0x0044B440: 2 words splat took for the start of a
 * function. Never executed: lui $2,0x800c; lw $2,7096($2).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0044B438
.type func_0044B438, @object
func_0044B438:
    .word 0x3C02800C
    .word 0x8C421BB8
.size func_0044B438, . - func_0044B438
