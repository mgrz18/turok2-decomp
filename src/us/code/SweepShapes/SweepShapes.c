#include "common.h"
#include "structs/game_object.h"
#include "structs/region.h"
#include "structs/collision.h"
#include "globals/scene.h"
#include "globals/collision.h"
#include "globals/engine.h"
#include "protos/scene.h"
#include "protos/game.h"
#define MAX(a, b) ((a) < (b) ? (b) : (a))

void CCollisionResults__Clear(CCollisionInfo *def);
void Sweep_FollowGround(Sweep *sweep);
void CVector3__Scale(CVector3 *out, CVector3 *v, f32 k);
void SweepHit_Clear(SweepHit *hit);
void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);
void SweepHit_Store(SweepHit *out, CInstanceHeader *src);
s32 Sweep_GetGroundBehavior(Sweep *sweep, void *region);

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
    if (def != &gCollisionInfoPlatformRider && def != &gCollisionInfoTrace && def != &gCollisionInfoSight && def != &gCollisionInfoNoInstances) {
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

/* Where the segment from sweep->inst's position to sweep->end first enters
 * box, as a fraction of dir (slab test): 1 and *t if it does before maxT. */
s32 Sweep_HitBox(Sweep *sweep, CBox *box, f32 *t) {
    CVector3 hit;
    CVector3 *start;
    CVector3 *end;
    f32 d;

    end = &sweep->end;
    start = &sweep->inst->m_vPos;
    if (box->m_vMax.x >= start->x && box->m_vMax.y >= start->y && box->m_vMax.z >= start->z &&
        box->m_vMin.x <= start->x && box->m_vMin.y <= start->y && box->m_vMin.z <= start->z) {
        *t = 0.0f;
        return 1;
    }
    if (start->x != end->x) {
        if (start->x < end->x) {
            d = (box->m_vMin.x - start->x) / (end->x - start->x);
        } else {
            d = (start->x - box->m_vMax.x) / (start->x - end->x);
        }
        if (d < sweep->hit.fraction) {
            hit.y = start->y + d * sweep->delta.y;
            hit.z = start->z + d * sweep->delta.z;
            if (box->m_vMin.y <= hit.y && hit.y <= box->m_vMax.y && box->m_vMin.z <= hit.z && hit.z <= box->m_vMax.z) {
                *t = d;
                return 1;
            }
        }
    }
    if (start->y != end->y) {
        if (start->y < end->y) {
            d = (box->m_vMin.y - start->y) / (end->y - start->y);
        } else {
            d = (start->y - box->m_vMax.y) / (start->y - end->y);
        }
        if (d < sweep->hit.fraction) {
            hit.x = start->x + d * sweep->delta.x;
            hit.z = start->z + d * sweep->delta.z;
            if (box->m_vMin.x <= hit.x && hit.x <= box->m_vMax.x && box->m_vMin.z <= hit.z && hit.z <= box->m_vMax.z) {
                *t = d;
                return 1;
            }
        }
    }
    if (start->z == end->z) {
        return 0;
    }
    if (start->z < end->z) {
        d = (box->m_vMin.z - start->z) / (end->z - start->z);
    } else {
        d = (start->z - box->m_vMax.z) / (start->z - end->z);
    }
    if (d < sweep->hit.fraction) {
        hit.x = start->x + d * sweep->delta.x;
        hit.y = start->y + d * sweep->delta.y;
        if (box->m_vMin.x <= hit.x && hit.x <= box->m_vMax.x && box->m_vMin.y <= hit.y && hit.y <= box->m_vMax.y) {
            *t = d;
            return 1;
        }
    }
    return 0;
}

#include "structs/rect.h"

/* A rectangle on the ground plane. */
/* Sweep_HitBox on the ground plane: where the segment from sweep->inst's
 * position to sweep->end first enters rect (x and z only). */
s32 Sweep_HitRect(Sweep *sweep, CRect *rect, f32 *t) {
    CVector3 hit;
    CVector3 *start;
    CVector3 *end;
    f32 d;

    end = &sweep->end;
    start = &sweep->inst->m_vPos;
    if (rect->maxX >= start->x && rect->maxZ >= start->z && rect->minX <= start->x && rect->minZ <= start->z) {
        *t = 0.0f;
        return 1;
    }
    if (start->x != end->x) {
        if (start->x < end->x) {
            d = (rect->minX - start->x) / (end->x - start->x);
        } else {
            d = (start->x - rect->maxX) / (start->x - end->x);
        }
        if (d < sweep->hit.fraction) {
            hit.z = start->z + d * sweep->delta.z;
            if (rect->minZ <= hit.z && hit.z <= rect->maxZ) {
                *t = d;
                return 1;
            }
        }
    }
    if (start->z == end->z) {
        return 0;
    }
    if (start->z < end->z) {
        d = (rect->minZ - start->z) / (end->z - start->z);
    } else {
        d = (start->z - rect->maxZ) / (start->z - end->z);
    }
    if (d < sweep->hit.fraction) {
        hit.x = start->x + d * sweep->delta.x;
        if (rect->minX <= hit.x && hit.x <= rect->maxX) {
            *t = d;
            return 1;
        }
    }
    return 0;
}

void CVector3__Add(CVector3 *out, CVector3 *a, CVector3 *b);           /* out = a + b */
void CVector3__Lerp(CVector3 *out, f32 t, CVector3 *a, CVector3 *b);    /* out = a + (b - a) * t */
void CVector3__Bounce(CVector3 *out, CVector3 *v, CVector3 *n, f32 bounce); /* bounce off n */
void CVector3__RemoveComponent(CVector3 *out, CVector3 *v, CVector3 *n);           /* slide along n */
void CVector3__ProjectKeepLength(CVector3 *out, CVector3 *v, CVector3 *n);
f32 func_00298470(f32);                                                /* sqrtf */
#define DOT(a, b) ((a)->x * (b)->x + (a)->y * (b)->y + (a)->z * (b)->z)

/* Tests the sweep against the plane through point with this normal; on a
 * nearer hit records it and answers it the way kind says (1 stop, 2 slide,
 * 3 bounce, 4 and 5 other slides, 6 slide or bounce, 7 pass through).
 * Returns 1 when the hit was taken. */
s32 Sweep_HitPlane(Sweep *sweep, CVector3 *normal, CVector3 *point, s32 frontOnly, s32 kind, f32 friction,
                  s32 allowBehind) {
    CVector3 toPoint;
    CVector3 dir;
    CVector3 rest;
    CVector3 bounced;
    CVector3 move;
    CVector3 a;
    CVector3 b;
    CVector3 push;
    CVector3 vel;
    f32 denom;
    f32 t;
    f32 len;
    f32 xx;
    f32 zz;

    if (kind == 0) {
        return 0;
    }
    if (sweep->lastKind == 1 && sweep->lastContact.x == normal->x && sweep->lastContact.y == normal->y
        && sweep->lastContact.z == normal->z) {
        return 0;
    }
    denom = DOT(normal, &sweep->delta);
    if (denom == 0.0f) {
        return 0;
    }
    if (frontOnly && denom > 0.0f) {
        return 0;
    }
    CVector3__Subtract(&toPoint, point, &sweep->inst->m_vPos);
    t = DOT(normal, &toPoint) / denom;
    if (sweep->hit.fraction <= t) {
        return 0;
    }
    if (t < 0.0f && !allowBehind) {
        return 0;
    }
    if (t < -0.2f) {
        t = -0.2f;
    }
    gCollisionResultsP->m_vNormal = *normal;
    sweep->hit.fraction = t;
    CVector3__Lerp(&sweep->hit.pos, t, &sweep->inst->m_vPos, &sweep->end);
    if (kind == 6) {
        kind = 3;
        if (sweep->onGround) {
            kind = 2;
        }
    }
    if (kind == 3 && sweep->def->m_BounceReturnEnergy < 1.0f) {
        xx = sweep->delta.x * sweep->delta.x;
        zz = sweep->delta.z * sweep->delta.z;
        if (xx + sweep->delta.y * sweep->delta.y + zz < 16.777218f) {
            kind = 2;
            if (xx + zz < 1.0485761f) {
                kind = 1;
            }
        }
    }
    switch (kind) {
        case 7:
            sweep->kind = 0;
            sweep->hit.type = 1;
            if (sweep->physics) {
                sweep->hit.vel = sweep->obj->m_vVelocity;
            }
            sweep->hit.slideTo = sweep->end;
            return 1;
        case 1:
            sweep->kind = 0;
            sweep->hit.type = 2;
            if (sweep->physics) {
                sweep->hit.vel.x = sweep->hit.vel.y = sweep->hit.vel.z = 0.0f;
            }
            break;
        case 3:
            sweep->kind = 0;
            if (sweep->physics || sweep->more) {
                len = func_00298470(normal->x * normal->x + normal->y * normal->y + normal->z * normal->z);
                if (len == 0.0f) {
                    return 0;
                }
                CVector3__Scale(&dir, normal, 1.0f / len);
            }
            if (sweep->physics) {
                CVector3__Bounce(&vel, &sweep->obj->m_vVelocity, &dir, sweep->def->m_BounceReturnEnergy);
                sweep->hit.vel = vel;
                CVector3__Scale(&sweep->hit.vel, &sweep->hit.vel, 1.0f - friction);
            }
            if (sweep->more) {
                sweep->hit.type = 3;
                CVector3__Scale(&rest, &sweep->delta, 1.0f - t);
                CVector3__Bounce(&bounced, &rest, &dir, sweep->def->m_BounceReturnEnergy);
                CVector3__Scale(&bounced, &bounced, 1.0f - friction);
                CVector3__Add(&sweep->hit.slideTo, &sweep->hit.pos, &bounced);
            } else {
                sweep->hit.type = 2;
            }
            break;
        case 2:
            sweep->kind = 1;
            sweep->contact = *normal;
            if (sweep->physics || sweep->more) {
                len = func_00298470(normal->x * normal->x + normal->y * normal->y + normal->z * normal->z);
                if (len == 0.0f) {
                    return 0;
                }
                CVector3__Scale(&dir, normal, 1.0f / len);
            }
            if (sweep->physics) {
                CVector3__RemoveComponent(&vel, &sweep->obj->m_vVelocity, &dir);
                sweep->hit.vel = vel;
                CVector3__Scale(&sweep->hit.vel, &sweep->hit.vel, 1.0f - friction);
            }
            if (sweep->more) {
                sweep->hit.type = 3;
                CVector3__Scale(&rest, &sweep->delta, 1.0f - t);
                CVector3__RemoveComponent(&move, &rest, &dir);
                CVector3__Scale(&move, &move, 1.0f - friction);
                CVector3__Add(&sweep->hit.slideTo, &sweep->hit.pos, &move);
                CVector3__Scale(&push, &dir, 0.2048f);
                CVector3__Add(&sweep->hit.slideTo, &sweep->hit.slideTo, &push);
            } else {
                sweep->hit.type = 2;
            }
            break;
        case 4:
            sweep->kind = 1;
            sweep->contact = *normal;
            if (sweep->physics || sweep->more) {
                len = func_00298470(normal->x * normal->x + normal->y * normal->y + normal->z * normal->z);
                if (len == 0.0f) {
                    return 0;
                }
                CVector3__Scale(&dir, normal, 1.0f / len);
            }
            if (sweep->physics) {
                CVector3__ProjectKeepLength(&vel, &sweep->obj->m_vVelocity, &dir);
                sweep->hit.vel = vel;
            }
            if (sweep->more) {
                sweep->hit.type = 3;
                CVector3__Scale(&rest, &sweep->delta, 1.0f - t);
                CVector3__ProjectKeepLength(&move, &rest, &dir);
                CVector3__Add(&sweep->hit.slideTo, &sweep->hit.pos, &move);
                CVector3__Scale(&push, &dir, 0.2048f);
                CVector3__Add(&sweep->hit.slideTo, &sweep->hit.slideTo, &push);
            } else {
                sweep->hit.type = 2;
            }
            break;
        case 5:
            sweep->kind = 1;
            sweep->contact = *normal;
            if (sweep->physics || sweep->more) {
                len = func_00298470(normal->x * normal->x + normal->y * normal->y + normal->z * normal->z);
                if (len == 0.0f) {
                    return 0;
                }
                CVector3__Scale(&dir, normal, 1.0f / len);
            }
            if (sweep->physics) {
                CVector3__RemoveComponent(&a, &sweep->obj->m_vVelocity, &dir);
                CVector3__ProjectKeepLength(&b, &sweep->obj->m_vVelocity, &dir);
                CVector3__Lerp(&sweep->hit.vel, 0.25f, &a, &b);
                CVector3__Scale(&sweep->hit.vel, &sweep->hit.vel, 1.0f - friction);
            }
            if (sweep->more) {
                sweep->hit.type = 3;
                CVector3__Scale(&rest, &sweep->delta, 1.0f - t);
                CVector3__RemoveComponent(&a, &rest, &dir);
                CVector3__ProjectKeepLength(&b, &rest, &dir);
                CVector3__Lerp(&move, 0.25f, &a, &b);
                CVector3__Scale(&move, &move, 1.0f - friction);
                CVector3__Add(&sweep->hit.slideTo, &sweep->hit.pos, &move);
                CVector3__Scale(&push, &dir, 0.2048f);
                CVector3__Add(&sweep->hit.slideTo, &sweep->hit.slideTo, &push);
            } else {
                sweep->hit.type = 2;
            }
            break;
        case 6:
            break;
    }
    return 1;
}

/* Sweep against a vertical cylinder of the given radius round centre
 * (optionally only between minY and maxY): 1 if it is hit before maxT and
 * Sweep_HitPlane accepts the hit. */
s32 Sweep_HitCylinder(Sweep *sweep, CVector3 *centre, f32 radius, s32 checkHeight, f32 maxY, f32 minY,
                  s32 frontOnly, s32 arg7, f32 arg8, s32 fromInside) {
    CVector3 delta;
    CVector3 dir;
    CVector3 perp;
    CVector3 hit;
    CVector3 normal;
    f32 len;
    f32 invLen;
    f32 along;
    f32 disc;
    f32 t;

    if (sweep->lastKind == 2 && sweep->lastRadius == radius && sweep->lastContact.x == centre->x &&
        sweep->lastContact.y == centre->y && sweep->lastContact.z == centre->z) {
        return 0;
    }
    delta.x = centre->x - sweep->inst->m_vPos.x;
    delta.z = centre->z - sweep->inst->m_vPos.z;
    if (frontOnly && delta.x * sweep->delta.x + delta.z * sweep->delta.z < 0.0f) {
        return 0;
    }
    len = func_00298470(sweep->delta.x * sweep->delta.x + sweep->delta.z * sweep->delta.z);
    if (len == 0.0f) {
        return 0;
    }
    invLen = 1.0f / len;
    dir.x = sweep->delta.x * invLen;
    dir.z = sweep->delta.z * invLen;
    along = delta.x * dir.x + delta.z * dir.z;
    perp.x = delta.x - dir.x * along;
    perp.z = delta.z - dir.z * along;
    disc = radius * radius - (perp.x * perp.x + perp.z * perp.z);
    if (!(disc > 0.0f)) {
        return 0;
    }
    t = (along - func_00298470(disc)) * invLen;
    if (t < 0.0f && !fromInside) {
        return 0;
    }
    if (!(t < sweep->hit.fraction)) {
        return 0;
    }
    CVector3__Lerp(&hit, t, &sweep->inst->m_vPos, &sweep->end);
    if (checkHeight) {
        if (maxY < hit.y) {
            return 0;
        }
        if (hit.y < minY) {
            return 0;
        }
    }
    normal.x = hit.x - centre->x;
    normal.y = 0.0f;
    normal.z = hit.z - centre->z;
    if (Sweep_HitPlane(sweep, &normal, &hit, 0, arg7, arg8, 1) == 0) {
        return 0;
    }
    sweep->kind = 2;
    sweep->contact = *centre;
    sweep->radius = radius;
    return 1;
}

s32 GameObject_CanClimbLedges(CGameObjectInstance *obj);
s32 GameObject_CanClimbUp(CGameObjectInstance *obj);
s32 GameObject_CanDropDown(CGameObjectInstance *obj);
s32 GameObject_CanEnterWater(CGameObjectInstance *obj);
s32 GameObject_CanLeaveWater(CGameObjectInstance *obj);
s32 GameObject_CanEnterInfoFlag52_80(CGameObjectInstance *obj);
s32 GameObject_CanLeaveInfoFlag52_80(CGameObjectInstance *obj);
s32 GameObject_CanEnterInfoFlag400000(CGameObjectInstance *obj);
s32 GameObject_CanLeaveInfoFlag400000(CGameObjectInstance *obj);
s32 GameObject_CanEnterRegionFlag1000(CGameObjectInstance *obj);
s32 GameObject_CanLeaveRegionFlag1000(CGameObjectInstance *obj);
s32 GameObject_CanEnterRegionFlag80(CGameObjectInstance *obj);
s32 GameObject_CanLeaveRegionFlag80(CGameObjectInstance *obj);
s32 CGameRegion__IsSteep(CGameRegion *region);
s32 CGameRegion__ContainsXZ(CGameRegion *region, f32 x, f32 z);
CGameRegion *CGameRegion__FindBelow(CGameRegion *region, CVector3 pos);
f32 CGameRegion__GetCeilingHeight(CGameRegion *region, f32 x, f32 z);
f32 CGameRegion__GetHeight(CGameRegion *region, f32 x, f32 z);
f32 CGameRegion__GetEdgeYaw(CGameRegion *region, s32 edge);
void Angle_Wrap(f32 *angle);
f32 Angle_Difference(f32 a, f32 b);

/* Whether the region's water is only a surface (not swum in). */
static __inline__ s32 isShallow(CRegionInfo *info) {
    return info->m_dwFlags & 0x20000000;
}

/* Whether the sweep's instance may go from region `from` across its edge to
 * the neighbouring region: returns that region (or the one found under pos
 * when the link is one-way), or NULL. */
CGameRegion *Sweep_CrossRegionEdge(Sweep *sweep, CGameRegion *from, s32 edge, CVector3 *pos) {
    CVector3 p;
    f32 angle;
    CGameRegion *to;
    CGameObjectInstance *obj;
    CCollisionInfo *def;
    CRegionInfo *fromInfo;
    CRegionInfo *toInfo;
    s32 found;
    s32 i;
    s32 canSwim;
    s32 fromWater;
    s32 toWater;
    s32 fromSpecial;
    s32 toSpecial;
    s32 climbs;
    s32 canClimb;
    f32 d;
    f32 fromY;
    f32 toY;
    f32 c0;
    f32 c1;
    f32 c2;
    f32 top;
    f32 top2;
    f32 y;

    to = from->m_pNeighbours[edge];
    if (to == NULL) {
        return NULL;
    }
    obj = sweep->obj;
    def = sweep->def;
    if (from->m_wFlags & 0x20) {
        found = 0;
        for (i = 0; i < 3; i++) {
            if (to->m_pNeighbours[i] == from) {
                found = 1;
                break;
            }
        }
        if (!found) {
            canSwim = 0;
            if ((def->m_dwFlags & 0x20000) || GameObject_CanDropDown(obj)) {
                canSwim = 1;
            }
            if (!canSwim) {
                return NULL;
            }
            if (!CGameRegion__ContainsXZ(to, pos->x, pos->z)) {
                p.x = pos->x;
                p.y = obj->header.m_vPos.y;
                p.z = pos->z;
                to = CGameRegion__FindBelow(from, p);
                if (to == NULL) {
                    return NULL;
                }
            }
        }
    }
    fromInfo = CScene__GetRegionInfo(&gScene, from);
    toInfo = CScene__GetRegionInfo(&gScene, to);
    if (!(def->m_dwFlags & 0x10) && (REGION_WORD(to) & 6) == 2) {
        return NULL;
    }
    if (!(to->m_wFlags & 0x1000) && (from->m_wFlags & 0x1000) && !(def->m_dwFlags & 0x200) && !GameObject_CanLeaveRegionFlag1000(obj)) {
        return NULL;
    }
    if ((to->m_wFlags & 0x1000) && !(from->m_wFlags & 0x1000) && !(def->m_dwFlags & 0x100) && !GameObject_CanEnterRegionFlag1000(obj)) {
        return NULL;
    }
    if (!(toInfo->m_wFlags54 & 2) && (fromInfo->m_wFlags54 & 2) && !(def->m_dwFlags & 0x800)) {
        return NULL;
    }
    if ((toInfo->m_wFlags54 & 2) && !(fromInfo->m_wFlags54 & 2) && !(def->m_dwFlags & 0x400)) {
        return NULL;
    }
    if ((to->m_wFlags & 0x40) && !(from->m_wFlags & 0x40) && CGameRegion__GetCeilingHeight(to, pos->x, pos->z) < pos->y) {
        return NULL;
    }
    /* The do/while (0) block (a macro in the original?) keeps reorg's
     * delay-slot choices of the ROM. */
    do {
        if (obj->header.m_Type == 1 && (obj->m_AI.m_dwFlags & 0xC0000)) {
            return to;
        }
        if (!(toInfo->m_dwFlags & 0x400000) && (fromInfo->m_dwFlags & 0x400000) && !(def->m_dwFlags & 0x4000000)
            && !GameObject_CanLeaveInfoFlag400000(obj)) {
            return NULL;
        }
    } while (0);
    if ((toInfo->m_dwFlags & 0x400000) && !(fromInfo->m_dwFlags & 0x400000) && !(def->m_dwFlags & 0x2000000)
        && !GameObject_CanEnterInfoFlag400000(obj)) {
        return NULL;
    }
    if (!(toInfo->m_wFlags52 & 0x80) && (fromInfo->m_wFlags52 & 0x80) && !(def->m_dwFlags & 0x10000000)
        && !GameObject_CanLeaveInfoFlag52_80(obj)) {
        return NULL;
    }
    if ((toInfo->m_wFlags52 & 0x80) && !(fromInfo->m_wFlags52 & 0x80) && !(def->m_dwFlags & 0x8000000)
        && !GameObject_CanEnterInfoFlag52_80(obj)) {
        return NULL;
    }
    if (!(def->m_dwFlags & 0x80)) {
        if (!(to->m_wFlags & 0x80) && (from->m_wFlags & 0x80) && !(def->m_dwFlags & 0x2000) && !GameObject_CanLeaveRegionFlag80(obj)) {
            return NULL;
        }
        if ((to->m_wFlags & 0x80) && !(from->m_wFlags & 0x80) && !(def->m_dwFlags & 0x1000) && !GameObject_CanEnterRegionFlag80(obj)) {
            return NULL;
        }
    }
    fromWater = 0;
    if (from->m_wFlags & 1) {
        fromWater = !isShallow(fromInfo);
    }
    toWater = 0;
    if (to->m_wFlags & 1) {
        toWater = !isShallow(toInfo);
    }
    if (!toWater && fromWater && !(def->m_dwFlags & 0x80000) && !GameObject_CanLeaveWater(obj)) {
        return NULL;
    }
    if (toWater && !fromWater && !(def->m_dwFlags & 0x40000) && !GameObject_CanEnterWater(obj)) {
        return NULL;
    }
    if ((to->m_wFlags & 0x40) && !(from->m_wFlags & 0x40) && CGameRegion__GetCeilingHeight(to, pos->x, pos->z) < pos->y) {
        return NULL;
    }
    fromSpecial = CGameRegion__IsSteep(from);
    toSpecial = CGameRegion__IsSteep(to);
    if (sweep->def->m_dwFlags & 0x40) {
        if (toSpecial) {
            return to;
        }
        return NULL;
    }
    climbs = GameObject_CanClimbLedges(obj);
    canClimb = 0;
    if ((def->m_dwFlags & 0x10000) || GameObject_CanClimbUp(obj)) {
        canClimb = 1;
    }
    canSwim = 0;
    if ((def->m_dwFlags & 0x20000) || GameObject_CanDropDown(obj)) {
        canSwim = 1;
    }
    if (climbs && fromSpecial) {
        if (!toSpecial) {
            return to;
        }
        if ((from->m_wFlags & 0x10) && !(to->m_wFlags & 0x10)
            && obj->header.m_vPos.y < CGameRegion__GetHeight(from, obj->header.m_vPos.x, obj->header.m_vPos.z) + 1.024f) {
            return NULL;
        }
    }
    if (!toSpecial || fromSpecial) {
        return to;
    }
    if ((to->m_wFlags & 0x10) && climbs) {
        angle = CGameRegion__GetEdgeYaw(from, edge) + 3.1415927f;
        Angle_Wrap(&angle);
        d = Angle_Difference(angle, obj->m_RotY);
        if ((d < 0.0f) ? (-d <= 0.6981318f) : (d <= 0.6981318f)) {
            canClimb = 1;
        }
    }
    if (canClimb && canSwim) {
        return to;
    }
    fromY = from->m_pVertices[0]->y + from->m_pVertices[1]->y + from->m_pVertices[2]->y;
    c0 = to->m_pVertices[0]->y;
    c1 = to->m_pVertices[1]->y;
    c2 = to->m_pVertices[2]->y;
    toY = c0 + c1 + c2;
    if (fromY < toY) {
        if (canClimb) {
            return to;
        }
        y = obj->header.m_vPos.y - 0.1f;
        top = c1;
        if (!(c0 <= top)) {
            top = c0;
        }
        top2 = c2;
        if (!(top <= top2)) {
            top2 = top;
        }
        if (top2 < y) {
            return to;
        }
        return NULL;
    }
    if (canSwim) {
        return to;
    }
    return NULL;
}

void CGameRegion__GetNormalRaw(CVector3 *normal, CGameRegion *region);
f32 func_002982F0(f32);   /* sinf */
f32 func_002974C0(f32);   /* cosf */

/* Whether the sweep stays on the ground of its region; if so, end is put
 * on the region's surface at the same distance along the slope. */
void Sweep_FollowGround(Sweep *sweep) {
    CGameObjectInstance *obj;
    CRegionInfo *info;
    CVector3 normal;
    CVector3 toEnd;
    s32 allowed;
    s32 ok;
    f32 startY;
    f32 endY;
    f32 dy;
    f32 dx;
    f32 dz;
    f32 sum;
    f32 len;
    f32 k;
    f32 x;
    s32 flag;

    if (sweep->def->m_dwFlags & 0x01000000) {
        sweep->onGround = 0;
        return;
    }
    obj = sweep->obj;
    if (sweep->checkRegionInfo) {
        info = CScene__GetRegionInfo(&gScene, (CGameRegion *)obj->header.m_pCurrentRegion);
        if (info != NULL && (info->m_dwFlags & 0x4000)) {
            flag = 0;
        } else {
            flag = sweep->def->m_GroundBehavior;
        }
    } else {
        flag = sweep->def->m_GroundBehavior;
    }
    if (!(sweep->def->m_dwFlags & 0x8000)) {
        sweep->onGround = 0;
        return;
    }
    if (flag == 0) {
        sweep->onGround = 0;
        return;
    }
    CGameRegion__GetNormalRaw(&normal, (CGameRegion *)obj->header.m_pCurrentRegion);
    if (CGameRegion__IsSteep((CGameRegion *)obj->header.m_pCurrentRegion)) {
        if (GameObject_CanClimbLedges(obj)) {
            if (!(((CGameRegion *)obj->header.m_pCurrentRegion)->m_wFlags & 0x10)) {
                sweep->onGround = 0;
                return;
            }
            if (obj->header.m_Type == 1 &&
                normal.x * -func_002982F0(obj->m_RotY) + normal.z * -func_002974C0(obj->m_RotY) >= 0.0f) {
                sweep->onGround = 0;
                return;
            }
        } else {
            allowed = 0;
            if ((sweep->def->m_dwFlags & 0x10000) || GameObject_CanClimbUp(obj)) {
                allowed = 1;
            }
            if (allowed == 0) {
                sweep->onGround = 0;
                return;
            }
        }
    }
    startY = CGameRegion__GetHeight((CGameRegion *)obj->header.m_pCurrentRegion, obj->header.m_vPos.x, obj->header.m_vPos.z);
    endY = CGameRegion__GetHeight((CGameRegion *)obj->header.m_pCurrentRegion, sweep->end.x, sweep->end.z);
    if (obj->header.m_pCurrentRegion != NULL) {
        CVector3__Subtract(&toEnd, &sweep->end, ((CGameRegion *)obj->header.m_pCurrentRegion)->m_pVertices[0]);
    } else {
        toEnd = sweep->end;
    }
    sum = normal.x * toEnd.x + normal.y * toEnd.y + normal.z * toEnd.z;
    ok = sum <= 0.0f;
    sweep->onGround = ok;
    if (ok) {
        dy = endY - startY;
        dx = sweep->end.x - obj->header.m_vPos.x;
        dz = sweep->end.z - obj->header.m_vPos.z;
        sum = dx * dx + dz * dz;
        len = sum + dy * dy;
        if (len == 0.0f) {
            sweep->end.y = startY;
            return;
        }
        k = func_00298470(sum / len);
        x = obj->header.m_vPos.x + k * (sweep->end.x - obj->header.m_vPos.x);
        sweep->end.y = startY + k * dy;
        sweep->end.x = x;
        sweep->end.z = obj->header.m_vPos.z + k * (sweep->end.z - obj->header.m_vPos.z);
    }
}

/* Clears a sweep's hit: no hit yet, the whole move to go. */
void SweepHit_Clear(SweepHit *hit) {
    hit->type = 0;
    hit->posOut = NULL;
    hit->regionOut = NULL;
    hit->kindOut = NULL;
    hit->dataOut = NULL;
    hit->fraction = 1.0f;
}

/* Writes a sweep's hit back where the test that took it asked: the object's
 * position and region, the kind and the data (see SweepHit). */
void SweepHit_Store(SweepHit *out, CInstanceHeader *src) {
    if (out->posOut != NULL) {
        *out->posOut = src->m_vPos;
    }
    if (out->regionOut != NULL) {
        *out->regionOut = src->m_pCurrentRegion;
    }
    if (out->kindOut != NULL) {
        *out->kindOut = out->kind;
    }
    if (out->dataOut != NULL) {
        *out->dataOut = out->data;
    }
}

/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CScene__GetRegionInfo gets this
 * function's $a1 untouched, the region its two callers pass; the host takes and passes it. */
s32 Sweep_GetGroundBehavior(Sweep *sweep, void *region) {
    CRegionInfo *target;

    target = CScene__GetRegionInfo(&gScene, region);
    if (target != NULL && (target->m_dwFlags & 0x4000)) {
        return 0;
    }
    return sweep->def->m_GroundBehavior;
}

#include "m2c_macros.h"
#include "structs/vector.h"

void CVector3__Subtract(CVector3 *arg0, CVector3 *arg1, CVector3 *arg2);
void CVector3__Cross(CVector3 *out, CVector3 *a, CVector3 *b);

void Quad_ComputeNormal(CVector3 *arg0) {
    CVector3 sp10;
    CVector3 sp20;
    CVector3 *temp_s1;

    temp_s1 = ((__typeof__(arg0))((s8 *)arg0 + 0xC));
    CVector3__Subtract(&sp10, temp_s1, arg0);
    CVector3__Subtract(&sp20, ((__typeof__(arg0))((s8 *)arg0 + 0x18)), temp_s1);
    CVector3__Cross(((__typeof__(arg0))((s8 *)arg0 + 0x30)), &sp10, &sp20);
}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/ANIM/80241170.CPP, func_80241260_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

struct Plane;
typedef struct Plane Plane;
struct Plane {
    CVector3 pos; /* 0x00 in Turok 2 */
    char pad24[0x24];
    CVector3 normal;
};

s32 Plane_IsBehind(void *arg0, CVector3 *arg1) {
    Plane *object = arg0;
    CVector3 sp10;
    s32 var_v0;
    CVector3__Subtract(&sp10, arg1, &object->pos);
    var_v0 = 0;
    if (!((object->normal.x * sp10.x) + (object->normal.y * sp10.y) + (object->normal.z * sp10.z) >= 0.0f)) {
        var_v0 = 1;
    }
    return var_v0;
}

s32 Plane_SegmentCrosses(CollisionQuad *plane, CVector3 *a, CVector3 *b) {
    CVector3 d;
    s32 sideA;
    s32 sideB;

    CVector3__Subtract(&d, a, (CVector3 *) plane);
    sideA = (plane->normal.x * d.x + plane->normal.y * d.y + plane->normal.z * d.z >= 0.0f) ? 0 : 1;
    CVector3__Subtract(&d, b, &plane->corner[0]);
    sideB = (plane->normal.x * d.x + plane->normal.y * d.y + plane->normal.z * d.z >= 0.0f) ? 0 : 1;
    return sideA ^ sideB;
}

s32 Plane_IntersectSegmentT(CollisionQuad *plane, CVector3 *start, CVector3 *end, f32 *t) {
    CVector3 dir;
    CVector3 diff;
    f32 denom;

    CVector3__Subtract(&dir, end, start);
    denom = plane->normal.x * dir.x + plane->normal.y * dir.y + plane->normal.z * dir.z;
    if (denom >= 0.0f) {
        *t = 1.0f;
        return 0;
    }
    CVector3__Subtract(&diff, start, &plane->corner[0]);
    *t = -(plane->normal.x * diff.x + plane->normal.y * diff.y + plane->normal.z * diff.z) / denom;
    return 1;
}

/* Plane_IntersectSegmentT, inlined. */
static __inline__ s32 PlaneSegmentT(CollisionQuad *plane, CVector3 *start, CVector3 *end, f32 *t) {
    CVector3 dir;
    CVector3 diff;
    f32 denom;

    CVector3__Subtract(&dir, end, start);
    denom = plane->normal.x * dir.x + plane->normal.y * dir.y + plane->normal.z * dir.z;
    if (denom >= 0.0f) {
        *t = 1.0f;
        return 0;
    }
    CVector3__Subtract(&diff, start, &plane->corner[0]);
    *t = -(plane->normal.x * diff.x + plane->normal.y * diff.y + plane->normal.z * diff.z) / denom;
    return 1;
}

/* Where the segment from start to end crosses plane (from its front), in *hit. */
s32 Plane_IntersectSegment(CollisionQuad *plane, CVector3 *start, CVector3 *end, CVector3 *hit) {
    f32 t;

    if (PlaneSegmentT(plane, start, end, &t)) {
        CVector3__Lerp(hit, t, start, end);
        return 1;
    }
    return 0;
}

/* Returns 1 when point lies on the inner side of all four edges of the quad corners. */
s32 Quad_ContainsPoint(CVector3 *corners, CVector3 point) {
    CVector3 edge;
    CVector3 toPoint;
    s32 i;
    s32 next;

    for (i = 0; i < 4; i++) {
        next = (i == 3) ? 0 : i + 1;
        CVector3__Subtract(&edge, &corners[next], &corners[i]);
        CVector3__Subtract(&toPoint, &point, &corners[i]);
        if (edge.x * toPoint.x + edge.y * toPoint.y + edge.z * toPoint.z < 0.0f) {
            return 0;
        }
    }
    return 1;
}

/* Point-in-quad test in one axis-aligned projection of the quad; the edge winding follows the sign of the normal. */
s32 Quad_ContainsPointYZ(CollisionQuad *quad, f32 u, f32 v) {
    s32 i;
    CVector3 *from;
    CVector3 *to;
    f32 d;

    for (i = 0; i < 4; i++) {
        if (0.0f < quad->normal.x) {
            to = &quad->corner[i];
            from = &quad->corner[(i + 1) & 3];
        } else {
            from = &quad->corner[i];
            to = &quad->corner[(i + 1) & 3];
        }
        d = (to->z - from->z) * (u - from->y) + (from->y - to->y) * (v - from->z);
        if (d < 0.0f) {
            return 0;
        }
    }
    return 1;
}

/* Point-in-quad test in one axis-aligned projection of the quad; the edge winding follows the sign of the normal. */
s32 Quad_ContainsPointXZ(CollisionQuad *quad, f32 u, f32 v) {
    s32 i;
    CVector3 *from;
    CVector3 *to;
    f32 d;

    for (i = 0; i < 4; i++) {
        if (quad->normal.y < 0.0f) {
            to = &quad->corner[i];
            from = &quad->corner[(i + 1) & 3];
        } else {
            from = &quad->corner[i];
            to = &quad->corner[(i + 1) & 3];
        }
        d = (to->z - from->z) * (u - from->x) + (from->x - to->x) * (v - from->z);
        if (d <= 0.0f) {
            return 0;
        }
    }
    return 1;
}

/* Point-in-quad test in one axis-aligned projection of the quad; the edge winding follows the sign of the normal. */
s32 Quad_ContainsPointXY(CollisionQuad *quad, f32 u, f32 v) {
    s32 i;
    CVector3 *from;
    CVector3 *to;
    f32 d;

    for (i = 0; i < 4; i++) {
        if (0.0f < quad->normal.z) {
            to = &quad->corner[i];
            from = &quad->corner[(i + 1) & 3];
        } else {
            from = &quad->corner[i];
            to = &quad->corner[(i + 1) & 3];
        }
        d = (to->y - from->y) * (u - from->x) + (from->x - to->x) * (v - from->y);
        if (d < 0.0f) {
            return 0;
        }
    }
    return 1;
}

/* Height of the quad's plane at (x, z); a vertical quad gives the mean corner height. */
f32 Quad_GetHeightAt(CollisionQuad *quad, f32 x, f32 z) {
    CVector3 normal;
    CVector3 origin;

    normal = quad->normal;
    if (normal.y == 0.0f) {
        return (quad->corner[0].y + quad->corner[1].y + quad->corner[2].y + quad->corner[3].y) * 0.25f;
    }
    origin = quad->corner[0];
    return ((origin.z - z) * normal.z + (origin.x - x) * normal.x + origin.y * normal.y) / normal.y;
}

void CVector3__Add(CVector3 *, CVector3 *, CVector3 *);

void Quad_Translate(CVector3 *arg0, CVector3 *arg1) {
    CVector3 *temp_a0;
    CVector3 *temp_a0_2;
    CVector3 *temp_s0;

    CVector3__Add(arg0, arg0, arg1);
    temp_a0 = ((__typeof__(arg0))((s8 *)arg0 + 0xC));
    CVector3__Add(temp_a0, temp_a0, arg1);
    temp_a0_2 = ((__typeof__(arg0))((s8 *)arg0 + 0x18));
    CVector3__Add(temp_a0_2, temp_a0_2, arg1);
    temp_s0 = ((__typeof__(arg0))((s8 *)arg0 + 0x24));
    CVector3__Add(temp_s0, temp_s0, arg1);
}

/* The edges belong to this helper; declared in the face function, GCC grows the frame. */
static __inline__ void quad_normal(CollisionQuad *quad) {
    CVector3 edge1;
    CVector3 edge2;

    CVector3__Subtract(&edge1, &quad->corner[1], &quad->corner[0]);
    CVector3__Subtract(&edge2, &quad->corner[2], &quad->corner[1]);
    CVector3__Cross(&quad->normal, &edge1, &edge2);
}

/* Sets a box face from points 0, 1, 2 and 3 of the box and computes its (unnormalised) normal. */
void Quad_SetBoxBottom(CollisionQuad *quad, CVector3 *points) {
    quad->corner[0] = points[0];
    quad->corner[1] = points[1];
    quad->corner[2] = points[2];
    quad->corner[3] = points[3];
    quad_normal(quad);
}

/* Sets a box face from points 0, 3, 7 and 4 of the box and computes its (unnormalised) normal. */
void Quad_SetBoxSide0374(CollisionQuad *quad, CVector3 *points) {
    quad->corner[0] = points[0];
    quad->corner[1] = points[3];
    quad->corner[2] = points[7];
    quad->corner[3] = points[4];
    quad_normal(quad);
}

/* Sets a box face from points 2, 1, 5 and 6 of the box and computes its (unnormalised) normal. */
void Quad_SetBoxSide2156(CollisionQuad *quad, CVector3 *points) {
    quad->corner[0] = points[2];
    quad->corner[1] = points[1];
    quad->corner[2] = points[5];
    quad->corner[3] = points[6];
    quad_normal(quad);
}

/* Sets a box face from points 3, 2, 6 and 7 of the box and computes its (unnormalised) normal. */
void Quad_SetBoxSide3267(CollisionQuad *quad, CVector3 *points) {
    quad->corner[0] = points[3];
    quad->corner[1] = points[2];
    quad->corner[2] = points[6];
    quad->corner[3] = points[7];
    quad_normal(quad);
}

/* Sets a box face from points 4, 5, 1 and 0 of the box and computes its (unnormalised) normal. */
void Quad_SetBoxSide4510(CollisionQuad *quad, CVector3 *points) {
    quad->corner[0] = points[4];
    quad->corner[1] = points[5];
    quad->corner[2] = points[1];
    quad->corner[3] = points[0];
    quad_normal(quad);
}

/* Sets a box face from points 7, 6, 5 and 4 of the box and computes its (unnormalised) normal. */
void Quad_SetBoxTop(CollisionQuad *quad, CVector3 *points) {
    quad->corner[0] = points[7];
    quad->corner[1] = points[6];
    quad->corner[2] = points[5];
    quad->corner[3] = points[4];
    quad_normal(quad);
}

typedef struct Sphere {
    CVector3 center;     /* 0x00 */
    f32 radius;      /* 0x0C */
    f32 radiusSq;    /* 0x10 */
} Sphere;

void Sphere_Set(Sphere *sphere, CVector3 *center, f32 radius) {
    sphere->center = *center;
    sphere->radius = radius;
    sphere->radiusSq = radius * radius;
}

/* A vertical cylinder: its centre and its squared radius. */
typedef struct {
    /* 0x00 */ CVector3 pos;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 radiusSq;
} Cylinder;

s32 Cylinder_ContainsPoint(Cylinder *arg0, CVector3 *arg1) {
    f32 temp_f0;
    f32 temp_f1;
    s32 var_v0;

    temp_f1 = arg0->pos.x - arg1->x;
    temp_f0 = arg0->pos.z - arg1->z;
    var_v0 = 1;
    if (!(((temp_f1 * temp_f1) + (temp_f0 * temp_f0)) <= arg0->radiusSq)) {
        var_v0 = 0;
    }
    return var_v0;
}

/* Whether the segment from a to b crosses the circle of sphere on the ground
 * plane (x and z only). */
s32 Circle_SegmentIntersects(Sphere *sphere, CVector3 *a, CVector3 *b) {
    CVector3 toCenter;
    CVector3 dir;
    CVector3 perp;
    CVector3 seg;
    f32 len;
    f32 invLen;
    f32 along;
    f32 disc;
    f32 t;

    seg.x = b->x - a->x;
    seg.z = b->z - a->z;
    toCenter.x = sphere->center.x - a->x;
    toCenter.z = sphere->center.z - a->z;
    len = func_00298470(seg.x * seg.x + seg.z * seg.z);
    if (len == 0.0f) {
        return 0;
    }
    invLen = 1.0f / len;
    dir.x = seg.x * invLen;
    dir.z = seg.z * invLen;
    along = toCenter.x * dir.x + toCenter.z * dir.z;
    perp.x = toCenter.x - dir.x * along;
    perp.z = toCenter.z - dir.z * along;
    disc = sphere->radiusSq - (perp.x * perp.x + perp.z * perp.z);
    if (!(disc > 0.0f)) {
        return 0;
    }
    t = (along - func_00298470(disc)) * invLen;
    return t >= 0.0f && t <= 1.0f;
}
