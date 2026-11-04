#include "common.h"

typedef struct {
    /* 0x0000 */ u8 pad0[0x2B4C];
    /* 0x2B4C */ s32 unk2B4C;
    /* 0x2B50 */ s32 table;
    /* 0x2B54 */ u8 pad2B54[0x2BA0 - 0x2B54];
    /* 0x2BA0 */ s32 unk2BA0;
    /* 0x2BA4 */ u8 pad2BA4[0x2BB0 - 0x2BA4];
    /* 0x2BB0 */ s32 unk2BB0;
} Unk2B4C;

void func_002718EC(Unk2B4C *arg0, s32 arg1) {
    arg0->unk2BB0 = arg1;
}
