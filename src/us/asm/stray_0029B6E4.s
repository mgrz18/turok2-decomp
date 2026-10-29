/* 0x0029B6E4-0x0029B6F0: 3 words splat took for the start of a
 * function. Never executed: sw, addiu, sw ahead of osRecvMesg.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029B6E4
.type func_0029B6E4, @object
func_0029B6E4:
    .word 0xACA40108
    .word 0x27BDFFE0
    .word 0xAFB00010
.size func_0029B6E4, . - func_0029B6E4
