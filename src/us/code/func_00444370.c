#include "common.h"
#include "structs/game_object.h"
#include "protos/ai.h"


void MotherBoss_Mode08_Step_2(CGameObjectInstance *arg0, CAI *ai) {
    if (ai->m_AnimRepeatedCount != 0) {
        arg0->m_dwHotPointFlags = (s32) (arg0->m_dwHotPointFlags & 0xEFFFFFFF);
        CAI__SetMode((s32) arg0, (s32) ai, 2);
    }
}
