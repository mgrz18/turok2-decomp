/* 0x00412548-0x00412550: 2 words splat took for the start of a
 * function. Never executed: lui $2,0x800c; lw $2,7096($2).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00412548
.type func_00412548, @object
func_00412548:
    .word 0x3C02800C
    .word 0x8C421BB8
.size func_00412548, . - func_00412548
