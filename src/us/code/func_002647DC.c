#include "common.h"
#include "m2c_macros.h"
#include "structs/game_object.h"

s32 CAI__SetMode(s32, s32, s32);

void Platform_EventForward(CGameObjectInstance *arg0, s32 *arg1) {
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x2100);
    *arg1 |= 0x08000000;
    CAI__SetMode((s32) arg0, (s32) arg1, 1);
}
