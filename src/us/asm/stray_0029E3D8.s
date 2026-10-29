/* 0x0029E3D8-0x0029E3E0: 2 words splat took for the start of a
 * function. Never executed: jal 243414; li $6,1.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0029E3D8
.type func_0029E3D8, @object
func_0029E3D8:
    .word 0x0C090D05
    .word 0x24060001
.size func_0029E3D8, . - func_0029E3D8
