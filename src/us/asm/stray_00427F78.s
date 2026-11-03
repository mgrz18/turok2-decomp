/* 0x00427F78-0x00427F80: 2 words splat took for the start of a
 * function. Never executed: addiu sp,96; addiu sp,-32: junk before the 16-aligned func_00427F80.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00427F78
.type func_00427F78, @object
func_00427F78:
    .word 0x27BD0060
    .word 0x27BDFFE0
.size func_00427F78, . - func_00427F78
