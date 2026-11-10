#include "common.h"
#include "structs/game_object.h"

#ifdef TARGET_PORT
#include "structs/pose_host.h"
/* Host body: the pose callback's context is T2PoseContext (structs/pose_host.h; the N64
 * view reads the instance at 0x00 and the bone at 0x90 of the N64 layout). */

extern void CMtxF__PreRotateY(void *arg0, f32 arg1);
extern void CMtxF__RotateY(void *arg0, f32 arg1);

void WeaponBarrel_NodeMatrix_AEE38(void *arg0, T2PoseContext *state) {
    f32 value = state->obj->m_AI.m_Weapon.m_BarrelAngle;
    f32 half = value * 0.5f;
    f32 more = value * 1.5f;

    switch (state->bone) {
    case 1:
        CMtxF__PreRotateY(arg0, half);
        break;
    case 7:
        CMtxF__RotateY(arg0, more);
        break;
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
        CMtxF__PreRotateY(arg0, -more);
        break;
    }
}
#else
typedef struct {
    /* 0x00 */ CGameObjectInstance *owner;
    /* 0x04 */ u8 pad4[0x90 - 0x4];
    /* 0x90 */ s32 mode;
} WeaponState;

extern void CMtxF__PreRotateY(void *arg0, f32 arg1);
extern void CMtxF__RotateY(void *arg0, f32 arg1);

void WeaponBarrel_NodeMatrix_AEE38(void *arg0, WeaponState *state) {
    f32 value = state->owner->m_AI.m_Weapon.m_BarrelAngle;
    f32 half = value * 0.5f;
    f32 more = value * 1.5f;

    switch (state->mode) {
    case 1:
        CMtxF__PreRotateY(arg0, half);
        break;
    case 7:
        CMtxF__RotateY(arg0, more);
        break;
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
        CMtxF__PreRotateY(arg0, -more);
        break;
    }
}
#endif
