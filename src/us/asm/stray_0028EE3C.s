/* 0x0028EE3C-0x0028EE40: 1 words splat took for the start of a
 * function. Never executed: jr $31.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0028EE3C
.type func_0028EE3C, @object
func_0028EE3C:
    .word 0x03E00008
.size func_0028EE3C, . - func_0028EE3C
