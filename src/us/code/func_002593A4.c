#include "common.h"

typedef struct {
    /* 0x000 */ u8 pad0[0x24C];
    /* 0x24C */ s32 unk24C;
} Owner;

typedef struct {
    /* 0x00 */ Owner *owner;
    /* 0x04 */ u8 pad4[0x90 - 0x4];
    /* 0x90 */ s32 state;
} Arg1;

f32 func_00210894(s32, s32);

void func_002593A4(s32 arg0, Arg1 *arg1) {
    if (arg1->state == 2) {
        func_00210894(arg0, arg1->owner->unk24C);
    }
}
