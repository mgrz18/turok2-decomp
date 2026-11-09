#include "common.h"
#include "structs/game_object.h"
#include "protos/ai.h"


void MotherBoss_Mode20_Step_2(s32 arg0, CAI *arg1) {
    if (arg1->m_AnimRepeatedCount != 0) {
        CAI__SetMode(arg0, (s32) arg1, 0x15);
    }
}
