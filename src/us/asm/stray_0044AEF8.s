/* 0x0044AEF8-0x0044AF00: 2 words splat took for the start of a
 * function. Never executed: lui $2,0x800c; lw $2,7088($2).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0044AEF8
.type func_0044AEF8, @object
func_0044AEF8:
    .word 0x3C02800C
    .word 0x8C421BB0
.size func_0044AEF8, . - func_0044AEF8
