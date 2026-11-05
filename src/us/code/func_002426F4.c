#include "common.h"
#include "structs/game_object.h"

f32 func_002982F0(f32 x);
f32 func_002974C0(f32 x);
s32 func_0023AB18(void *arg0, void *arg1, void *arg2, s32 arg3, s32 arg4, f32 arg5, s32 arg6);

s32 func_002426F4(void *arg0, CGameObjectInstance *mover, CGameObjectInstance *collider, char *arg3) {
    f32 scale;

    scale = func_002982F0(mover->m_AI.m_Platform.m_Pitch);
    scale *= func_002974C0(mover->m_AI.m_Platform.m_Pitch);
    if (func_0023AB18(arg0, arg3 + 0x30, arg3, 0, 2, scale, 0) != 0) {
        mover->m_AI.m_dwFlags |= 0x10000;
        if (collider->header.m_Type == 1 && (collider->m_dwFlags & 0x300000)) {
            mover->m_AI.m_dwFlags |= 0x20000;
        }
        collider->m_pInstanceBelow = mover;
        if (collider->m_vVelocity.y < 0.0f) {
            collider->m_vVelocity.y = 0.0f;
        }
        return 1;
    }
    return 0;
}
