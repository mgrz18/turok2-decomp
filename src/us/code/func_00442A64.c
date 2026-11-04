#include "common.h"
#include "structs/game_object.h"

/* Teleports the current target (when it is a flagged actor) to a random spot: around the spawn point
 * nearest to it (within 460.8) inside that point's angle range, else around itself; retried until the spot
 * is at least 460.8 away from every spawn point. */

typedef struct {
    /* 0x00 */ CVector3 pos;
    /* 0x0C */ f32 minAngle;
    /* 0x10 */ f32 maxAngle;
} SpawnPoint;

extern SpawnPoint D_00431C40[4];
extern void func_00246108(CGameObjectInstance *actor, void *ai, CAITargets *t, s32 f1, s32 f2);
extern f32 func_002119FC(f32 min, f32 max);
extern void func_0020EF2C(CVector3 *out, CVector3 *a, CVector3 *b); /* out = a - b */
extern f32 func_00298470(f32 x); /* sqrt */
extern f32 func_002982F0(f32 angle); /* sin */
extern f32 func_002974C0(f32 angle); /* cos */
extern void func_00246690(CGameObjectInstance *actor, s32 id, CVector3 pos, s32 a5);

static __inline__ void FindTarget(CGameObjectInstance *obj, CGameObjectInstance **target) {
    CAITargets targets;

    *target = NULL;
    func_00246108(obj, (u8 *)obj + 0x140, &targets, 1, 0);
    if (targets.m_Path.m_pTarget != NULL && (targets.m_Path.m_pTarget->m_dwFlags & 0x300000)) {
        *target = targets.m_Path.m_pTarget;
    }
}

static __inline__ f32 DistXZ(CVector3 a, CVector3 *b) {
    CVector3 d;

    a.y = 0.0f;
    func_0020EF2C(&d, &a, b);
    return func_00298470(d.x * d.x + d.y * d.y + d.z * d.z);
}

void func_00442A64(CGameObjectInstance *obj, s32 nearSelf) {
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
        func_0020EF2C(&p, &target->header.m_vPos, &D_00431C40[i].pos);
        if (func_00298470(p.x * p.x + p.y * p.y + p.z * p.z) < 460.8f) {
            idx = i;
        }
    }
    if (retry) {
        height = 1024.0f;
        do {
            if (idx >= 0) {
                baseX = D_00431C40[idx].pos.x;
                baseZ = D_00431C40[idx].pos.z;
                r = func_002119FC(460.8f, 921.6f);
                angle = func_002119FC(D_00431C40[idx].minAngle, D_00431C40[idx].maxAngle);
            } else {
                baseX = target->header.m_vPos.x;
                baseZ = target->header.m_vPos.z;
                if (nearSelf) {
                    r = 0.0f;
                    angle = 0.0f;
                } else {
                    r = func_002119FC(204.79999f, 921.6f);
                    angle = func_002119FC(-3.1415927f, 3.1415927f);
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
                if (DistXZ(p, &D_00431C40[i].pos) < 460.8f) {
                    retry = 1;
                }
            }
        } while (retry);
    }
    func_00246690(target, 0x112, p, 0);
}
