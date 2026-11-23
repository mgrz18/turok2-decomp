#include "common.h"
#include "m2c_macros.h"
#include "structs/game_object.h"

void Platform_EventShow(CGameObjectInstance *arg0) {
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x2100);
}
