/* 0x00425754-0x00425760: 3 words splat took for the start of a
 * function. Never executed: lui $2,0x800b; lw $2,31724($2); addiu $3,$6,8.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00425754
.type func_00425754, @object
func_00425754:
    .word 0x3C02800B
    .word 0x8C427BEC
    .word 0x24C30008
.size func_00425754, . - func_00425754
