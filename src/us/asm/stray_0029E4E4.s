/* 0x0029E4E4-0x0029E4F0: 3 words splat took for the start of a
 * function. Never executed: beqz $2,29e534; move $4,$16; move $5,$16.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029E4E4
.type func_0029E4E4, @object
func_0029E4E4:
    .word 0x10400013
    .word 0x02002021
    .word 0x02002821
.size func_0029E4E4, . - func_0029E4E4
