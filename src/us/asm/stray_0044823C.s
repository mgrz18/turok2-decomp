/* 0x0044823C-0x00448240: 1 words splat took for the start of a
 * function. Never executed: lw $2,2660($4).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0044823C
.type func_0044823C, @object
func_0044823C:
    .word 0x8C820A64
.size func_0044823C, . - func_0044823C
