/* 0x002A2FA8-0x002A2FB0: 2 words splat took for the start of a
 * function. Never executed: sw $31,84($29); sdc1 $f20,88($29).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_002A2FA8
.type func_002A2FA8, @object
func_002A2FA8:
    .word 0xAFBF0054
    .word 0xF7B40058
.size func_002A2FA8, . - func_002A2FA8
