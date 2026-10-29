/* 0x002A12EC-0x002A12F0: 1 words splat took for the start of a
 * function. Never executed: jr $2.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_002A12EC
.type func_002A12EC, @object
func_002A12EC:
    .word 0x00400008
.size func_002A12EC, . - func_002A12EC
