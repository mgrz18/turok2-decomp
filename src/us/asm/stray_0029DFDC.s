/* 0x0029DFDC-0x0029DFE0: 1 words splat took for the start of a
 * function. Never executed: move $4,$18.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029DFDC
.type func_0029DFDC, @object
func_0029DFDC:
    .word 0x02402021
.size func_0029DFDC, . - func_0029DFDC
