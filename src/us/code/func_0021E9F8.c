#include "common.h"
#include "structs/game_object.h"

s32 CGameObjectInstance__RequestAnim();                            /* extern */

s32 CGameObjectInstance__SetAnim(CGameObjectInstance *arg0, s32 arg1, s32 arg2) {
    if ((arg1 < 0) || ((arg2 == 0) && (arg0->m_dwFlags & 0x400))) {
        return 0;
    }
    CGameObjectInstance__RequestAnim();
    return 1;
}
