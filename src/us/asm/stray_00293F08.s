/* 0x00293F08-0x00293F10: 2 words splat took for the start of a
 * function. Never executed: add.s $f1,$f1,$f0; swc1 $f1,52($29).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00293F08
.type func_00293F08, @object
func_00293F08:
    .word 0x46000840
    .word 0xE7A10034
.size func_00293F08, . - func_00293F08
