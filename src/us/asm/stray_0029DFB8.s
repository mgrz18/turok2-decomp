/* 0x0029DFB8-0x0029DFC0: 2 words splat took for the start of a
 * function. Never executed: lui $1,0x800a; lwc1 $f14,31852($1).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029DFB8
.type func_0029DFB8, @object
func_0029DFB8:
    .word 0x3C01800A
    .word 0xC42E7C6C
.size func_0029DFB8, . - func_0029DFB8
