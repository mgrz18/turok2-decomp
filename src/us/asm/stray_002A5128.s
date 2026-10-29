/* 0x002A5128-0x002A5130: 2 words splat took for the start of a
 * function. Never executed: ahead of __floatdisf.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_002A5128
.type func_002A5128, @object
func_002A5128:
    .word 0x0C089097
    .word 0x00000000
.size func_002A5128, . - func_002A5128
