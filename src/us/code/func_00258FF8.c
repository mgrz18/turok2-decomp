#include "common.h"

#ifdef TARGET_PORT
#include "structs/game_object.h"
#include "structs/pose_host.h"
/* Host body: the pose callback's context is T2PoseContext (structs/pose_host.h; the N64
 * view reads the instance at 0x00 and the bone at 0x90 of the N64 layout). */

void CMtxF__PreRotateY(void *mtx, f32 angle);

void WeaponNode_Rotate_AF324(void *mtx, T2PoseContext *arg1) {
    f32 angle = arg1->obj->m_AI.m_Weapon.m_BarrelAngle * 1.5f; /* N64 instance 0x24C */

    if (arg1->bone == 2) {
        CMtxF__PreRotateY(mtx, -angle);
    }
}
#else
void CMtxF__PreRotateY(void *mtx, f32 angle);

void WeaponNode_Rotate_AF324(void *mtx, void *arg1) {
    f32 angle = *(f32 *) (*(s8 **) arg1 + 0x24C) * 1.5f;

    if (*(s32 *) ((s8 *) arg1 + 0x90) == 2) {
        CMtxF__PreRotateY(mtx, -angle);
    }
}
#endif
