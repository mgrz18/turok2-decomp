/* 0x00276814-0x00276820: 3 words splat took for the start of a
 * function. Never executed: bnez $3,27682c; move $6,$2; lw $2,0($6).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00276814
.type func_00276814, @object
func_00276814:
    .word 0x14600005
    .word 0x00403021
    .word 0x8CC20000
.size func_00276814, . - func_00276814
