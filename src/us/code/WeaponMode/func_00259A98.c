#include "common.h"

#ifdef TARGET_PORT
#include "structs/game_object.h"
#include "structs/pose_host.h"
/* Host body: the pose callback's context is T2PoseContext (structs/pose_host.h; the N64
 * view reads the instance at 0x00 and the bone at 0x90 of the N64 layout). */

void CMtxF__PreRotateZ(void *mtx, f32 angle);

void WeaponNode_Rotate_AF180(void *mtx, T2PoseContext *bone) {
    if (bone->bone == 2) {
        CMtxF__PreRotateZ(mtx, -bone->obj->m_AI.m_Weapon.m_BarrelAngle * 0.5f);
    }
}
#else
#include "structs/weapon_node.h"

void CMtxF__PreRotateZ(void *mtx, f32 angle);

void WeaponNode_Rotate_AF180(void *mtx, WeaponNodeBone *bone) {
    if (bone->type == 2) {
        CMtxF__PreRotateZ(mtx, -bone->anim->angle * 0.5f);
    }
}
#endif
