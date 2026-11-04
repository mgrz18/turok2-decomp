#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x44];
    /* 0x44 */ s32 flags;
    /* 0x48 */ u8 pad48[0x58 - 0x48];
    /* 0x58 */ u8 unk58;
    /* 0x59 */ u8 pad59[0x5B - 0x59];
    /* 0x5B */ u8 unk5B;
    /* 0x5C */ s8 unk5C;
} Unk0022425C;

void *func_0022425C(s32 *, s32);                /* extern */
extern s32 D_800F7078;

void func_0026C724(s32 arg0) {
    Unk0022425C *temp_v0;

    temp_v0 = func_0022425C(&D_800F7078, arg0);
    if (temp_v0 != NULL) {
        temp_v0->flags = temp_v0->flags | 0x80000;
    }
}
