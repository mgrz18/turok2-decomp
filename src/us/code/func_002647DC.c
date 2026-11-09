#include "common.h"
#include "m2c_macros.h"
#include "structs/game_object.h"
#include "protos/ai.h"


void Platform_EventForward(CGameObjectInstance *arg0, s32 *arg1) {
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x2100);
    *arg1 |= 0x08000000;
    CAI__SetMode(arg0, arg1, 1);
}
