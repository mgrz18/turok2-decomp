/* 0x0029A398-0x0029A3A0: 2 words splat took for the start of a
 * function. Never executed: lwc1 $f1,64($17); lui $1,0x800a.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029A398
.type func_0029A398, @object
func_0029A398:
    .word 0xC6210040
    .word 0x3C01800A
.size func_0029A398, . - func_0029A398
