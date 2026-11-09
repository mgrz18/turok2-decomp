/* MotherBoss_TeleportTarget and MotherBoss_AnimEvent: one file. MotherBoss_AnimEvent's pool (0x800C061C)
 * starts 4 past an 8-byte boundary and holds a jump table, which its own object
 * would 8-align; the file's .rodata opens with MotherBoss_TeleportTarget's pool at 0x800C05F8.
 * MotherBoss_TeleportTarget is the first-module twin of src/us/code/MotherBoss_TeleportTarget_2.c.
 * FAKEMATCH (MotherBoss_AnimEvent): the actor count uses fewerActorsThan, the inline copy
 * of MotherBoss_FewerActorsThan with identical if/else arms in its loop (the same fake as
 * src/us/code/func_0040C4F4.c, func_0040A418.c and func_0040A8FC.c, where func_0040C4F4.c says why and what was
 * tried); the plain loop puts the counter in $v0 instead of $v1. */
#include "common.h"
#include "structs/game_object.h"
#include "structs/vector.h"
#include "structs/player.h"
#include "structs/boss.h"
#include "globals/boss.h"
#include "globals/scene.h"
#include "protos/ai.h"
#include "protos/game.h"

typedef struct {
    /* 0x00 */ CVector3 pos;
    /* 0x0C */ s32 type;
} AnimEvent;

typedef struct {
    s16 a;
    s16 b;
    u8 c[4];
} __attribute__((aligned(4))) Part;

extern void AI_UpdateTargets(CListedInstance *actor, void *ai, CAITargets *t, s32 f1, s32 f2);
extern f32 Random_Blend(f32 min, f32 max);
extern void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b); /* out = a - b */
extern f32 func_00298470(f32 x); /* sqrt */
extern f32 func_002982F0(f32 angle); /* sin */
extern f32 func_002974C0(f32 angle); /* cos */
extern void Actor_SpawnParticle(CListedInstance *actor, s32 id, CVector3 pos, s32 a5);

static __inline__ void FindTarget(CListedInstance *obj, CListedInstance **target) {
    CAITargets targets;

    *target = NULL;
    AI_UpdateTargets(obj, (u8 *)obj + 0x140, &targets, 1, 0);
    if (targets.m_Path.m_pTarget != NULL && (targets.m_Path.m_pTarget->m_dwFlags & 0x300000)) {
        *target = (CListedInstance *) targets.m_Path.m_pTarget;
    }
}

static __inline__ f32 DistXZ(CVector3 a, CVector3 *b) {
    CVector3 d;

    a.y = 0.0f;
    CVector3__Subtract(&d, &a, b);
    return func_00298470(d.x * d.x + d.y * d.y + d.z * d.z);
}

/* Teleports the current target (when it is a flagged actor) to a random spot: around the spawn point
 * nearest to it (within 460.8) inside that point's angle range, else around itself; retried until the spot
 * is at least 460.8 away from every spawn point. */
void MotherBoss_TeleportTarget(CListedInstance *obj, s32 nearSelf) {
    CVector3 p;
    CListedInstance *target;
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
        CVector3__Subtract(&p, &target->m_Instance.header.m_vPos, &gMotherSpawnPoints[i].pos);
        if (func_00298470(p.x * p.x + p.y * p.y + p.z * p.z) < 460.8f) {
            idx = i;
        }
    }
    if (retry) {
        height = 1024.0f;
        do {
            if (idx >= 0) {
                baseX = gMotherSpawnPoints[idx].pos.x;
                baseZ = gMotherSpawnPoints[idx].pos.z;
                r = Random_Blend(460.8f, 921.6f);
                angle = Random_Blend(gMotherSpawnPoints[idx].minAngle, gMotherSpawnPoints[idx].maxAngle);
            } else {
                baseX = target->m_Instance.header.m_vPos.x;
                baseZ = target->m_Instance.header.m_vPos.z;
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
                if (DistXZ(p, &gMotherSpawnPoints[i].pos) < 460.8f) {
                    retry = 1;
                }
            }
        } while (retry);
    }
    Actor_SpawnParticle(target, 0x112, p, 0);
}

extern void GameObject_TraceMove(void *obj, CVector3 in, CVector3 *out, s32 *region, s32 a, s32 b);
extern void ImpactEvent_Send(void *src, void *dst, s32 kind, s32 id, CVector3 pos, Part part);
extern void CVector3__Normalize(CVector3 *v);
extern void CVector3__Scale(CVector3 *out, CVector3 *a, f32 s);
extern void CVector3__Add(CVector3 *out, CVector3 *a, CVector3 *b);

/* MotherBoss_FewerActorsThan */
static __inline__ s32 fewerActorsThan(s32 max) {
    CListedInstance *actor;
    s32 count = 0;

    for (actor = gScene.m_InstancePool.m_ActiveList.m_pHead; actor != NULL; actor = actor->m_pNext) {
        /* both arms are merged by cross-jumping; the extra block gives the ROM's register allocation */
        if (actor->m_pNext != NULL) {
            count++;
        } else {
            count++;
        }
    }
    return count < max;
}

/* MotherBoss_GetTargetDist */
static __inline__ f32 getTargetDist(CListedInstance *obj, CListedInstance **found) {
    CAITargets t;
    f32 dist;

    dist = -1.0f;
    *found = NULL;
    AI_UpdateTargets(obj, &obj->m_Instance.m_AI, &t, 1, 0);
    if (t.m_Path.m_pTarget != NULL && (t.m_Path.m_pTarget->m_dwFlags & 0x300000)) {
        dist = t.m_Path.m_FlatDist;
        *found = (CListedInstance *) t.m_Path.m_pTarget;
    }
    return dist;
}

/* Boss animation event callback: by event, spawns a minion (fewer than 3
 * actors), hits the target's owner with a shock wave when close, throws the
 * live minions around the target, or knocks the target back. 1228.7999f: the
 * ROM's pool word is 0x44999999, one ulp under 1228.8f. */
void MotherBoss_AnimEvent(s32 arg0, void *self, s32 arg2, AnimEvent ev) {
    MotherBossState *s = &gMotherBoss;
    CListedInstance *obj = self;
    CListedInstance *found;
    CPlayer *owner;
    CListedInstance *actor;
    f32 dist;
    s32 i;
    sptr region;
    CVector3 scale;
    f32 r;
    f32 angle;
    f32 dx;
    f32 dz;
    f32 x;
    f32 z;

    dist = getTargetDist(obj, &found);
    owner = NULL;
    if (found != NULL) {
        owner = (CPlayer *) found->m_Instance.m_AI.m_pParent;
    }
    switch (ev.type) {
    case 0:
        if (fewerActorsThan(3) && (s->phase == 2 || 1228.7999f < dist)) {
            GameObject_TraceMove(self, ev.pos, &ev.pos, &region, 0, 0);
            scale.x = 0.5f;
            scale.y = 0.5f;
            scale.z = 0.5f;
            CInstancePool__Spawn(&gScene.m_InstancePool, -1, 0x458, s->intel, region, ev.pos, 0.0f, obj->m_Instance.m_vVelocity, scale, NULL);
        }
        break;
    case 1:
        if (found != NULL && owner != NULL && dist < 1228.7999f && owner->m_Mode != 5) {
            Part part;

            part.a = 0xA00;
            part.b = 0x500;
            part.c[0] = part.c[1] = part.c[2] = part.c[3] = 0;
            ImpactEvent_Send(obj, obj, 3, -1, owner->m_CharacterActor.header.m_vPos, part);
        }
        break;
    case 2:
        if (found != NULL && owner != NULL) {
            for (i = 0; i < s->partsLeft; i++) {
                actor = (CGameObjectInstance *)s->parts[i];
                r = Random_Blend(153.59999f, 256.0f);
                angle = found->m_Instance.m_RotY + 3.1415927f + Random_Blend(-0.5235988f, 0.5235988f);
                x = owner->m_CharacterActor.header.m_vPos.x;
                z = owner->m_CharacterActor.header.m_vPos.z;
                dx = r * func_002982F0(angle);
                dz = r * func_002974C0(angle);
                x += dx;
                z += dz;
                actor->m_Instance.header.m_vPos.y = Random_Blend(-204.79999f, -51.199997f);
                actor->m_Instance.header.m_vPos.x = x;
                actor->m_Instance.header.m_vPos.z = z;
                CAI__SetMode(actor, &actor->m_Instance.m_AI, 1);
            }
        }
        break;
    case 3:
        if (found != NULL && owner != NULL) {
            Part part;

            part.a = 0xA00;
            part.b = 0xA00;
            part.c[0] = part.c[1] = part.c[2] = part.c[3] = 0;
            ImpactEvent_Send(obj, obj, 3, -1, owner->m_CharacterActor.header.m_vPos, part);
        }
        break;
    case 4:
        if (found != NULL && owner != NULL && dist < 1433.6f) {
            Part part;

            part.a = 0xA00;
            part.b = 0x500;
            part.c[0] = part.c[1] = part.c[2] = part.c[3] = 0;
            ImpactEvent_Send(obj, obj, 3, -1, owner->m_CharacterActor.header.m_vPos, part);
        }
        break;
    case 5:
        if (found != NULL) {
            CVector3 push;

            CVector3__Subtract(&push, &obj->m_Instance.header.m_vPos, &found->m_Instance.header.m_vPos);
            CVector3__Normalize(&push);
            CVector3__Scale(&push, &push, dist);
            CVector3__Add(&found->m_Instance.m_vVelocity, &found->m_Instance.m_vVelocity, &push);
        }
        break;
    }
}
