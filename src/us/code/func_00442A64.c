#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "globals/boss.h"

/* Teleports the current target (when it is a flagged actor) to a random spot: around the spawn point
 * nearest to it (within 460.8) inside that point's angle range, else around itself; retried until the spot
 * is at least 460.8 away from every spawn point. */

extern void AI_UpdateTargets(CGameObjectInstance *actor, void *ai, CAITargets *t, s32 f1, s32 f2);
extern f32 Random_Blend(f32 min, f32 max);
extern void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b); /* out = a - b */
extern f32 func_00298470(f32 x); /* sqrt */
extern f32 func_002982F0(f32 angle); /* sin */
extern f32 func_002974C0(f32 angle); /* cos */
extern void Actor_SpawnParticle(CGameObjectInstance *actor, s32 id, CVector3 pos, s32 a5);

static __inline__ void FindTarget(CGameObjectInstance *obj, CGameObjectInstance **target) {
    CAITargets targets;

    *target = NULL;
#ifdef TARGET_PORT
    AI_UpdateTargets(obj, &obj->m_AI, &targets, 1, 0);
#else
    AI_UpdateTargets(obj, (u8 *)obj + 0x140, &targets, 1, 0);
#endif
    if (targets.m_Path.m_pTarget != NULL && (targets.m_Path.m_pTarget->m_dwFlags & 0x300000)) {
        *target = targets.m_Path.m_pTarget;
    }
}

static __inline__ f32 DistXZ(CVector3 a, CVector3 *b) {
    CVector3 d;

    a.y = 0.0f;
    CVector3__Subtract(&d, &a, b);
    return func_00298470(d.x * d.x + d.y * d.y + d.z * d.z);
}

void MotherBoss_TeleportTarget_2(CGameObjectInstance *obj, s32 nearSelf) {
    CVector3 p;
    CGameObjectInstance *target;
    s32 retry;
    s32 idx;
    s32 i;
    f32 baseX;
    f32 baseZ;
    f32 r;
    f32 angle;
    f32 dx;
    f32 dz;
    f32 height;

    retry = 1;
    FindTarget(obj, &target);
    if (target == NULL) {
        return;
    }
    i = 0;
    idx = -1;
    for (; i < 4; i++) {
        if (idx >= 0) {
            break;
        }
        CVector3__Subtract(&p, &target->header.m_vPos, &gMotherSpawnPoints_2[i].pos);
        if (func_00298470(p.x * p.x + p.y * p.y + p.z * p.z) < 460.8f) {
            idx = i;
        }
    }
    if (retry) {
        height = 1024.0f;
        do {
            if (idx >= 0) {
                baseX = gMotherSpawnPoints_2[idx].pos.x;
                baseZ = gMotherSpawnPoints_2[idx].pos.z;
                r = Random_Blend(460.8f, 921.6f);
                angle = Random_Blend(gMotherSpawnPoints_2[idx].minAngle, gMotherSpawnPoints_2[idx].maxAngle);
            } else {
                baseX = target->header.m_vPos.x;
                baseZ = target->header.m_vPos.z;
                if (nearSelf) {
                    r = 0.0f;
                    angle = 0.0f;
                } else {
                    r = Random_Blend(204.79999f, 921.6f);
                    angle = Random_Blend(-3.1415927f, 3.1415927f);
                }
            }
            dx = r * func_002982F0(angle);
            dz = r * func_002974C0(angle);
            retry = 0;
            baseX += dx;
            baseZ += dz;
            p.x = baseX;
            p.y = height;
            p.z = baseZ;
            for (i = 0; i < 4 && !retry; i++) {
                if (DistXZ(p, &gMotherSpawnPoints_2[i].pos) < 460.8f) {
                    retry = 1;
                }
            }
        } while (retry);
    }
    Actor_SpawnParticle(target, 0x112, p, 0);
}
