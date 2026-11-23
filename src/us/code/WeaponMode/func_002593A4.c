#include "common.h"
#include "structs/game_object.h"

#ifdef TARGET_PORT
#include "structs/pose_host.h"
/* Host body: the pose callback's context is T2PoseContext (structs/pose_host.h; the N64
 * view reads the instance at 0x00 and the bone at 0x90 of the N64 layout). */

void CMtxF__PreRotateY(void *mtx, f32 angle);

void WeaponNode_Rotate_AF084(sptr arg0, T2PoseContext *arg1) {
    if (arg1->bone == 2) {
        CMtxF__PreRotateY((void *) arg0, arg1->obj->m_AI.m_Weapon.m_BarrelAngle);
    }
}
#else
typedef struct {
    /* 0x00 */ CGameObjectInstance *owner;
    /* 0x04 */ u8 pad4[0x90 - 0x4];
    /* 0x90 */ s32 state;
} Arg1;

f32 CMtxF__PreRotateY(sptr, f32);

void WeaponNode_Rotate_AF084(sptr arg0, Arg1 *arg1) {
    if (arg1->state == 2) {
        CMtxF__PreRotateY(arg0, arg1->owner->m_AI.m_Weapon.m_BarrelAngle);
    }
}
#endif
