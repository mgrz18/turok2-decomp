/* 0x00298664-0x00298670: 3 words splat took for the start of a
 * function. Never executed: swc1 $f0,308($16); swc1 $f0,312($16); lw $31,20($29).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00298664
.type func_00298664, @object
func_00298664:
    .word 0xE6000134
    .word 0xE6000138
    .word 0x8FBF0014
.size func_00298664, . - func_00298664
