#include "common.h"
#include "structs/game_object.h"
#include "protos/ai.h"


void MotherBoss_Mode08_Step(CGameObjectInstance *arg0, CAI *arg1) {
    if (arg1->m_AnimRepeatedCount != 0) {
        arg0->m_dwHotPointFlags = (s32) (arg0->m_dwHotPointFlags & 0xEFFFFFFF);
        CAI__SetMode(arg0, arg1, 2);
    }
}
