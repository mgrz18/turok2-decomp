/* The second VM module's copy of src/us/vm1/BlindOne/func_0040E30C.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "globals/boss.h"
#include "protos/ai.h"



void BlindOne_ActorDeathEnter_2(CGameObjectInstance *actor, CAI *ai) {
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
