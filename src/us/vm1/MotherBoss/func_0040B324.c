#include "common.h"
#include "structs/game_object.h"
#include "protos/ai.h"


void MotherBoss_Mode07_Step(CGameObjectInstance *arg0, CAI *arg1) {
    if (arg1->m_AnimRepeatedCount != 0) {
        arg0->m_dwHotPointFlags = (s32) (arg0->m_dwHotPointFlags & 0xF7FFFFFF);
        CAI__SetMode(arg0, arg1, 2);
    }
}
