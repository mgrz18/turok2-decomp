/* 0x00429124-0x00429130: 3 words splat took for the start of a
 * function. Never executed: beqz $19,4291e8; mov.s $f0,$f21; lw $8,4($16).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00429124
.type func_00429124, @object
func_00429124:
    .word 0x12600030
    .word 0x4600A806
    .word 0x8E080004
.size func_00429124, . - func_00429124
