#include "common.h"
#include "protos/weapon_mode.h"

#ifdef TARGET_PORT
#include "structs/game_object.h"
#include "structs/pose_host.h"
/* Host body: the pose callback's context is T2PoseContext (structs/pose_host.h; the N64
 * view reads the instance at 0x00 and the bone at 0x90 of the N64 layout). */


void WeaponNode_Rotate_AF1D4(void *mtx, T2PoseContext *bone) {
    f32 angle = bone->obj->m_AI.m_Weapon.m_BarrelAngle; /* N64 instance 0x24C */

    switch (bone->bone) {
    case 1:
        CMtxF__PreRotateY(mtx, -angle);
        break;
    case 12:
        CMtxF__PreRotateY(mtx, angle + angle);
        break;
    }
}
#else
#include "structs/weapon_node.h"


void WeaponNode_Rotate_AF1D4(void *mtx, WeaponNodeBone *bone) {
    f32 angle = bone->anim->angle;

    switch (bone->type) {
    case 1:
        CMtxF__PreRotateY(mtx, -angle);
        break;
    case 12:
        CMtxF__PreRotateY(mtx, angle + angle);
        break;
    }
}
#endif
