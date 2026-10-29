/* 0x002985B8-0x002985C0: 2 words splat took for the start of a
 * function. Never executed: lw $6,28($29); lw $7,32($29).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_002985B8
.type func_002985B8, @object
func_002985B8:
    .word 0x8FA6001C
    .word 0x8FA70020
.size func_002985B8, . - func_002985B8
