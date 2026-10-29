/* 0x00460094-0x004600A0: 3 words splat took for the start of a
 * function. Never executed: jal 20ef2c; move $6,$20; move $4,$17.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00460094
.type func_00460094, @object
func_00460094:
    .word 0x0C083BCB
    .word 0x02803021
    .word 0x02202021
.size func_00460094, . - func_00460094
