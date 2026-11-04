#include "common.h"
#include "structs/player.h"

typedef struct {
    /* 0x000 */ s32 unk0;
    /* 0x004 */ s32 unk4;
    /* 0x008 */ s32 unk8;
    /* 0x00C */ u8 padC[0x37C - 0xC];
    /* 0x37C */ s32 unk37C;
} PlayerSubState;

void func_002563DC(CPlayer *arg0) {
    PlayerSubState *temp_v0;

    temp_v0 = &arg0->m_Barrel;
    arg0->m_Barrel.m_nItems = 0;
    temp_v0->unk4 = 0;
    temp_v0->unk37C = -1;
    temp_v0->unk8 = 0;
    arg0->m_bHeadMovementDisabled = 0;
}
