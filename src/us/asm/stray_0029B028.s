/* 0x0029B028-0x0029B030: 2 words splat took for the start of a
 * function. Never executed: move $5,$16; jal 24599c.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029B028
.type func_0029B028, @object
func_0029B028:
    .word 0x02002821
    .word 0x0C091667
.size func_0029B028, . - func_0029B028
