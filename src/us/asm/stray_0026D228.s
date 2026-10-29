/* 0x0026D228-0x0026D230: 2 words splat took for the start of a
 * function. Never executed: jal 20d908; addu $5,$16,$5.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0026D228
.type func_0026D228, @object
func_0026D228:
    .word 0x0C083642
    .word 0x02052821
.size func_0026D228, . - func_0026D228
