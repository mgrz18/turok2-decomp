#include "common.h"
#include "structs/game_object.h"

s32 func_0024E5F0(s32, void *, s32);        /* extern */
s32 func_002685F0(void *);                          /* extern */

s32 func_00257644(s32 arg0, CGameObjectInstance *arg1) {
    if ((arg1->m_vVelocity.y <= 0.0f) && (func_002685F0(arg1) != 0)) {
        func_0024E5F0(arg0, arg1, 2);
        return 1;
    }
    return 0;
}
