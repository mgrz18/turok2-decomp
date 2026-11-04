#include "common.h"

s32 func_00210790(s32, s32);                    /* extern */
s32 func_00210B38(s32, s32);                    /* extern */

typedef struct {
    /* 0x000 */ u8 pad0[0x164];
    /* 0x164 */ s32 unk164;
    /* 0x168 */ s32 unk168;
} Owner0025E4B8;

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s8 min;
    /* 0x19 */ s8 max;
} Range0025E4B8;

typedef struct {
    /* 0x00 */ Owner0025E4B8 *owner;
    /* 0x04 */ u8 pad4[0x14 - 0x4];
    /* 0x14 */ Range0025E4B8 *range;
    /* 0x18 */ u8 pad18[0x90 - 0x18];
    /* 0x90 */ s32 value;
} Effect0025E4B8;

void func_0025E4B8(s32 arg0, Effect0025E4B8 *arg1) {
    s32 temp_v1;
    Range0025E4B8 *temp_a0;

    temp_a0 = arg1->range;
    temp_v1 = arg1->value;
    if ((temp_v1 >= temp_a0->min) && (temp_a0->max >= temp_v1)) {
        func_00210B38(arg0, arg1->owner->unk164);
        func_00210790(arg0, arg1->owner->unk168);
    }
}
