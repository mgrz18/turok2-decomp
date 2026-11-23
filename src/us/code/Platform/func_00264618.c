#include "common.h"
#include "structs/game_object.h"

void Platform_AtEndEnter(CGameObjectInstance *arg0, s32 *arg1) {
    if (!(arg0->header.m_pIntelligence->m_Platform.m_dwPlatformFlags & 0x80)) {
        *arg1 &= 0xEFFFFFFF;
    }
}
