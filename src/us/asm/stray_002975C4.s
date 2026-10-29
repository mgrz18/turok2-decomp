/* 0x002975C4-0x002975D0: 3 words splat took for the start of a
 * function. Never executed: mov.s, c.lt.s, nop after cosf; guLookAt starts at 0x2975D0.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_002975C4
.type func_002975C4, @object
func_002975C4:
    .word 0x46000086
    .word 0x4601103C
    .word 0x00000000
.size func_002975C4, . - func_002975C4
