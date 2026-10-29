/* 0x00201CB8-0x00201CC0: 2 words splat took for the start of a
 * function. Never executed: daddiu $15,$10,25194; daddiu $3,$11,29769.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00201CB8
.type func_00201CB8, @object
func_00201CB8:
    .word 0x654F626A
    .word 0x65637449
.size func_00201CB8, . - func_00201CB8
