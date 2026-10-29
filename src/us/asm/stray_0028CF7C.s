/* 0x0028CF7C-0x0028CF80: 1 words splat took for the start of a
 * function. Never executed: sw $0,4748($16).
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0028CF7C
.type func_0028CF7C, @object
func_0028CF7C:
    .word 0xAE00128C
.size func_0028CF7C, . - func_0028CF7C
