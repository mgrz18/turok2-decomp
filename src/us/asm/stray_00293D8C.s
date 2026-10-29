/* 0x00293D8C-0x00293D90: 1 words splat took for the start of a
 * function. Never executed: bne $10,$1,2a630c.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00293D8C
.type func_00293D8C, @object
func_00293D8C:
    .word 0x1541495F
.size func_00293D8C, . - func_00293D8C
