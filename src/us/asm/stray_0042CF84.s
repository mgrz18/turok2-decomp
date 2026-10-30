/* 0x0042CF84-0x0042CF90: 3 words splat took for the start of a
 * function. Never executed: a padding nop, then lw $2,64($4); sw $2,0($3).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0042CF84
.type func_0042CF84, @object
func_0042CF84:
    .word 0x00000000
    .word 0x8C820040
    .word 0xAC620000
.size func_0042CF84, . - func_0042CF84
