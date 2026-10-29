/* 0x0044584C-0x00445850: 1 words splat took for the start of a
 * function. Never executed: lw $2,28($16).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0044584C
.type func_0044584C, @object
func_0044584C:
    .word 0x8E02001C
.size func_0044584C, . - func_0044584C
