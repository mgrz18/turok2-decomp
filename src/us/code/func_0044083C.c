#include "common.h"
#include "structs/game_object.h"

/* Stops the object. */
void func_0044083C(CGameObjectInstance *arg0) {
    arg0->m_vVelocity.x = 0;
    arg0->m_vVelocity.y = 0;
    arg0->m_vVelocity.z = 0;
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x2000);
}
