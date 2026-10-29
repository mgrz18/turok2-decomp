/* 0x0045AEA8-0x0045AEB0: 2 words splat took for the start of a
 * function. Never executed: lui $2,0x800c; lw $2,8220($2).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0045AEA8
.type func_0045AEA8, @object
func_0045AEA8:
    .word 0x3C02800C
    .word 0x8C42201C
.size func_0045AEA8, . - func_0045AEA8
