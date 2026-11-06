#include "common.h"
#include "structs/game_object.h"
#include "structs/region.h"
#include "structs/collision.h"
#include "globals/scene.h"
#include "globals/collision.h"

#define MAX(a, b) ((a) < (b) ? (b) : (a))

extern f32 gFrameIncrement;                  /* frame time, in 15ths of a second */
extern void (*sCollisionTests[])(Sweep *);   /* the collision tests, NULL-ended */
extern CCollisionInfo D_800F1E10;
extern CCollisionInfo D_800F1CC0;
extern CCollisionInfo D_800F1C60;

CRegionInfo *CScene__GetRegionInfo(s32 *list, CGameRegion *region);
void CCollisionResults__Clear(CCollisionInfo *def);
void Sweep_FollowGround(Sweep *sweep);
void CVector3__Scale(CVector3 *out, CVector3 *v, f32 k);
void SweepHit_Clear(SweepHit *hit);
void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);
void SweepHit_Store(SweepHit *hit, CGameObjectInstance *obj);
f32 GameObject_GetRegionFloorHeight(CGameObjectInstance *obj, s32 arg1);
s32 Sweep_GetGroundBehavior(Sweep *sweep, CGameRegion *region);

/* Moves obj to pos through the collision tests, in up to 100 slides, and
 * returns the number of hits. */
s32 Collision_MoveInstance(CGameObjectInstance *obj, CVector3 pos, CCollisionInfo *def) {
    Sweep sweep;
    CVector3 accel;
    CRegionInfo *info;
    s32 iter;
    s32 i;
    s32 hits;
    s32 passes;
    s32 physics;
    s32 more;
    f32 remaining;
    f32 dt;
    f32 startY;
    f32 drag;
    f32 max;
    f32 half;
    f32 ground;
    f32 dy;

    info = CScene__GetRegionInfo(&gScene, obj->header.m_pCurrentRegion);
    if (def != &D_800F1E10 && def != &D_800F1CC0 && def != &gCollisionInfoSight && def != &D_800F1C60) {
        obj->m_pInstanceBelow = 0;
        if (obj->header.m_Type == 1) {
            obj->m_AI.m_dwFlags &= ~0x30000;
        }
    }
    CCollisionResults__Clear(def);
    iter = 0;
    remaining = 1.0f;
    hits = 0;
    sweep.obj = obj;
    sweep.def = def;
    sweep.end = pos;
    sweep.stop = 0;
    sweep.unkA0 = 0;
    sweep.lastKind = 0;
    dt = gFrameIncrement * (1.0f / 15);
    if (def->m_dwFlags & 4) {
        passes = 1;
    } else if (def->m_dwFlags & 0x200000) {
        passes = 3;
    } else {
        passes = 2;
    }
    physics = 0;
    if ((def->m_dwFlags & 0x4000) && dt != 0.0f) {
        physics = 1;
    }
    sweep.physics = physics;
    startY = obj->header.m_vPos.y;
    if (physics) {
        drag = def->m_AirFriction;
        if (def->m_WaterFriction != drag && obj->header.m_pCurrentRegion != NULL && (obj->header.m_pCurrentRegion->m_wFlags & 1) && info != NULL
            && startY < info->m_WaterHeight) {
            drag = def->m_WaterFriction;
        }
        drag *= 15.0f;
        max = 1.0f / dt;
        if (!(drag <= max)) {
            drag = max;
        }
        if (drag == 0.0f) {
            accel.x = 0.0f;
            accel.y = def->m_GravityAcceleration;
            accel.z = 0.0f;
        } else {
            accel.x = -obj->m_vVelocity.x * drag;
            accel.y = -obj->m_vVelocity.y * drag + def->m_GravityAcceleration;
            accel.z = -obj->m_vVelocity.z * drag;
        }
        half = dt * 0.5f;
        sweep.end.x += (obj->m_vVelocity.x + accel.x * half) * dt;
        sweep.end.y += (obj->m_vVelocity.y + accel.y * half) * dt;
        sweep.end.z += (obj->m_vVelocity.z + accel.z * half) * dt;
        obj->m_vVelocity.x += accel.x * dt;
        obj->m_vVelocity.y += accel.y * dt;
        obj->m_vVelocity.z += accel.z * dt;
        gCollisionResultsP->m_vVelocity = obj->m_vVelocity;
    }
    if (obj->header.m_vPos.x == sweep.end.x && obj->header.m_vPos.y == sweep.end.y && obj->header.m_vPos.z == sweep.end.z) {
        return 0;
    }
    if (info != NULL) {
        sweep.checkRegionInfo = info->m_dwFlags & 0x4000;
    } else {
        sweep.checkRegionInfo = 0;
    }
    sweep.more = hits + 1 < passes;
    Sweep_FollowGround(&sweep);
    if (sweep.onGround && sweep.physics) {
        CVector3__Scale(&obj->m_vVelocity, &obj->m_vVelocity, (def->m_GroundFriction > 1.0f) ? 0.0f : 1.0f - def->m_GroundFriction);
    }
    do {
        SweepHit_Clear(&sweep.hit);
        CVector3__Subtract(&sweep.delta, &sweep.end, &obj->header.m_vPos);
        if (sweep.physics) {
            sweep.hit.vel = obj->m_vVelocity;
        }
        if (sCollisionTests[0] != NULL && !sweep.stop) {
            i = 0;
            do {
                sCollisionTests[i](&sweep);
                i++;
            } while (sCollisionTests[i] != NULL && !sweep.stop);
        }
        switch ((u32) sweep.hit.type) {
            case 2:
            case 3:
                hits++;
            case 1:
                obj->header.m_vPos = sweep.hit.pos;
                SweepHit_Store(&sweep.hit, obj);
                if (sweep.physics) {
                    obj->m_vVelocity = sweep.hit.vel;
                }
                sweep.lastKind = sweep.kind;
                sweep.lastContact = sweep.contact;
                sweep.lastRadius = sweep.radius;
                break;
            case 0:
                obj->header.m_vPos = sweep.end;
                break;
        }
        if ((def->m_dwFlags & 0x800000) && gCollisionResultsP->m_pWallRegion != 0) {
            return hits;
        }
        remaining -= sweep.hit.fraction * remaining;
        if (sweep.onGround) {
            ground = GameObject_GetRegionFloorHeight(obj, 0);
            obj->header.m_vPos.y = (obj->header.m_vPos.y <= ground) ? ground : obj->header.m_vPos.y;
        }
        more = 0;
        if (sweep.hit.type == 1 || sweep.hit.type == 3) {
            more = 1;
        }
        if (more) {
            sweep.end = sweep.hit.slideTo;
            sweep.more = hits + 1 < passes;
            Sweep_FollowGround(&sweep);
        }
        iter++;
    } while (more && iter < 100 && hits < passes);

    if (def->m_WallBehavior != 0 && !(def->m_dwFlags & 0x01000000) && Sweep_GetGroundBehavior(&sweep, obj->header.m_pCurrentRegion)) {
        if (remaining > 0.01f) {
            if (sweep.hit.posOut == &gCollisionResultsP->m_vInstancePos) {
                more = def->m_InstanceBehavior != 1;
            } else if (sweep.hit.posOut == &gCollisionResultsP->m_vWallPos) {
                more = def->m_WallBehavior != 1;
            } else {
                more = 0;
            }
        } else {
            more = 0;
        }
        ground = GameObject_GetRegionFloorHeight(obj, 0);
        obj->header.m_vPos.y = MAX(ground, more ? sweep.end.y : obj->header.m_vPos.y);
    }
    if (sweep.onGround && sweep.physics && dt != 0.0f && def->m_GravityAcceleration != 0.0f) {
        dy = obj->header.m_vPos.y - startY;
        if (dy < 204.79999f) {
            obj->m_vVelocity.y = dy / dt;
        }
    }
    return hits;
}
