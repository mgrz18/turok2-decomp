/* 0x00408FF8-0x00409000: 2 words splat took for the start of a
 * function. Never executed: two junk words; func_00409340 installs func_00408FF8 + 8 = func_00409000.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00408FF8
.type func_00408FF8, @object
func_00408FF8:
    .word 0x8C430000
    .word 0x8C450004
.size func_00408FF8, . - func_00408FF8
