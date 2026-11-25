/* Local types that vm1/Demo and its vm2 copy both define (one layout): one definition
 * for both copies (tools/struct_census.py). */
#ifndef STRUCTS_TWIN_DEMO_H
#define STRUCTS_TWIN_DEMO_H

#include "common.h"
#include "structs/camera.h"

typedef struct Floats {
    f32 unk0, unk4, unk8, unkC, unk10, unk14;
} Floats;
/* One per player, 0x224 bytes apart. */
typedef struct Slot {
    /* 0x00 */ s32 a;
    /* 0x04 */ u8 pad4[0x12];
    /* 0x16 */ u8 b;
    /* 0x17 */ u8 c;
    /* 0x18 */ u8 pad18[0x224 - 0x18];
} Slot;
typedef struct {
    /* 0x00 */ s32 result;
    /* 0x04 */ u8 pad4[0xC - 0x4];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 pad10[0x1C - 0x10];
    /* 0x1C */ s32 (*func)();
} Job;

#endif
