#include "common.h"
#include "structs/game_object.h"

s32 CAI__SetMode(CGameObjectInstance *obj, CAI *ai, s32 mode);

extern s8 gLevelScript_scriptCOn;

void Primagen_UpdateMode16_2(CGameObjectInstance *obj, CAI *ai) {
    if (ai->m_AnimRepeatedCount != 0) {
        gLevelScript_scriptCOn = 0;
        CAI__SetMode(obj, ai, 2);
    }
}
