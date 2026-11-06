#include "common.h"
#include "structs/game_object.h"

s32 CScene__GetInstanceSavedBit(s32, s32);
s32 CAI__SetMode(s32, s32, s32);

extern s32 gScene;

void Platform_Advance(CGameObjectInstance *arg0, CAI *arg1) {
    arg1->m_Action = 0;
    CAI__SetMode(arg0, (s32) arg1, 0);
    if (CScene__GetInstanceSavedBit((s32) &gScene, arg0) == 0) {
        arg0->m_dwFlags = (s32) (arg0->m_dwFlags & ~0x100);
    }
}
