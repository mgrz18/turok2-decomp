#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

typedef struct {
    /* 0x000 */ u8 pad0[0x10C];
    /* 0x10C */ s32 hits;
    /* 0x110 */ s32 index;
} Spawn;

extern BlindOneState D_80130200;

extern s32 func_00243414(CGameObjectInstance *actor, void *ai, s32 state);

void func_0040E30C(CGameObjectInstance *actor, Spawn *spawn) {
    BlindOneState *counters = &D_80130200;
    CGameObjectInstance *next;

    if (counters->boss->m_AI.m_Mode != 1) {
        func_00243414(counters->boss, &counters->boss->m_AI, 8);
    }
    spawn->hits++;
    switch (actor->m_nTypeFlag) {
        case 0x454:
            counters->alive1--;
            break;
        case 0x455:
            counters->alive2--;
            break;
        case 0x456:
            counters->alive3--;
            next = counters->actors3[(spawn->index + 1) % counters->count3];
            if (next->m_AI.m_VarBuffer[0] == 0) {
                func_00243414(next, &next->m_AI, 0);
            }
            break;
    }
}
