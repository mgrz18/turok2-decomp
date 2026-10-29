/* 0x002C23D8-0x002C23E0: 2 words splat took for the start of a
 * function. Never executed: lui $3,0x800c; lw $3,8240($3).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_002C23D8
.type func_002C23D8, @object
func_002C23D8:
    .word 0x3C03800C
    .word 0x8C632030
.size func_002C23D8, . - func_002C23D8
