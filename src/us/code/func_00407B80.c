#include "common.h"
#include "structs/game_object.h"
#include "globals/boss.h"

s32 CAI__SetMode(CGameObjectInstance *obj, CAI *ai, s32 mode);


void Primagen_UpdateMode14(CGameObjectInstance *obj, CAI *ai) {
    s32 var_a2;

    if (ai->m_AnimRepeatedCount != 0) {
        var_a2 = 0x13;
        if (gLevelScript.unk3CA == 0) {
            var_a2 = 7;
        }
        CAI__SetMode(obj, ai, var_a2);
    }
}
