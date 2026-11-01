/* 0x00467B4C-0x00467B50: 1 words splat took for the start of a
 * function. Never executed: lw v1,0x20(sp): junk between objects.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00467B4C
.type func_00467B4C, @object
func_00467B4C:
    .word 0x8FA30020
.size func_00467B4C, . - func_00467B4C
