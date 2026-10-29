/* 0x0029AC84-0x0029AC90: 3 words splat took for the start of a
 * function. Never executed: swc1 $f1,280($16); lwc1 $f0,52($17); swc1 $f20,296($16).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029AC84
.type func_0029AC84, @object
func_0029AC84:
    .word 0xE6010118
    .word 0xC6200034
    .word 0xE6140128
.size func_0029AC84, . - func_0029AC84
