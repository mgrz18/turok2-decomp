#include "common.h"
#include "structs/game_object.h"
#include "protos/ai.h"


void Generator_SpawnUpdate(s32 arg0, CAI *ai) {
    if (ai->m_AnimRepeatedCount != 0) {
        CAI__SetMode(arg0, ai, 0);
    }
}
