/* 0x0026F078-0x0026F080: 2 words splat took for the start of a
 * function. Never executed: addiu, jal func_0026D288; callers jal 0x26F080.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0026F078
.type func_0026F078, @object
func_0026F078:
    .word 0x26246A88
    .word 0x0C09B4A2
.size func_0026F078, . - func_0026F078
