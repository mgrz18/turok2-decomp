/* func_00409A64 and func_00409D4C: one file. func_00409D4C's pool (0x800C061C)
 * starts 4 past an 8-byte boundary and holds a jump table, which its own object
 * would 8-align; the file's .rodata opens with func_00409A64's pool at 0x800C05F8.
 * func_00409A64 is the first-module twin of src/us/code/func_00442A64.c.
 * FAKEMATCH (func_00409D4C): the actor count uses fewerActorsThan, the inline copy
 * of func_0040C4F4 with identical if/else arms in its loop (the same fake as
 * src/us/code/func_0040C4F4.c, func_0040A418.c and func_0040A8FC.c); the plain
 * loop puts the counter in $v0 instead of $v1. */
#include "common.h"
#include "structs/game_object.h"
#include "structs/vector.h"
#include "structs/player.h"
#include "structs/boss.h"

typedef struct {
    /* 0x00 */ CVector3 pos;
    /* 0x0C */ f32 minAngle;
    /* 0x10 */ f32 maxAngle;
} SpawnPoint;

typedef struct {
    /* 0x00 */ CVector3 pos;
    /* 0x0C */ s32 type;
} AnimEvent;

typedef struct {
    s16 a;
    s16 b;
    u8 c[4];
} __attribute__((aligned(4))) Part;


extern SpawnPoint D_00431D20[4];
extern void func_00246108(CListedInstance *actor, void *ai, CAITargets *t, s32 f1, s32 f2);
extern f32 func_002119FC(f32 min, f32 max);
extern void func_0020EF2C(CVector3 *out, CVector3 *a, CVector3 *b); /* out = a - b */
extern f32 func_00298470(f32 x); /* sqrt */
extern f32 func_002982F0(f32 angle); /* sin */
extern f32 func_002974C0(f32 angle); /* cos */
extern void func_00246690(CListedInstance *actor, s32 id, CVector3 pos, s32 a5);

static __inline__ void FindTarget(CListedInstance *obj, CListedInstance **target) {
    CAITargets targets;

    *target = NULL;
    func_00246108(obj, (u8 *)obj + 0x140, &targets, 1, 0);
    if (targets.m_Path.m_pTarget != NULL && (targets.m_Path.m_pTarget->m_dwFlags & 0x300000)) {
        *target = (CListedInstance *) targets.m_Path.m_pTarget;
    }
}

static __inline__ f32 DistXZ(CVector3 a, CVector3 *b) {
    CVector3 d;

    a.y = 0.0f;
    func_0020EF2C(&d, &a, b);
    return func_00298470(d.x * d.x + d.y * d.y + d.z * d.z);
}

/* Teleports the current target (when it is a flagged actor) to a random spot: around the spawn point
 * nearest to it (within 460.8) inside that point's angle range, else around itself; retried until the spot
 * is at least 460.8 away from every spawn point. */
void func_00409A64(CListedInstance *obj, s32 nearSelf) {
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
        func_0020EF2C(&p, &target->m_Instance.header.m_vPos, &D_00431D20[i].pos);
        if (func_00298470(p.x * p.x + p.y * p.y + p.z * p.z) < 460.8f) {
            idx = i;
        }
    }
    if (retry) {
        height = 1024.0f;
        do {
            if (idx >= 0) {
                baseX = D_00431D20[idx].pos.x;
                baseZ = D_00431D20[idx].pos.z;
                r = func_002119FC(460.8f, 921.6f);
                angle = func_002119FC(D_00431D20[idx].minAngle, D_00431D20[idx].maxAngle);
            } else {
                baseX = target->m_Instance.header.m_vPos.x;
                baseZ = target->m_Instance.header.m_vPos.z;
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
                if (DistXZ(p, &D_00431D20[i].pos) < 460.8f) {
                    retry = 1;
                }
            }
        } while (retry);
    }
    func_00246690(target, 0x112, p, 0);
}

extern CListedInstance *D_8010FEB4;
extern u8 D_8010ABA0[];

extern void func_002688C8(void *obj, CVector3 in, CVector3 *out, s32 *region, s32 a, s32 b);
extern void *func_00228108(void *world, s32 model, s32 type, s32 intelligence, s32 region, CVector3 pos, f32 rotY,
                           CVector3 vel, CVector3 scale, s32 *owner);
extern void func_00236EA0(void *src, void *dst, s32 kind, s32 id, CVector3 pos, Part part);
extern void func_00243414(CListedInstance *obj, void *ai, s32 state);
extern void func_0020F040(CVector3 *v);
extern void func_0020EF60(CVector3 *out, CVector3 *a, f32 s);
extern void func_0020EEF8(CVector3 *out, CVector3 *a, CVector3 *b);

/* func_0040C4F4 */
static __inline__ s32 fewerActorsThan(s32 max) {
    CListedInstance *actor;
    s32 count = 0;

    for (actor = D_8010FEB4; actor != NULL; actor = actor->m_pNext) {
        /* both arms are merged by cross-jumping; the extra block gives the ROM's register allocation */
        if (actor->m_pNext != NULL) {
            count++;
        } else {
            count++;
        }
    }
    return count < max;
}

/* func_0040BD60 */
static __inline__ f32 getTargetDist(CListedInstance *obj, CListedInstance **found) {
    CAITargets t;
    f32 dist;

    dist = -1.0f;
    *found = NULL;
    func_00246108(obj, &obj->m_Instance.m_AI, &t, 1, 0);
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
void func_00409D4C(s32 arg0, void *self, s32 arg2, AnimEvent ev) {
    MotherBossState *s = &D_8012FB70;
    CListedInstance *obj = self;
    CListedInstance *found;
    CPlayer *owner;
    CListedInstance *actor;
    f32 dist;
    s32 i;
    s32 region;
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
            func_002688C8(self, ev.pos, &ev.pos, &region, 0, 0);
            scale.x = 0.5f;
            scale.y = 0.5f;
            scale.z = 0.5f;
            func_00228108(D_8010ABA0, -1, 0x458, (s32)s->intel, region, ev.pos, 0.0f, obj->m_Instance.m_vVelocity, scale, NULL);
        }
        break;
    case 1:
        if (found != NULL && owner != NULL && dist < 1228.7999f && owner->m_Mode != 5) {
            Part part;

            part.a = 0xA00;
            part.b = 0x500;
            part.c[0] = part.c[1] = part.c[2] = part.c[3] = 0;
            func_00236EA0(obj, obj, 3, -1, owner->m_CharacterActor.header.m_vPos, part);
        }
        break;
    case 2:
        if (found != NULL && owner != NULL) {
            for (i = 0; i < s->partsLeft; i++) {
                actor = (CGameObjectInstance *)s->parts[i];
                r = func_002119FC(153.59999f, 256.0f);
                angle = found->m_Instance.m_RotY + 3.1415927f + func_002119FC(-0.5235988f, 0.5235988f);
                x = owner->m_CharacterActor.header.m_vPos.x;
                z = owner->m_CharacterActor.header.m_vPos.z;
                dx = r * func_002982F0(angle);
                dz = r * func_002974C0(angle);
                x += dx;
                z += dz;
                actor->m_Instance.header.m_vPos.y = func_002119FC(-204.79999f, -51.199997f);
                actor->m_Instance.header.m_vPos.x = x;
                actor->m_Instance.header.m_vPos.z = z;
                func_00243414(actor, &actor->m_Instance.m_AI, 1);
            }
        }
        break;
    case 3:
        if (found != NULL && owner != NULL) {
            Part part;

            part.a = 0xA00;
            part.b = 0xA00;
            part.c[0] = part.c[1] = part.c[2] = part.c[3] = 0;
            func_00236EA0(obj, obj, 3, -1, owner->m_CharacterActor.header.m_vPos, part);
        }
        break;
    case 4:
        if (found != NULL && owner != NULL && dist < 1433.6f) {
            Part part;

            part.a = 0xA00;
            part.b = 0x500;
            part.c[0] = part.c[1] = part.c[2] = part.c[3] = 0;
            func_00236EA0(obj, obj, 3, -1, owner->m_CharacterActor.header.m_vPos, part);
        }
        break;
    case 5:
        if (found != NULL) {
            CVector3 push;

            func_0020EF2C(&push, &obj->m_Instance.header.m_vPos, &found->m_Instance.header.m_vPos);
            func_0020F040(&push);
            func_0020EF60(&push, &push, dist);
            func_0020EEF8(&found->m_Instance.m_vVelocity, &found->m_Instance.m_vVelocity, &push);
        }
        break;
    }
}
