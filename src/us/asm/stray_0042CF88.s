/* 0x0042CF88-0x0042CF90: 2 words splat took for the start of a
 * function. Never executed: lw $2,64($4); sw $2,0($3).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0042CF88
.type func_0042CF88, @object
func_0042CF88:
    .word 0x8C820040
    .word 0xAC620000
.size func_0042CF88, . - func_0042CF88
