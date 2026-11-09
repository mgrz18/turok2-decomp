#include "common.h"
#include "structs/game_object.h"
#include "protos/ai.h"


void BlindOne_BossFlinchUpdate_2(s32 arg0, CAI *arg1) {
    if (arg1->m_AnimRepeatedCount >= 7) {
        CAI__SetMode(arg0, (s32) arg1, 7);
    }
}
