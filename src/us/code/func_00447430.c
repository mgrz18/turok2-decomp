#include "common.h"
#include "structs/game_object.h"

void BlindOne_ActorDeathUpdate_2(CGameObjectInstance *arg0, CAI *ai) {
    if (ai->m_AnimRepeatedCount != 0) {
        arg0->m_dwFlags = (s32) (arg0->m_dwFlags & ~0x100);
    }
}
