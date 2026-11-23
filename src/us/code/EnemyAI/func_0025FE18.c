#include "common.h"
#include "structs/game_object.h"
#include "structs/player.h"
#include "protos/ai.h"


void EnemyMode_PlayerRide_Update(CGameObjectInstance *self, CAI *ai) {
    CPlayer *player = (CPlayer *)ai->m_Enemy.m_pGrabbed->m_AI.m_pParent;

    self->m_dwFlags &= ~0x20000;
    self->m_dwFlags &= ~0x10000;
    ai->m_pfDraw = 0;
    if (ai->m_Mode != 0x2E || ai->m_AnimRepeatedCount != 0) {
        if (player->m_Mode == 0x21) {
            CAI__SetMode(self, ai, 0x2C);
        } else if (player->m_Mode == 0x22) {
            CAI__SetMode(self, ai, 0x2D);
        }
    }
}
