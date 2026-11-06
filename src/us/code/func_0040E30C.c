#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

extern BlindOneState gBlindOne;

extern s32 CAI__SetMode(CGameObjectInstance *actor, void *ai, s32 state);

void BlindOne_ActorDeathEnter(CGameObjectInstance *actor, CAI *ai) {
    BlindOneState *counters = &gBlindOne;
    CGameObjectInstance *next;

    if (counters->boss->m_AI.m_Mode != 1) {
        CAI__SetMode(counters->boss, &counters->boss->m_AI, 8);
    }
    ai->m_BlindOne.m_nPhaseHits++;
    switch (actor->m_nTypeFlag) {
        case 0x454:
            counters->alive1--;
            break;
        case 0x455:
            counters->alive2--;
            break;
        case 0x456:
            counters->alive3--;
            next = counters->actors3[(ai->m_BlindOne.m_nGroupIndex + 1) % counters->count3];
            if (next->m_AI.m_BlindOne.m_nPhaseHits == 0) {
                CAI__SetMode(next, &next->m_AI, 0);
            }
            break;
    }
}
