#include "common.h"
#include "structs/game_object.h"
#include "structs/vector.h"
#include "structs/boss.h"

typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ u8 padC[0x24 - 0xC];
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ u8 pad2C[0x9C - 0x2C];
    /* 0x9C */ CVector3 pos;
    /* 0xA8 */ s32 unkA8;
} Spawn;

extern BlindOneState D_80130200;

void func_0044700C(CGameObjectInstance *actor, Spawn *spawn) {
    BlindOneState *counters;

    spawn->unk28 = 0;
    spawn->unk24 = 0;
    spawn->unk4 = spawn->unk8;
    actor->header.m_vPos = spawn->pos;
    actor->header.m_pCurrentRegion = spawn->unkA8;
    counters = &D_80130200;
    switch (actor->m_nTypeFlag) {
        case 0x454:
            counters->alive1++;
            break;
        case 0x455:
            counters->alive2++;
            break;
        case 0x456:
            counters->alive3++;
            break;
    }
}
