/* 0x00430298-0x004302A0: 2 words splat took for the start of a
 * function. Never executed: lw $17,0($7); li $2,75.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00430298
.type func_00430298, @object
func_00430298:
    .word 0x8CF10000
    .word 0x2402004B
.size func_00430298, . - func_00430298
