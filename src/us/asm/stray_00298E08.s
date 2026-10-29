/* 0x00298E08-0x00298E10: 2 words splat took for the start of a
 * function. Never executed: lw $2,20($4); lh $2,116($2).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00298E08
.type func_00298E08, @object
func_00298E08:
    .word 0x8C820014
    .word 0x84420074
.size func_00298E08, . - func_00298E08
