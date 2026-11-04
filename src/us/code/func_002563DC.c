#include "common.h"

typedef struct {
    /* 0x000 */ s32 unk0;
    /* 0x004 */ s32 unk4;
    /* 0x008 */ s32 unk8;
    /* 0x00C */ u8 padC[0x37C - 0xC];
    /* 0x37C */ s32 unk37C;
} PlayerSubState;

typedef struct {
    /* 0x0000 */ u8 pad0[0xCD0];
    /* 0x0CD0 */ PlayerSubState sub;
    /* 0x1050 */ u8 pad1050[0x1284 - 0x1050];
    /* 0x1284 */ s32 unk1284;
} Player;

void func_002563DC(Player *arg0) {
    PlayerSubState *temp_v0;

    temp_v0 = &arg0->sub;
    arg0->sub.unk0 = 0;
    temp_v0->unk4 = 0;
    temp_v0->unk37C = -1;
    temp_v0->unk8 = 0;
    arg0->unk1284 = 0;
}
