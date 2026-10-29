/* 0x0029B814-0x0029B820: 3 words splat took for the start of a
 * function. Never executed: beqz into osSendMesg+4, nop, jal.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029B814
.type func_0029B814, @object
func_0029B814:
    .word 0x10400003
    .word 0x00000000
    .word 0x0C090D05
.size func_0029B814, . - func_0029B814
