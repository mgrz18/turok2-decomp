/* 0x0029A574-0x0029A580: 3 words splat took for the start of a
 * function. Never executed: lui $1,0x800a; lwc1 $f14,31372($1); mul.s $f14,$f20,$f14.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029A574
.type func_0029A574, @object
func_0029A574:
    .word 0x3C01800A
    .word 0xC42E7A8C
    .word 0x460EA382
.size func_0029A574, . - func_0029A574
