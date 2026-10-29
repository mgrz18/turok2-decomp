/* 0x00407F14-0x00407F20: 3 words splat took for the start of a
 * function. Never executed: cvt.s.w $f7,$f7; sll $3,$17,0x1; addu $3,$3,$17.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00407F14
.type func_00407F14, @object
func_00407F14:
    .word 0x468039E0
    .word 0x00111840
    .word 0x00711821
.size func_00407F14, . - func_00407F14
