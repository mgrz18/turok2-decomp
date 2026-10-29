/* 0x00262C18-0x00262C20: 2 words splat took for the start of a
 * function. Never executed: mov.s $f21,$f0; c.eq.s $f21,$f22.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00262C18
.type func_00262C18, @object
func_00262C18:
    .word 0x46000546
    .word 0x4616A832
.size func_00262C18, . - func_00262C18
