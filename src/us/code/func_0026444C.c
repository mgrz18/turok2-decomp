#include "common.h"
#include "structs/game_object.h"

void func_0026444C(CGameObjectInstance *obj, s32 *arg1) {
    if (!(obj->header.m_pIntelligence->m_Platform.m_dwPlatformFlags & 0x40)) {
        *arg1 &= 0xF7FFFFFF;
    }
}
