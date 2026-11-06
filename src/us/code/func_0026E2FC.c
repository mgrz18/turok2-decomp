#include "common.h"
#include "m2c_macros.h"
#include "structs/game_object.h"

void Event_Hide(CGameObjectInstance *arg0) {
    arg0->m_dwFlags &= ~0x2000;
    arg0->m_dwFlags &= ~0x100;
}
