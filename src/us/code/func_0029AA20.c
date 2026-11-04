#include "common.h"

/* *D_800B7AC4: flags at 0 (bit 4: the value at 0x24 changed), a float at 0x24. */
typedef struct {
    /* 0x00 */ u16 flags;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ u8 pad4[0x10 - 4];
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ u8 pad18[0x24 - 0x18];
    /* 0x24 */ f32 value;
} AudioState;

s32 __osDisableInt();                                /* extern */
s32 __osRestoreInt(s32);                         /* extern */
extern AudioState *D_800B7AC4;

void func_0029AA20(s32 arg0, s32 arg1, s16 arg2) {
    s32 temp_a0;

    temp_a0 = __osDisableInt();
    D_800B7AC4->unk10 = arg0;
    D_800B7AC4->unk14 = arg1;
    D_800B7AC4->unk2 = arg2;
    __osRestoreInt(temp_a0);
}
