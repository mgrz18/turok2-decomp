/* 0x0029BB0C-0x0029BB10: 1 words splat took for the start of a
 * function. Never executed: jal 21034c.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029BB0C
.type func_0029BB0C, @object
func_0029BB0C:
    .word 0x0C0840D3
.size func_0029BB0C, . - func_0029BB0C
