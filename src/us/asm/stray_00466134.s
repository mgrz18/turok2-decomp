/* 0x00466134-0x00466140: 3 words splat took for the start of a
 * function. Never executed: jal 2017d4; ori $5,$16,0x1; lw $3,0($17).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00466134
.type func_00466134, @object
func_00466134:
    .word 0x0C0805F5
    .word 0x36050001
    .word 0x8E230000
.size func_00466134, . - func_00466134
