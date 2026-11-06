#include "common.h"
#include "structs/game_object.h"
#include "structs/camera.h"
#include "globals/collision.h"

struct CollisionInfo;
CGameObjectInstance *CCamera__GetPlayer(CCamera *cam);
s32 Collision_MoveInstance(void *obj, CVector3 to, struct CollisionInfo *info);

/* Whether the line from the camera to `target` is clear: a collision test
 * from the camera's position, made with the camera's player (moved there and
 * back) when it has one and the camera is in its normal mode, else with a
 * copy of the target's head. */
s32 CCamera__CanSeeTarget(CGameObjectInstance *target, CCamera *cam) {
    CObjectHead tmp;
    CGameObjectInstance *player;
    s32 hit;

    player = CCamera__GetPlayer(cam);
    if (player != NULL && cam->m_ViewParams.m_nType == 0) {
        tmp = *(CObjectHead *) player;
        player->header.m_vPos = cam->m_vPos;
        hit = Collision_MoveInstance(player, target->header.m_vPos, &gCollisionInfoLineOfSight);
        *(CObjectHead *) player = tmp;
    } else {
        tmp = *(CObjectHead *) target;
        tmp.header.m_vPos = cam->m_vPos;
        hit = Collision_MoveInstance(&tmp, target->header.m_vPos, &gCollisionInfoLineOfSight);
    }
    return hit == 0;
}
