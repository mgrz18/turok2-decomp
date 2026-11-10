#include "common.h"
#include "structs/game_object.h"
#include "globals/boss.h"
#include "protos/ai.h"



void Primagen_UpdateMode16(sptr arg0, CAI *ai) {
    if (ai->m_AnimRepeatedCount != 0) {
        gLevelScript.scriptCOn = 0;
        CAI__SetMode(arg0, ai, 2);
    }
}
