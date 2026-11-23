#include "common.h"
#include "structs/game_object.h"
#include "structs/vector.h"
#include "structs/boss.h"
#include "globals/boss.h"




void BlindOne_ActorAppearEnter(CGameObjectInstance *actor, CAI *spawn) {
    BlindOneState *stats;

    spawn->m_HeadTilt = 0;
    spawn->m_HeadTurn = 0;
    spawn->m_Health = spawn->m_InitialHealth;
    actor->header.m_vPos = spawn->m_vInitialPos;
    actor->header.m_pCurrentRegion = spawn->m_pInitialRegion;
    stats = &gBlindOne;
    switch (actor->m_nTypeFlag) {
        case 0x454:
            stats->alive1++;
            break;
        case 0x455:
            stats->alive2++;
            break;
        case 0x456:
            stats->alive3++;
            break;
    }
}
