/* 0x0028B424-0x0028B430: 3 words splat took for the start of a
 * function. Never executed: move $5,$16; addiu $4,$18,12; jal 200518.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0028B424
.type func_0028B424, @object
func_0028B424:
    .word 0x02002821
    .word 0x2644000C
    .word 0x0C080146
.size func_0028B424, . - func_0028B424
