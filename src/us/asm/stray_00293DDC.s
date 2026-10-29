/* 0x00293DDC-0x00293DE0: 1 words splat took for the start of a
 * function. Never executed: sdc1 $f20,112($29).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00293DDC
.type func_00293DDC, @object
func_00293DDC:
    .word 0xF7B40070
.size func_00293DDC, . - func_00293DDC
