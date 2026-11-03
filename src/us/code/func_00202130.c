/* WRONG CUT: splat named this D_00202128 (0x202128, 200 bytes): its first 8 bytes are junk fill
 * ("Style" text), the real function starts at the 16-aligned 0x202130 (184 bytes; func_00286674 calls it),
 * followed by 8 bytes of fill (a nop and a junk word) before func_002021F0.
 * Heap set-up at boot: clear everything from the end of the image (D_8014A000) to the top of RDRAM
 * (minus 512 KB when the expansion pak is present), hand it to the heap, then start the subsystems. */

#include "common.h"

s32 func_00266B80(void);                                  /* osGetMemSize */
void *func_00266C5C(void *dst, s32 value, u32 size);      /* memset */
void func_00202474(void *heap, void *base, u32 size);
void func_00201AD0(void *p, s32 n);
void func_00201CC0(void *p, s32 n);
void func_0028D820(s32 a, s32 b);
void func_00202BB0(s32 a, s32 b);

extern u8 D_8014A000[];   /* end of the loaded image */
extern u8 D_800D1CC0[];   /* the main heap */
extern u8 D_800D6D68[];
extern u8 D_800D1CD8[];

void func_00202130(void) {
    u8 *base;
    u32 top;
    u32 size;

    top = 0x80400000;
    base = D_8014A000;
    if (func_00266B80() != 0x400000) {
        top = (func_00266B80() | 0x80000000) - 0x80000;
    }
    size = top - (u32) base;
    func_00266C5C(base, 0, size);
    func_00202474(D_800D1CC0, base, size);
    func_00201AD0(D_800D6D68, 3);
    func_00201CC0(D_800D1CD8, 4);
    func_0028D820(0, 0);
    func_00202BB0(0, 5);
}
