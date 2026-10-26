#include "common.h"
#include "gfx.h"

/* One two-word display list command at the list head. */
#define GFX_CMD(a, b) { Gfx *_g = D_800D8E24++; _g->w0 = (a); _g->w1 = (b); }

extern s32 D_800C2214;          /* display list capacity, in commands */
extern u8 *D_800F7070;          /* frame state; 0x114 = start of the frame's list */

s32 func_00208810(s32, s32);
s32 *func_002017D4(s32 *, s32);
void func_0020565C(s32, s32 **);
void func_00207FEC(s32);

void func_0020BBE4(s32 arg0, s32 renderMode, s32 **model, s32 matrix, s32 projected,
                   s32 lights, s32 *segment, s32 group, s32 arg8) {
    s32 *obj;
    s32 *list;
    s32 *entry;
    s32 *dl;
    s32 count;
    s32 i;
    s32 *seg;

    if ((u32) (D_800C2214 - ((u32) ((u8 *) D_800D8E24 - *(u8 **) (D_800F7070 + 0x114)) >> 3)) < 3000) {
        return;
    }
    if (func_00208810(arg0, arg8) == 0) {
        return;
    }
    if (projected) {
        GFX_CMD(0xDB060004, matrix);
    } else {
        GFX_CMD(0xDA380003, matrix);
    }
    if (lights) {
        GFX_CMD(0xDB020000, 0x18);
        GFX_CMD(0xDC08060A, lights + 8);
        GFX_CMD(0xDC08090A, lights);
    }
    if (renderMode != -1) {
        func_00207FEC(renderMode);
    }
    func_0020565C(0, model);
    obj = *model;
    seg = segment;
    if (seg == NULL) {
        seg = func_002017D4(obj, 0);
    }
    GFX_CMD(0xDB060008, (u32) seg);
    list = func_002017D4(func_002017D4(obj, 2), group);
    count = *list;
    for (i = 0; i < count; i++) {
        entry = func_002017D4(list, i);
        func_002017D4(entry, 0);
        dl = func_002017D4(entry, 1);
        GFX_CMD(0xDE000000, (u32) dl);
    }
}
