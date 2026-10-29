/* 0x002A305C-0x002A3060: 1 words splat took for the start of a
 * function. Never executed: lui $1,0x800a.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_002A305C
.type func_002A305C, @object
func_002A305C:
    .word 0x3C01800A
.size func_002A305C, . - func_002A305C
