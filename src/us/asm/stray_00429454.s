/* 0x00429454-0x00429460: 3 words splat took for the start of a
 * function. Never executed: lw $8,72($29); lw $9,76($29); sw $8,16($29).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00429454
.type func_00429454, @object
func_00429454:
    .word 0x8FA80048
    .word 0x8FA9004C
    .word 0xAFA80010
.size func_00429454, . - func_00429454
