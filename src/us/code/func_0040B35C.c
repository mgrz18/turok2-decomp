#include "common.h"
#include "m2c_macros.h"
#include "structs/game_object.h"

void MotherBoss_Mode08_Enter(CGameObjectInstance *arg0) {
    arg0->m_dwHotPointFlags = (s32) (arg0->m_dwHotPointFlags | 0x10000000);
}
