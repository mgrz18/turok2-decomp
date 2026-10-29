/* 0x0029E3A8-0x0029E3B0: 2 words splat took for the start of a
 * function. Never executed: lwc1 $f1,100($17); mtc1 $0,$f0.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029E3A8
.type func_0029E3A8, @object
func_0029E3A8:
    .word 0xC6210064
    .word 0x44800000
.size func_0029E3A8, . - func_0029E3A8
