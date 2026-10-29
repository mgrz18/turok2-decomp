/* 0x002A40CC-0x002A40D0: 1 words splat took for the start of a
 * function. Never executed: ahead of __moddi3.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_002A40CC
.type func_002A40CC, @object
func_002A40CC:
    .word 0xAFA90028
.size func_002A40CC, . - func_002A40CC
