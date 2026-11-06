#include "common.h"
#include "structs/game_object.h"

s32 CAI__SetMode(s32, s32, s32);

void MotherBoss_Mode08_Step(CGameObjectInstance *arg0, CAI *arg1) {
    if (arg1->m_AnimRepeatedCount != 0) {
        arg0->m_dwHotPointFlags = (s32) (arg0->m_dwHotPointFlags & 0xEFFFFFFF);
        CAI__SetMode((s32) arg0, (s32) arg1, 2);
    }
}
