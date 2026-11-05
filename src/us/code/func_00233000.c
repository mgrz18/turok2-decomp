#include "common.h"
#include "structs/game_object.h"
#include "structs/camera.h"

struct CollisionInfo;
extern struct CollisionInfo D_800F1B40;
CGameObjectInstance *func_00281194(CCamera *cam);
s32 func_00239F00(void *obj, CVector3 to, struct CollisionInfo *info);

/* Whether the line from the camera to `target` is clear: a collision test
 * from the camera's position, made with the camera's player (moved there and
 * back) when it has one and the camera is in its normal mode, else with a
 * copy of the target's head. */
s32 func_00233000(CGameObjectInstance *target, CCamera *cam) {
    CObjectHead tmp;
    CGameObjectInstance *player;
    s32 hit;

    player = func_00281194(cam);
    if (player != NULL && cam->m_ViewParams.m_nType == 0) {
        tmp = *(CObjectHead *) player;
        player->header.m_vPos = cam->m_vPos;
        hit = func_00239F00(player, target->header.m_vPos, &D_800F1B40);
        *(CObjectHead *) player = tmp;
    } else {
        tmp = *(CObjectHead *) target;
        tmp.header.m_vPos = cam->m_vPos;
        hit = func_00239F00(&tmp, target->header.m_vPos, &D_800F1B40);
    }
    return hit == 0;
}
