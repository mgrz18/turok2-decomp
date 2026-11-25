/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/CAMERA/CODE_80245D20.CPP, func_80245D30_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/game_object.h"
#include "globals/engine.h"
#include "globals/region.h"

extern s32 GameObject_IsOnGround(void *actor);
extern f32 CVector3__GetYaw(CVector3 *v);                                 /* yaw of v */
extern void CVector3__Scale(CVector3 *out, CVector3 *in, f32 s);            /* out = in * s */
extern void CVector3__Lerp(CVector3 *out, f32 t, CVector3 *a, CVector3 *b);   /* out = lerp(a, b, t) */
extern void Angle_Wrap(f32 *angle);                              /* wraps an angle */
/* A surface that carries what stands on it (water current, conveyor). */
#include "structs/region.h"

static __inline__ s32 currentEnabled(CGameObjectInstance *actor) {
    if (actor->m_AI.m_dwFlags & 0x280) return 1;
    if (actor->m_AI.m_dwFlags & 0x400) return GameObject_IsOnGround(actor);
    return 0;
}

/* Applies a surface current to an actor: pushes pos, turns the actor with it
 * (flags 0x600 spin, 0x2000 face the flow) and handles the vertical part. */
void GameObject_ApplyCurrent(CGameObjectInstance *actor, CRegionInfo *mat, CVector3 *pos, s32 active) {
    CGameObjectInstance *rider;
    CVector3 flow;
    CVector3 flat;
    f32 cur;
    f32 want;
    f32 spin;
    f32 strength;
    f32 wrapped;
    f32 target;
    f32 from;
    f32 blend;
    s32 enabled;
    s32 blocked;
    u16 flags;
    CVector3 *current;

    if (actor->header.m_Type == 1) {
        rider = actor;
        enabled = currentEnabled(actor);
        blocked = rider->m_AI.m_dwFlags & 0xC0000;
    } else {
        enabled = 0;
        blocked = 0;
    }
    if (blocked != 0 || (enabled == 0 && !(mat->m_wFlags52 & 0x8000))) {
        actor->m_vWaterDisplacement.x = 0.0f;
        actor->m_vWaterDisplacement.y = 0.0f;
        actor->m_vWaterDisplacement.z = 0.0f;
        return;
    }
    if ((mat->m_wFlags52 & 0x100) && active != 0) {
        flow.x = mat->m_CurrentX * 0.007874016f;
        flow.y = mat->m_CurrentY * 0.007874016f;
        flow.z = mat->m_CurrentZ * 0.007874016f;
        strength = mat->m_CurrentStrength;
        spin = (strength + 1.0f) * 0.008726647f;
        CVector3__Scale(&flow, &flow, strength * 3.0f);
        flags = mat->m_wFlags52;
        if (flags & 0x600) {
            if (flags & 0x400) {
                spin = -spin;
            }
            if (actor->header.m_Type == 1) {
                actor->m_RotY += spin;
            }
        }
        if ((mat->m_wFlags52 & 0x2000) && actor->header.m_Type == 1) {
            rider = actor;
            flat = flow;
            flat.y = 0.0f;
            want = CVector3__GetYaw(&flat);
            cur = actor->m_RotY;
            if (want != cur) {
                Angle_Wrap(&cur);
                Angle_Wrap(&want);
                if (cur > want) {
                    wrapped = want + 6.2831855f;
                    if ((wrapped - cur) < (cur - want)) want = wrapped;
                } else {
                    wrapped = want - 6.2831855f;
                    if ((cur - wrapped) < (want - cur)) want = wrapped;
                }
                target = want;
                from = cur;
                if (from < target) {
                    cur = from + mat->m_CurrentStrength * sSurfaceCurrentRates[1] * gFrameIncrement * 0.017453294f;
                    if (target < cur) cur = target;
                } else {
                    cur = from - mat->m_CurrentStrength * sSurfaceCurrentRates[1] * gFrameIncrement * 0.017453294f;
                    if (cur < target) cur = target;
                }
                Angle_Wrap(&cur);
            }
            rider->m_RotY = cur;
        }
    } else {
        flow.x = 0.0f;
        flow.y = 0.0f;
        flow.z = 0.0f;
    }
    blend = mat->m_CurrentStrength * sSurfaceCurrentRates[0] * gFrameIncrement;
    blend = ((blend < 0.0f ? 0.0f : blend) > 1.0f ? 1.0f : (blend < 0.0f ? 0.0f : blend));
    current = &actor->m_vWaterDisplacement;
    CVector3__Lerp(current, blend, current, &flow);
    pos->x += actor->m_vWaterDisplacement.x;
    pos->z += actor->m_vWaterDisplacement.z;
    if (mat->m_wFlags52 & 0x100) {
        if (flow.y != 0.0f) {
            actor->m_vVelocity.y = 0.0f;
        } else {
            actor->m_vWaterDisplacement.y += (flow.y - actor->m_vWaterDisplacement.y) * 0.2f;
        }
        actor->m_vVelocity.y += 2.0f * (actor->m_vWaterDisplacement.y * 10.24f) + 4.0f;
        return;
    }
    actor->m_vWaterDisplacement.y += (flow.y - actor->m_vWaterDisplacement.y) * 0.2f;
}

#include "structs/collision.h"
#include "globals/collision.h"

s32 GameObject_CanEnterWater(CGameObjectInstance *actor);
s32 GameObject_CanDropDown(CGameObjectInstance *actor);
f32 CGameRegion__GetHeight(void *region, f32 x, f32 z);
s32 Collision_MoveInstance(CObjectHead *obj, CVector3 to, CCollisionInfo *info);

/* Slides an actor to (x, z) with collision, keeping its height above the floor. */
void GameObject_SlideTo(CGameObjectInstance *actor, f32 x, f32 z) {
    CVector3 savedVelocity;
    CVector3 to;
    s32 noA;
    s32 noB;
    s32 oldFlags;
    f32 height;

    noA = GameObject_CanEnterWater(actor) == 0;
    noB = GameObject_CanDropDown(actor) == 0;
    height = actor->header.m_vPos.y - CGameRegion__GetHeight(actor->header.m_pCurrentRegion, actor->header.m_vPos.x, actor->header.m_vPos.z);
    savedVelocity = actor->m_vVelocity;
    actor->m_vVelocity.x = actor->m_vVelocity.y = actor->m_vVelocity.z = 0.0f;
    to.x = x;
    to.y = actor->header.m_vPos.y;
    to.z = z;
    oldFlags = gCollisionInfoTestMoveLand.m_dwFlags;
    if (noA) {
        gCollisionInfoTestMoveLand.m_dwFlags = oldFlags & ~0x40000;
    } else {
        gCollisionInfoTestMoveLand.m_dwFlags = oldFlags | 0x40000;
    }
    if (noB) {
        gCollisionInfoTestMoveLand.m_dwFlags &= ~0x30000;
    } else {
        gCollisionInfoTestMoveLand.m_dwFlags |= 0x30000;
    }
    Collision_MoveInstance((CObjectHead *) actor, to, &gCollisionInfoTestMoveLand);
    gCollisionInfoTestMoveLand.m_dwFlags = oldFlags;
    actor->m_vVelocity = savedVelocity;
    actor->header.m_vPos.y = CGameRegion__GetHeight(actor->header.m_pCurrentRegion, actor->header.m_vPos.x, actor->header.m_vPos.z) + height;
}

#include "structs/vector.h"

s32 GameObject_CanClimbUp(CGameObjectInstance *actor);
s32 GameObject_CanLeaveWater(CGameObjectInstance *actor);

/* Tests moving an actor to pos with the collision settings that suit it, leaving it where it was; returns the collision result. */
s32 GameObject_TestMove(CGameObjectInstance *actor, CVector3 pos, CCollisionInfo **outInfo) {
    CObjectHead saved;
    CCollisionInfo *info;
    s32 alt;
    s32 hit;

    if (actor->header.m_Type == 1 && (actor->m_dwFlags & 0x300000)) {
        info = &gCollisionInfoTestMoveLand;
    } else {
        alt = GameObject_CanClimbUp(actor);
        if (!GameObject_CanEnterWater(actor)) {
            info = alt ? &gCollisionInfoTestMoveClimb : &gCollisionInfoTestMoveLand;
        } else {
            info = !GameObject_CanLeaveWater(actor) ? &gCollisionInfoTestMoveWater : &gCollisionInfoTestMove;
        }
    }
    if (outInfo != NULL) {
        *outInfo = info;
    }
    saved = *(CObjectHead *) actor;
    hit = Collision_MoveInstance((CObjectHead *) actor, pos, info);
    *(CObjectHead *) actor = saved;
    return hit;
}

f32 GameObject_GetCollisionHeight(CGameObjectInstance *actor);    /* height */

/* Whether actor can see target: a clear line from 80% of one's height to 80% of the other's. */
s32 GameObject_CanSee(CGameObjectInstance *actor, CGameObjectInstance *target) {
    CObjectHead saved;
    CVector3 to;
    s32 hit;

    saved = *(CObjectHead *) actor;
    actor->header.m_vPos.y += GameObject_GetCollisionHeight(actor) * 0.8f;
    to = target->header.m_vPos;
    to.y += GameObject_GetCollisionHeight(target) * 0.8f;
    hit = Collision_MoveInstance((CObjectHead *) actor, to, &gCollisionInfoSight);
    *(CObjectHead *) actor = saved;
    return hit == 0;
}

/* Whether obj can move to `to` without hitting anything; obj is left where it was. */
s32 GameObject_CanMoveTo(CObjectHead *obj, CVector3 to, CCollisionInfo *info) {
    CObjectHead saved;
    CCollisionInfo probe;
    s32 hit;

    saved = *obj;
    probe = *info;
    probe.m_WallBehavior = 1;
    probe.m_InstanceBehavior = 0;
    Collision_MoveInstance(obj, to, &probe);
    *obj = saved;
    hit = 0;
    if (gCollisionResults.m_pInstance != NULL || gCollisionResults.m_pWallRegion != NULL) {
        hit = 1;
    }
    return hit ^ 1;
}

#include "globals/scene.h"

s32 GameObject_GetFloorKind(CGameObjectInstance *obj);
void Sound__Play(s32 sound, CVector3 pos, sptr arg4, sptr arg5);
void CScene__BroadcastNoise(void *table, s32 arg1, CVector3 *pos);

void GameObject_PlayLandSound(CGameObjectInstance *obj) {
    s32 kind = GameObject_GetFloorKind(obj);
    s32 sound;

    if (kind == -1) {
        return;
    }
    switch (kind) {
        default:
        case 0:
        case 4:
        case 9:
            sound = 0x39F;
            break;
        case 1:
        case 5:
        case 6:
        case 7:
        case 8:
            sound = 0x3A1;
            break;
        case 2:
            sound = 0x39E;
            break;
        case 3:
            sound = 0x39D;
            break;
        case 10:
            sound = 0x3A0;
            break;
    }
    Sound__Play(sound, obj->header.m_vPos, 0, -1);
    CScene__BroadcastNoise(&gScene, 1, &obj->header.m_vPos);
}
