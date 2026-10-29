/* 0x0029E0C8-0x0029E0D0: 2 words splat took for the start of a
 * function. Never executed: lw $5,4($16); lw $6,8($16).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029E0C8
.type func_0029E0C8, @object
func_0029E0C8:
    .word 0x8E050004
    .word 0x8E060008
.size func_0029E0C8, . - func_0029E0C8
