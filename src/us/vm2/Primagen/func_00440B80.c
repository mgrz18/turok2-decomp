#include "common.h"
#include "structs/game_object.h"
#include "globals/boss.h"
#include "protos/ai.h"



void Primagen_UpdateMode14_2(s32 arg0, CAI *arg1) {
    s32 var_a2;

    if (arg1->m_AnimRepeatedCount != 0) {
        var_a2 = 0x13;
        if (gLevelScript.unk3CA == 0) {
            var_a2 = 7;
        }
        CAI__SetMode(arg0, arg1, var_a2);
    }
}
