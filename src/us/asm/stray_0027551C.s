/* 0x0027551C-0x00275520: 1 words splat took for the start of a
 * function. Never executed: mul.s $f0,$f0,$f1.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0027551C
.type func_0027551C, @object
func_0027551C:
    .word 0x46010002
.size func_0027551C, . - func_0027551C
