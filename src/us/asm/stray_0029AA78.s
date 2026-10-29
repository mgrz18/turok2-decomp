/* 0x0029AA78-0x0029AA80: 2 words splat took for the start of a
 * function. Never executed: lui $1,0x800a; lwc1 $f12,31480($1).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029AA78
.type func_0029AA78, @object
func_0029AA78:
    .word 0x3C01800A
    .word 0xC42C7AF8
.size func_0029AA78, . - func_0029AA78
