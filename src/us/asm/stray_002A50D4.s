/* 0x002A50D4-0x002A50E0: 3 words splat took for the start of a
 * function. Never executed: ahead of __cmpdi2.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_002A50D4
.type func_002A50D4, @object
func_002A50D4:
    .word 0x8FB00030
    .word 0x03E00008
    .word 0x27BD0040
.size func_002A50D4, . - func_002A50D4
