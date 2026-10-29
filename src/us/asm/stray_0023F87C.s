/* 0x0023F87C-0x0023F880: 1 words splat took for the start of a
 * function. Never executed: nop.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0023F87C
.type func_0023F87C, @object
func_0023F87C:
    .word 0x00000000
.size func_0023F87C, . - func_0023F87C
