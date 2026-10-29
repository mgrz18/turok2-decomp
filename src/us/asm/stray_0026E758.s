/* 0x0026E758-0x0026E760: 2 words splat took for the start of a
 * function. Never executed: sw, j .L0023587C; func_002851C0 calls 0x26E760.
 * Written by tools/fix_cut.py from the ROM; edit versions/cuts.us.txt. */

.set noreorder
.section .text, "ax"

.globl func_0026E758
.type func_0026E758, @object
func_0026E758:
    .word 0xAE020010
    .word 0x0808D61F
.size func_0026E758, . - func_0026E758
