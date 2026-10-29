/* 0x0029B054-0x0029B060: 3 words splat took for the start of a
 * function. Never executed: lui $1,0x800a; lwc1 $f1,31576($1); c.lt.s $f1,$f0.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029B054
.type func_0029B054, @object
func_0029B054:
    .word 0x3C01800A
    .word 0xC4217B58
    .word 0x4600083C
.size func_0029B054, . - func_0029B054
