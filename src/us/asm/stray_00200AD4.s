/* 0x00200AD4-0x00200AE0: 3 words splat took for the start of a
 * function. Never executed: "Color" and two words; func_00224D54 passes 0x200AE0 as a load callback.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_00200AD4
.type func_00200AD4, @object
func_00200AD4:
    .word 0x436F6C6F
    .word 0x72190000
    .word 0x00960800
.size func_00200AD4, . - func_00200AD4
