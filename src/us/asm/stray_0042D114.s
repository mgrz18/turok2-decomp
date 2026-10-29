/* 0x0042D114-0x0042D120: 3 words splat took for the start of a
 * function. Never executed: add.s $f2,$f2,$f0; mtc1 $0,$f20; c.le.s $f20,$f2.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0042D114
.type func_0042D114, @object
func_0042D114:
    .word 0x46001080
    .word 0x4480A000
    .word 0x4602A03E
.size func_0042D114, . - func_0042D114
