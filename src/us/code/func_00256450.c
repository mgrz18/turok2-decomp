#include "common.h"
#include "structs/player.h"

/* The player's weapon wheel (CPlayer.m_Barrel, CBarrel; see func_00255A44.c). */
typedef struct {
    /* 0x000 */ s32 unk0;
    /* 0x004 */ s32 unk4;
    /* 0x008 */ s32 unk8;
    /* 0x00C */ u8 padC[0x37C - 0xC];
    /* 0x37C */ s32 unk37C;
} Barrel;

void func_00256450(CPlayer *arg0) {
    Barrel *temp_v0;

    temp_v0 = (Barrel *) arg0->m_Barrel;
    ((Barrel *) arg0->m_Barrel)->unk0 = 0;
    temp_v0->unk4 = 0;
    temp_v0->unk37C = -1;
    temp_v0->unk8 = 0;
    arg0->m_bHeadMovementDisabled = 0;
}
