/* 0x00453308-0x00453310: 2 words splat took for the start of a
 * function. Never executed: mtc1 $0,$f1; lui $1,0x800b.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00453308
.type func_00453308, @object
func_00453308:
    .word 0x44800800
    .word 0x3C01800B
.size func_00453308, . - func_00453308
