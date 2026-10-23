#include "common.h"

s32 *func_0020367C(s32 heap, s32 src, s32 size, s32 type, s32 arg4, s32 callback, s32 name, s32 arg7);
s32 func_0020185C(s32 *table, s32 base, s32 index, s32 *size);
void func_002051F4(s32 heap, s32 *block);

extern s32 func_0020B4B0;
extern u8 D_800A5858[]; /* +4: "simp object index" */
extern u8 D_800A5870[]; /* "simp geometry" */

/* Loads a simple object geometry: reads its index, then the geometry block it points at. */
s32 *func_00215944(void *obj) {
    s32 *index;
    s32 *geometry;
    s32 src;
    s32 size;

    index = func_0020367C(0, *(s32 *) ((u8 *) obj + 0x34), 0x18, 4, 0, 0, (s32) (D_800A5858 + 4), 0);
    if (index == NULL) {
        geometry = NULL;
    } else {
        src = func_0020185C((s32 *) *index, *(s32 *) ((u8 *) obj + 0x34), 1, &size);
        func_002051F4(0, index);
        geometry = func_0020367C(0, src, size, 4, 0, (s32) &func_0020B4B0, (s32) D_800A5870, 0);
    }
    return geometry;
}
