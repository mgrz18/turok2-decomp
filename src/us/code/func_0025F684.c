#include "common.h"
#include "structs/game_object.h"
#include "protos/ai.h"


void EnemyMode_CloakActivate_Update(s32 arg0, CAI *arg1) {
    if (arg1->m_AnimRepeatedCount >= 4) {
        CAI__SetMode(arg0, arg1, 0xA);
    }
}
