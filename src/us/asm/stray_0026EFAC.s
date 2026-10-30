/* 0x0026EFAC-0x0026EFB0: 1 words splat took for the start of a
 * function. Never executed: move $v1,$v0 after func_0026EF30's object; an empty function starts at 0x26EFB0.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0026EFAC
.type func_0026EFAC, @object
func_0026EFAC:
    .word 0x00401821
.size func_0026EFAC, . - func_0026EFAC
