#include "common.h"

s32 __osDisableInt();                                /* extern */
s32 __osRestoreInt(s32);                         /* extern */
typedef struct {
    /* 0x0 */ u16 flags;
    /* 0x2 */ u8 pad2[2];
    /* 0x4 */ s32 value;
} Unk800B7AC4;
extern Unk800B7AC4 *D_800B7AC4;

void func_0029AC90(s32 arg0) {
    s32 temp_v0;

    temp_v0 = __osDisableInt();
    D_800B7AC4->value = arg0;
    D_800B7AC4->flags = (u16) (D_800B7AC4->flags | 0x10);
    __osRestoreInt(temp_v0);
}
