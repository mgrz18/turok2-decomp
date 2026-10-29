/* 0x0026060C-0x00260610: 1 words splat took for the start of a
 * function. Never executed: sw $2,13828($17).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0026060C
.type func_0026060C, @object
func_0026060C:
    .word 0xAE223604
.size func_0026060C, . - func_0026060C
