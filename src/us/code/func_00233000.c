#include "common.h"
#include "structs/game_object.h"
#include "structs/camera.h"

/* The leading 0x34 bytes of a game object: what the collision test reads. */
typedef struct {
    /* 0x00 */ CInstanceHeader header;
    /* 0x18 */ CVector3 m_vVelocity;
    /* 0x24 */ CVector3 m_vWaterDisplacement;
    /* 0x30 */ struct CGameObjectInstance *m_pInstanceBelow;
} ObjectHead; /* size 0x34 */

struct CollisionInfo;
extern struct CollisionInfo D_800F1B40;
CGameObjectInstance *func_00281194(CCamera *cam);
s32 func_00239F00(void *obj, CVector3 to, struct CollisionInfo *info);

/* Whether the line from the camera to `target` is clear: a collision test
 * from the camera's position, made with the camera's player (moved there and
 * back) when it has one and the camera is in its normal mode, else with a
 * copy of the target's head. */
s32 func_00233000(CGameObjectInstance *target, CCamera *cam) {
    ObjectHead tmp;
    CGameObjectInstance *player;
    s32 hit;

    player = func_00281194(cam);
    if (player != NULL && cam->m_ViewParams.m_nType == 0) {
        tmp = *(ObjectHead *) player;
        player->header.m_vPos = cam->m_vPos;
        hit = func_00239F00(player, target->header.m_vPos, &D_800F1B40);
        *(ObjectHead *) player = tmp;
    } else {
        tmp = *(ObjectHead *) target;
        tmp.header.m_vPos = cam->m_vPos;
        hit = func_00239F00(&tmp, target->header.m_vPos, &D_800F1B40);
    }
    return hit == 0;
}
