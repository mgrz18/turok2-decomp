/* 0x00298494-0x002984A0: 3 words splat took for the start of a
 * function. Never executed: lw $31,172($29); lw $18,168($29); lw $17,164($29).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00298494
.type func_00298494, @object
func_00298494:
    .word 0x8FBF00AC
    .word 0x8FB200A8
    .word 0x8FB100A4
.size func_00298494, . - func_00298494
