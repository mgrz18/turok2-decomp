#include "common.h"

s32 func_0020185C(s32 *table, s32 base, s32 index, s32 *size);
s32 func_0020367C(s32 heap, s32 src, s32 size, s32 type, s32 owner, s32 callback, s32 name, s32 arg7);
s32 *func_002017D4(s32 *file, s32 index); /* returns chunk `index` of a chunked file */
s32 func_00219968(void *anim);
s32 *func_0021793C(void *anim);

extern s32 func_00216A9C;
extern u8 D_800A5A10[]; /* resource name */

typedef struct {
    /* 0x00 */ u8 pad00[4];
    /* 0x04 */ s16 index;
    /* 0x06 */ u8 pad06[2];
    /* 0x08 */ u16 count;
    /* 0x0A */ u8 pad0A[2];
    /* 0x0C */ s32 frames;
    /* 0x10 */ s32 data;
} Func00219ACCAnim;

/* Loads entry `index` of a resource table into anim; returns 1 on success. */
s32 func_00219ACC(Func00219ACCAnim *anim, s32 *table, s32 base, s32 index) {
    s32 size;
    s32 *file;

    if (index == -1) {
        anim->data = 0;
        return 0;
    }
    anim->data = func_0020367C(0, func_0020185C(table, base, index, &size), size, 4, (s32) anim,
                               (s32) &func_00216A9C, (s32) D_800A5A10, 0);
    if (!func_00219968(anim)) {
        return 0;
    }
    file = func_0021793C(anim);
    anim->count = ((u16 *) func_002017D4(file, 0))[3];
    anim->index = index;
    anim->frames = ((u16 *) func_002017D4(func_002017D4(file, 3), 0))[1];
    return 1;
}
