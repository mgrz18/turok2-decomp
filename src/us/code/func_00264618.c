#include "common.h"
#include "structs/game_object.h"

void func_00264618(CGameObjectInstance *arg0, s32 *arg1) {
    if (!(arg0->header.m_pIntelligence->m_Enemy.m_dwFlags2 & 0x80)) {
        *arg1 &= 0xEFFFFFFF;
    }
}
