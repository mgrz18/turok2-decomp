/* 0x00274B34-0x00274B40: 3 words splat took for the start of a
 * function. Never executed: beqz into func_00274B40.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00274B34
.type func_00274B34, @object
func_00274B34:
    .word 0x02602021
    .word 0x104000AE
    .word 0x00001021
.size func_00274B34, . - func_00274B34
