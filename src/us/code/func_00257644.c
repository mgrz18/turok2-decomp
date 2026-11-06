#include "common.h"
#include "structs/game_object.h"

s32 CPlayer__SetState(s32, void *, s32);        /* extern */
s32 GameObject_IsOnGround(void *);                          /* extern */

s32 CPlayer__CheckLanded(s32 arg0, CGameObjectInstance *arg1) {
    if ((arg1->m_vVelocity.y <= 0.0f) && (GameObject_IsOnGround(arg1) != 0)) {
        CPlayer__SetState(arg0, arg1, 2);
        return 1;
    }
    return 0;
}
