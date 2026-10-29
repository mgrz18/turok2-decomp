/* 0x00275F94-0x00275FA0: 3 words splat took for the start of a
 * function. Never executed: div.s $f21,$f23,$f0; mul.s $f5,$f5,$f21; lwc1 $f4,72($29).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00275F94
.type func_00275F94, @object
func_00275F94:
    .word 0x4600BD43
    .word 0x46152942
    .word 0xC7A40048
.size func_00275F94, . - func_00275F94
