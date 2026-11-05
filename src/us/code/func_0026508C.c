#include "common.h"
#include "structs/vector.h"
#include "structs/game_object.h"

extern f32 func_0024665C(void *actor, CVector3 point); /* squared XZ distance */
extern f32 func_002672C8(void *actor);              /* collision radius */
extern void func_0025E488(void *actor, void *ai, s32 arg2);
extern void func_00243414(void *obj, void *state, s32 mode);
extern f32 func_00298470(f32 x); /* sqrtf */

extern s8 D_800F1F94[];

typedef struct {
    /* 0x000 */ u8 pad0[0x10C];
    /* 0x10C */ CGameObjectInstance *owner;
    /* 0x110 */ f32 tiltAngle;
} OwnedState;

void func_0026508C(CGameObjectInstance *obj, OwnedState *state) {
    CGameObjectInstance *owner = state->owner;
    f32 distSq;

    if (owner != NULL) {
        D_800F1F94[0] = 0;
        distSq = func_0024665C(obj, owner->header.m_vPos);
        if ((func_002672C8(obj) + func_002672C8(owner)) * (func_002672C8(obj) + func_002672C8(owner)) < distSq) {
            state->owner = NULL;
            func_0025E488(owner, (u8 *) owner + 0x140, 0);
        }
    } else {
        D_800F1F94[0] = 2;
    }
    if (obj->m_vVelocity.x == 0.0f && obj->m_vVelocity.y == 0.0f && obj->m_vVelocity.z == 0.0f) {
        func_00243414(obj, state, 0);
    }
    state->tiltAngle += func_00298470(obj->m_vVelocity.x * obj->m_vVelocity.x + obj->m_vVelocity.y * obj->m_vVelocity.y + obj->m_vVelocity.z * obj->m_vVelocity.z)
                        * 0.00021305290f;
}
