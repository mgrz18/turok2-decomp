/* 0x0022723C-0x00227240: 1 words splat took for the start of a
 * function. Never executed: nop.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0022723C
.type func_0022723C, @object
func_0022723C:
    .word 0x00000000
.size func_0022723C, . - func_0022723C
