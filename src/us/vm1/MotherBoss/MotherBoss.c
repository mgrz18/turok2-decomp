/* 0x00409000-0x00409340. splat started it at 0x00408FF8, two junk words before
 * it (src/us/asm/stray_00408FF8.s); MotherBoss_Init installs func_00408FF8 + 8,
 * this function, as the AI hit callback. */
#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "structs/collision.h"
#include "globals/boss.h"
#include "protos/ai.h"
#include "common.h"
#include "globals/scene.h"
#include "globals/boss_ai.h"
#include "protos/scene.h"
#include "globals/collision.h"
/* First-module copy of MotherBoss_DropPickupsAtSpawnPoints_2 (src/us/code/MotherBoss_DropPickupsAtSpawnPoints_2.c). */
#include "common.h"
#include "structs/vector.h"
#include "structs/level_actor.h"
/* MotherBoss_TeleportTarget and MotherBoss_AnimEvent: one file. MotherBoss_AnimEvent's pool (0x800C061C)
 * starts 4 past an 8-byte boundary and holds a jump table, which its own object
 * would 8-align; the file's .rodata opens with MotherBoss_TeleportTarget's pool at 0x800C05F8.
 * MotherBoss_TeleportTarget is the first-module twin of src/us/code/MotherBoss_TeleportTarget_2.c.
 * FAKEMATCH (MotherBoss_AnimEvent): the actor count uses fewerActorsThan, the inline copy
 * of MotherBoss_FewerActorsThan with identical if/else arms in its loop (the same fake as
 * src/us/vm1/MotherBoss/func_0040C4F4.c, func_0040A418.c and func_0040A8FC.c, where func_0040C4F4.c says why and what was
 * tried); the plain loop puts the counter in $v0 instead of $v1. */
#include "common.h"
#include "structs/player.h"
#include "protos/game.h"
#include "structs/pick.h"
#include "globals/engine.h"
/* FAKEMATCH: identical if/else arms in fewerActorsThan's counting loop (as in MotherBoss_FewerActorsThan,
 * src/us/vm1/MotherBoss/func_0040C4F4.c, which says why and what was tried). */
#include "common.h"
/* MATCHES at its real extent (checked with the symbol size patched to 0x20C); both flag masks are
 * computed before the tests (found with the permuter). */
/* WRONG CUT: real function 0x0040A6F0-0x0040A8FC (size 0x20C); splat split it at its tail jumps into
 * MotherBoss_Phase1ChooseAttack, func_0040A7AC, func_0040A8A0 and func_0040A8A4. */
#include "common.h"
/* FAKEMATCH: identical if/else arms in fewerActorsThan's counting loop (as in MotherBoss_FewerActorsThan,
 * src/us/vm1/MotherBoss/func_0040C4F4.c, which says why and what was tried); the plain
 * loop shifts its registers by one. */
#include "common.h"
/* gMotherBoss_phase/D0/D4/EC are fields of gMotherBoss (phase, activeMask, unk664, counter) that the ROM addresses
 * through their own symbols (as arrays: their loads are reloaded after the ai store). */
#include "common.h"
#include "m2c_macros.h"
#include "structs/game.h"
#include "globals/game.h"
#include "structs/light.h"
#include "structs/model.h"
#include "protos/ai.h" /* CAI__CallHurt(void *, void *, void *): called through CAIHurtFn with its own argument types */
/* FAKEMATCH: identical if/else arms in the counting loop (found by the permuter); the plain loop puts the
 * counter in $v0 instead of $a1. Why it works: the arms' test loads m_pNext into a temporary that lives in
 * one block, local-alloc gives it $v0, so every value live across the loop conflicts with $v0 and the
 * counter falls to $a1; cross-jumping merges the arms only after reload, and the dead load goes with them.
 * Natural forms tried in the L pass (all give the counter $v0): for/while/do-while/comma-for loops, a `next`
 * pointer, a break on the last actor, an inline count function, `count < max ? 1 : 0`, if/return pairs,
 * `register` locals, `max` copied to a local, a ternary next, and a dead second counter or `last` pointer
 * (deleted before allocation, so they leave no temporary). Only arms that survive to reload do it. */
#include "common.h"
#include "protos/player.h"
#include "structs/twin_motherboss.h"

void MotherBoss_StartSpotAnims(void *obj, void *src, s32 mask);

void MotherBoss_Hurt(void *obj, CAI *ai, DamageEvent *hit) {
    MotherBossState *s;
    s32 mask;
    s32 spot;
    s32 i;
    s32 kind;
    s32 fx;
    s32 target;
    s32 damage;
    s32 bit;

#ifdef TARGET_PORT
    {
        extern void T2Port_LogBossHurt(const char *who, void *obj, void *hit); /* DEBUG-ONLY */
        T2Port_LogBossHurt("mother", obj, hit);
    }
#endif
    if (hit->frame == NULL) {
        return;
    }
    i = 0;
    spot = -1;
    s = &gMotherBoss;
    mask = s->activeMask & hit->frame->unk_6C;
    for (; i < 15; i++) {
        if (spot >= 0) {
            break;
        }
        bit = 1 << i;
        if (bit & mask) {
            spot = i;
        }
    }
    switch (spot) {
    case 10:
    case 11:
        kind = 0;
        fx = 0xC00;
        break;
    case 12:
    case 14:
        kind = 1;
        fx = 0x5000;
        break;
    case 13:
        kind = 2;
        fx = 0x2000;
        break;
    default:
        return;
    }
    target = -1;
    switch (s->phase) {
    case 0:
        if (kind == 0) {
            if (s->slots[2].health > 0) {
                target = 2;
            } else if (s->slots[3].health > 0) {
                target = 3;
            } else if (s->slots[4].health > 0) {
                target = 4;
            }
        } else if (kind == 1) {
            if (s->slots[6].health > 0) {
                target = 6;
            } else if (s->slots[7].health > 0) {
                target = 7;
            } else if (s->slots[8].health > 0) {
                target = 8;
            }
        }
        break;
    case 1:
        if (kind == 0) {
            if (s->slots[1].health > 0) {
                target = 1;
            }
        } else if (kind == 1) {
            if (s->slots[5].health > 0) {
                target = 5;
            }
        }
        break;
    case 2:
        if (kind == 2) {
            if (s->slots[13].health > 0) {
                target = 13;
            }
        }
        break;
    }
    if (target == -1) {
        return;
    }
    damage = s->slots[target].health < hit->damage ? (s->slots + target)->health : hit->damage;
    s->slots[target].health -= damage;
    s->health -= damage;
    MotherBoss_StartSpotAnims(obj, sMotherBossHurtScript, fx);
    if (s->slots[target].health > 0) {
        return;
    }
    if (s->phase == 0) {
        s->partsLeft--;
        switch (kind) {
        case 0:
            CAI__SetMode(obj, ai, 7);
            break;
        case 1:
            CAI__SetMode(obj, ai, 8);
            break;
        }
    }
    bit = 1 << target;
    if (s->phase != 2) {
        ai->m_VisBits &= ~bit;
    }
    s->liveMask &= ~bit;
    if (s->liveMask != 0) {
        return;
    }
    switch (s->phase) {
    case 0:
        CAI__SetMode(obj, ai, 9);
        break;
    case 1:
        CAI__SetMode(obj, ai, 0x13);
        break;
    case 2:
        CAI__SetMode(obj, ai, 0x1E);
        break;
    }
}

/* Boss fight setup: finds the six level actors with id 0x1DB1 and the one
 * with 0x1DB2, installs the AI callbacks and resets the 15 attack slots. */



void ScriptAnimator_Start(void *, CGameObjectInstance *);
void MotherBoss_Step(void *obj, CAI *ai);
void MotherBoss_Draw(CGameObjectInstance *actor, void *ai, CDrawArgs *req);
void MotherBoss_Collision(CGameObjectInstance *obj, CAI *ai);
void EnemyAI_HeadNodeCallback();
extern u8 func_00408FF8[];

void MotherBoss_Init(CGameObjectInstance *obj, CAI *ai) {
    MotherBossState *s = &gMotherBoss;
    s32 i;

    gScene_m_LevelID = 0x1DB1;
    s->pickupTimer = 0;
    s->dropTimer = 0;
    s->partCount = CScene__FindInstances(&gScene, 0x650, 10, 0x1DB1, (CGameObjectInstance **)s->parts, 6);
    for (i = 0; i < s->partCount; i++) {
        ((CGameObjectInstance *)s->parts[i])->m_AI.m_Mother.m_bActive = 1;
    }
    s->partsLeft = s->partCount;
    CScene__FindInstances(&gScene, 0x650, 10, 0x1DB2, (CGameObjectInstance **)&s->controller, 1);
    ((CGameObjectInstance *)s->controller)->m_AI.m_Mother.m_bActive = 1;
    s->intel = (void *)CScene__FindIntelligence(&gScene, 1, 0x1DB1);
    ai->m_pModeTable = sMotherBossModeTable;
    ai->m_pfStep = MotherBoss_Step;
#ifdef TARGET_PORT
    /* func_00408FF8 + 8 is MotherBoss_Hurt in the ROM (splat started its object two junk words
     * early); on the host func_00408FF8 is not code at that distance: the first hit called data */
    ai->m_pfHurt = MotherBoss_Hurt;
#else
    ai->m_pfHurt = func_00408FF8 + 8;
#endif
    ai->m_pfEffects = EnemyAI_HeadNodeCallback;
    ai->m_pfCollision = MotherBoss_Collision;
    ai->m_pfDraw = MotherBoss_Draw;
    CAI__SetMode(obj, ai, 0);
    gMotherBoss.obj = obj;
    gCollisionInfoGravity.m_dwFlags &= ~0x300;
    obj->m_dwFlags |= 0x04000000;
    for (i = 0; i < 15; i++) {
        ScriptAnimator_Start(&gMotherBoss.slots[i].anim, obj);
    }
}
f32 MotherBoss_GetTargetDist(CGameObjectInstance *obj, CGameObjectInstance **found);
extern s16 WeightedTable_PickStatic(void *table); /* ABI: returns s16 (the host leaves the upper bits of the return register undefined) */
extern f32 Random_Blend(f32 min, f32 max);
extern void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b); /* out = a - b */
extern f32 func_00298470(f32 x); /* sqrt */
extern void *CLevelActorPool__Spawn(CLevelActorPool *pool, void *refCount, s32 id, CVector3 vel, CVector3 pos, sptr region, f32 lifetime);
extern void Actor_SpawnParticle(void *actor, s32 id, CVector3 pos, s32 a5);

static __inline__ void SpawnAt(s32 arg0, s16 type, CVector3 pos, f32 speed) {
    CVector3 vel;
    sptr region;
    void *actor;

    region = CScene__FindRegionAt(&gScene, &pos);
    vel.x = 0.0f;
    vel.y = speed;
    vel.z = 0.0f;
    actor = CLevelActorPool__Spawn(&gScene.m_ActorPool, 0, type, vel, pos, region, arg0);
    if (actor != NULL) {
        Actor_SpawnParticle(actor, 0x11D, pos, 0);
    }
}

void MotherBoss_DropPickupsAtSpawnPoints(s32 arg0, void *arg1) {
    CVector3 v;
    CGameObjectInstance *found;
    s32 i;
    s32 type;
    s32 dx;
    s32 dz;

    MotherBoss_GetTargetDist(gMotherBoss.obj, &found);
    for (i = 0; i < 4; i++) {
        type = (s16)WeightedTable_PickStatic(arg1);
        dx = Random_Blend(-15.0f, 15.0f);
        dz = Random_Blend(-15.0f, 15.0f);
        dx = dx * 10.24f;
        dz = dz * 10.24f;
        if (found != NULL) {
            CVector3__Subtract(&v, &found->header.m_vPos, &gMotherSpawnPoints[i].pos);
            if (func_00298470(v.x * v.x + v.y * v.y + v.z * v.z) > 460.8f) {
                v = gMotherSpawnPoints[i].pos;
                v.y = 30.72f;
                v.x += dx;
                v.z += dz;
                SpawnAt(arg0, type, v, 512.0f);
            }
        } else {
            v = gMotherSpawnPoints[i].pos;
            v.y = 30.72f;
            v.x += dx;
            v.z += dz;
            SpawnAt(arg0, type, v, 512.0f);
        }
    }
}

/* Boss fight tick: updates the 15 attack slots, then (outside cinematics)
 * counts up the two timers: by phase, drops a random pickup somewhere in the
 * arena (inlined MotherBoss_SpawnPickup) and, later, a pickup at every spawn point
 * (MotherBoss_DropPickupsAtSpawnPoints). */

extern void Animator_Update(void *slot);
extern s32 Cinema_PathPlaying(void);
extern f32 Random_Blend(f32 lo, f32 hi);
extern f32 func_002982F0(f32);
extern f32 func_002974C0(f32);

/* MotherBoss_SpawnPickup */
static __inline__ void spawnPickup(s32 id, s32 lifetime, CVector3 pos) {
    CVector3 vel;
    sptr region;
    CPooledLevelActor *pickup;

    region = CScene__FindRegionAt(&gScene, &pos);
    vel.x = 0.0f;
    vel.y = 512.0f;
    vel.z = 0.0f;
    pickup = CLevelActorPool__Spawn(&gScene.m_ActorPool, NULL, id, vel, pos, region, lifetime);
    if (pickup != NULL) {
        Actor_SpawnParticle(pickup, 0x11D, pos, 0);
    }
}

void MotherBoss_UpdateSpotsAndPickups(MotherBossState *s) {
    CVector3 pos;
    s32 i;
    s32 spawn;
    s32 lifetime;
    s32 drop;
    s32 dropLifetime;
    void *table;
    s32 id;
    f32 dist;
    f32 angle;

    spawn = 0;
    drop = 0;
    lifetime = 0;
    dropLifetime = 0;
    table = NULL;
    for (i = 0; i < 15; i++) {
        Animator_Update(&s->slots[i].anim);
    }
    if (Cinema_PathPlaying()) {
        return;
    }
    s->pickupTimer++;
    s->dropTimer++;
    switch (s->phase) {
    case 1:
        if (s->pickupTimer >= 76) {
            spawn = 1;
            lifetime = 600;
        }
        if (s->dropTimer >= 526) {
            drop = 1;
            table = sMotherBossDropTablePhase1;
            dropLifetime = 330;
        }
        break;
    case 2:
        if (s->pickupTimer >= 151) {
            spawn = 1;
            lifetime = 450;
        }
        if (s->dropTimer >= 526) {
            drop = 1;
            table = sMotherBossDropTablePhase2;
            dropLifetime = 330;
        }
        break;
    case 0:
        if (s->pickupTimer >= 151) {
            spawn = 1;
            lifetime = 450;
        }
        break;
    }
    if (Cinema_PathPlaying()) {
        return;
    }
    if (spawn) {
        s->pickupTimer = 0;
        id = (s16) WeightedTable_PickStatic(sMotherBossPickupTable);
        dist = Random_Blend(gMotherBoss.minRadius, gMotherBoss_maxRadius);
        angle = Random_Blend(-3.1415927f, 3.1415927f);
        pos.x = dist * func_002982F0(angle);
        pos.z = dist * func_002974C0(angle);
        pos.y = 0.0f;
        spawnPickup(id, lifetime, pos);
    }
    if (table != NULL && drop) {
        s->dropTimer = 0;
        MotherBoss_DropPickupsAtSpawnPoints(dropLifetime, table);
    }
}

typedef struct {
    s16 a;
    s16 b;
    u8 c[4];
} __attribute__((aligned(4))) Part;
extern void AI_UpdateTargets(void *obj, void *ai, CAITargets *t, s32 f1, s32 f2);
extern f32 func_002982F0(f32 angle); /* sin */
extern f32 func_002974C0(f32 angle); /* cos */

static __inline__ void FindTarget(CListedInstance *obj, CListedInstance **target) {
    CAITargets targets;

    *target = NULL;
    /* the AI by member (N64 obj + 0x140): the host CAI is at another offset; the N64 address made
     * AI_UpdatePathHeading read a garbage plan and crash on the Mother's first teleport (round 9 x2) */
    AI_UpdateTargets(obj, &obj->m_Instance.m_AI, &targets, 1, 0);
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

s16 MotherBoss_PickPhaseAnim(void *obj, CAI *ai) {
    Pick picks[8];
    s32 n = 0;

    switch (gMotherBoss.phase) {
        case 0:
            if (ai->m_VisBits & 0x1C) {
                picks[n].id = 0x55FA;
                picks[n++].weight = 4;
            }
            if (ai->m_VisBits & 0x1C0) {
                picks[n].id = 0x5604;
                picks[n++].weight = 4;
            }
            break;
        case 1:
            if (ai->m_VisBits & 2) {
                picks[n].id = 0x5654;
                picks[n++].weight = 4;
            }
            if (ai->m_VisBits & 0x20) {
                picks[n].id = 0x565E;
                picks[n++].weight = 4;
            }
            break;
        case 2:
            break;
    }
    picks[n].id = -1;
    return WeightedTable_PickStatic(picks);
}

/* Per-part callback of the module's actors: in part 0, fades the actor's two blend values in with the module
 * object's anim state (9: first value up to 1; 0x13: after a delay, second value up to 3 then back down to 1;
 * from 30 on: second value 1); the other parts apply one of the values to the part. */

#ifdef TARGET_PORT
#include "structs/pose_host.h"

/* the host layouts (round 9 x2): the boss's m_AI.m_Mode (N64 0x174) and the bone-posing context
 * (structs/pose_host.h: the actor at 0, the bone at N64 0x90). The N64 views read other bytes on
 * the host: the parts never grew in after phase 0 nor pulsed in the phase 1 cinema. */
typedef struct {
    u8 pad0[__builtin_offsetof(CGameObjectInstance, m_AI.m_Mode)];
    s8 animState;
} ModuleObj;

typedef struct {
    CGameObjectInstance *actor;
    u8 pad4[__builtin_offsetof(T2PoseContext, bone) - sizeof(CGameObjectInstance *)];
    u32 part;
} PartInfo;
_Static_assert(__builtin_offsetof(PartInfo, part) == __builtin_offsetof(T2PoseContext, bone), "PartInfo");
#else
typedef struct {
    /* 0x000 */ u8 pad0[0x174];
    /* 0x174 */ s8 animState;
} ModuleObj;

typedef struct {
    /* 0x00 */ CGameObjectInstance *actor;
    /* 0x04 */ u8 pad4[0x90 - 0x4];
    /* 0x90 */ u32 part;
} PartInfo;
#endif
extern void CMtxF__ScaleAxes(void *arg0, f32 r, f32 g, f32 b);

void MotherBoss_PartEffects(void *arg0, PartInfo *info) {
    CGameObjectInstance *actor = info->actor;
    ModuleObj *obj;
    f32 v;
    f32 t;
    f32 dt;

    switch (info->part) {
        case 0:
            obj = gMotherBoss.obj;
            if (obj->animState == 9) {
                v = actor->m_AI.m_Mother.m_GrowScale + gFrameIncrement * 0.01f;
                if (v > 1.0f) {
                    v = 1.0f;
                }
                actor->m_AI.m_Mother.m_GrowScale = v;
            }
            if (obj->animState == 0x13) {
                t = sMotherBossFadeTimer;
                if (t < 8.8f) {
                    sMotherBossFadeTimer = t + gFrameIncrement * (1.0f / 15);
                } else if (sMotherBossCinemaStarted == 0) {
                    dt = gFrameIncrement;
                    actor->m_AI.m_Mother.m_PulseScale += dt * 0.4f;
                    if (actor->m_AI.m_Mother.m_PulseScale > 3.0f) {
                        sMotherBossCinemaStarted = 1;
                    }
                    sMotherBossFadeTimer = t + dt * (1.0f / 15);
                } else {
                    v = actor->m_AI.m_Mother.m_PulseScale - gFrameIncrement * 0.12f;
                    if (v < 1.0f) {
                        v = 1.0f;
                    }
                    actor->m_AI.m_Mother.m_PulseScale = v;
                }
            }
            if (obj->animState >= 30) {
                actor->m_AI.m_Mother.m_PulseScale = 1.0f;
            }
            break;
        case 21:
        case 26:
        case 31:
        case 36:
        case 46:
        case 51:
        case 56:
        case 61:
            CMtxF__ScaleAxes(arg0, actor->m_AI.m_Mother.m_GrowScale, actor->m_AI.m_Mother.m_GrowScale, actor->m_AI.m_Mother.m_GrowScale);
            break;
        case 89:
        case 93:
        case 97:
        case 101:
        case 105:
        case 109:
            CMtxF__ScaleAxes(arg0, actor->m_AI.m_Mother.m_PulseScale, actor->m_AI.m_Mother.m_PulseScale, actor->m_AI.m_Mother.m_PulseScale);
            break;
    }
}

/* MotherBoss_GetTargetDist */
static __inline__ f32 getTargetDist_40A418(CGameObjectInstance *obj, CListedInstance **found) {
    CAITargets t;
    f32 dist;

    dist = -1.0f;
    *found = NULL;
    AI_UpdateTargets(obj, &obj->m_AI, &t, 1, 0);
    if (t.m_Path.m_pTarget != NULL && (t.m_Path.m_pTarget->m_dwFlags & 0x300000)) {
        dist = t.m_Path.m_FlatDist;
        *found = (CListedInstance *) t.m_Path.m_pTarget;
    }
    return dist;
}

void MotherBoss_Phase0ChooseAttack(CGameObjectInstance *obj, CAI *ai) {
    Pick picks[8];
    CListedInstance *found;
    MotherBossState *state = &gMotherBoss;
    s32 n;
    f32 dist;

    n = 0;
    dist = getTargetDist_40A418(obj, &found);
    if (found != NULL) {
        if (dist < 1228.7999f) {
            picks[n].id = 4;
            picks[n++].weight = 6;
            if (state->lastAnim == 4) {
                picks[n].id = 3;
                picks[n++].weight = 6;
            }
        } else {
            state->unk668 = 0;
            if (fewerActorsThan(3)) {
                picks[n].id = 5;
                picks[n++].weight = 5;
            } else {
                picks[n].id = 6;
                picks[n++].weight = 5;
            }
        }
        picks[n].id = -1;
        state->lastAnim = WeightedTable_PickStatic(picks);
        state->counter = 0;
        CAI__SetMode(obj, ai, state->lastAnim);
    }
}

/* Module phase 1 setup: timers, the anim for the phase, slot values; unless ai->m_Mode is 9, resets the
 * phase's active slots and runs MotherBoss_DropPickupsAtSpawnPoints three times. */

extern void ScriptAnimator_SetScript(void *dst, void *src);

static __inline__ void resetSlots(s32 mask, void *src) {
    MotherBossState *state = &gMotherBoss;
    s32 i;

    for (i = 0; i < 15; i++) {
        if ((mask >> i) & 1) {
            ScriptAnimator_SetScript(&state->slots[i].anim, src);
        }
    }
}

void MotherBoss_SetupPhase1(CGameObjectInstance *obj, CAI *ai) {
    MotherBossState *state = &gMotherBoss;
    s32 mask;
    s32 i;

    state->minRadius = 0x600;
    state->maxRadius = 0x900;
    state->phase = 1;
    state->counter = 0;
    obj->m_dwHotPointFlags &= ~0x18000000;
    ai->m_VisBits = sMotherBossPhaseVisBits[state->phase];
    state->liveMask = sMotherBossPhaseLiveMasks[state->phase];
    mask = sMotherBossPhaseActiveMasks[state->phase];
    state->slots[1].health = state->slots[5].health = 0x9600;
    state->totalHealth = state->health = 0x12C00;
    state->activeMask = mask;
    if (ai->m_Mode != 9) {
        resetSlots(state->activeMask, sMotherBossSpotResetScript);
        for (i = 0; i < 3; i++) {
            MotherBoss_DropPickupsAtSpawnPoints(0, sMotherBossPhase1SpawnDrops);
        }
    }
}

/* MotherBoss_GetTargetDist */
static __inline__ f32 getTargetDist_40A6F0(CGameObjectInstance *obj, CGameObjectInstance **found) {
    CAITargets t;
    f32 dist;

    dist = -1.0f;
    *found = NULL;
    AI_UpdateTargets(obj, &obj->m_AI, &t, 1, 0);
    if (t.m_Path.m_pTarget != NULL && (t.m_Path.m_pTarget->m_dwFlags & 0x300000)) {
        dist = t.m_Path.m_FlatDist;
        *found = t.m_Path.m_pTarget;
    }
    return dist;
}

void MotherBoss_Phase1ChooseAttack(CGameObjectInstance *obj, CAI *ai) {
    Pick picks[8];
    CGameObjectInstance *found;
    MotherBossState *state = &gMotherBoss;
    s32 n;
    s32 flagged;
    f32 dist;
    f32 ratio;
    s32 flag2;
    s32 flag20;

    n = 0;
    dist = getTargetDist_40A6F0(obj, &found);
    flagged = 0;
    flag2 = ai->m_VisBits & 2;
    flag20 = ai->m_VisBits & 0x20;
    if (flag2) {
        if (flag20) {
            flagged = 1;
        }
    }
    if (state->totalHealth != 0) {
        ratio = (f32)state->health / (f32)state->totalHealth;
    } else {
        ratio = 1.0f;
    }
    if (found != NULL) {
        if (dist < 1433.6f) {
            picks[n].id = 0x10;
            picks[n++].weight = 5;
            picks[n].id = 0xD;
            picks[n++].weight = 5;
        } else {
            if (flagged && state->lastAnim != 0xF) {
                picks[n].id = 0xF;
                picks[n++].weight = 5;
            }
            picks[n].id = 0x10;
            picks[n++].weight = 5;
            picks[n].id = 0x12;
            if (ratio < 0.75f) {
                if (ratio < 0.5f) {
                    picks[n].weight = 10;
                } else {
                    picks[n].weight = 5;
                }
            } else {
                picks[n].weight = 2;
            }
            n++;
        }
        picks[n].id = -1;
        state->lastAnim = WeightedTable_PickStatic(picks);
        state->counter = 0;
        CAI__SetMode(obj, ai, state->lastAnim);
    }
}

/* MotherBoss_GetTargetDist */
static __inline__ f32 getTargetDist_40A8FC(CGameObjectInstance *obj, CListedInstance **found) {
    CAITargets t;
    f32 dist;

    dist = -1.0f;
    *found = NULL;
    AI_UpdateTargets(obj, &obj->m_AI, &t, 1, 0);
    if (t.m_Path.m_pTarget != NULL && (t.m_Path.m_pTarget->m_dwFlags & 0x300000)) {
        dist = t.m_Path.m_FlatDist;
        *found = (CListedInstance *) t.m_Path.m_pTarget;
    }
    return dist;
}

void MotherBoss_Phase2ChooseAttack(CGameObjectInstance *obj, CAI *ai) {
    Pick picks[8];
    CListedInstance *found;
    MotherBossState *state = &gMotherBoss;
    s32 n;
    s32 close;
    s32 far;
    s32 mid;
    s32 low;
    s32 veryLow;
    f32 dist;
    f32 ratio;

    n = veryLow = low = mid = far = close = 0;
    dist = getTargetDist_40A8FC(obj, &found);
    if (state->totalHealth != 0) {
        ratio = (f32)state->health / (f32)state->totalHealth;
    } else {
        ratio = 1.0f;
    }
    if (ratio < 0.4f) {
        low = 1;
    }
    if (ratio < 0.6f) {
        veryLow = 1;
    }
    if (found != NULL) {
        if (state->lastAnim == 0x17) {
            if (state->counter < 101) {
                close = 1;
            } else {
                picks[n].id = 0x18;
                picks[n++].weight = 5;
            }
        } else if (dist < 460.8f) {
            close = 1;
        } else {
            far = mid = 1;
        }
        if (close) {
            picks[n].id = 0x1A;
            picks[n++].weight = 5;
            picks[n].id = 0x1B;
            picks[n++].weight = 5;
        } else {
            if (far) {
                if (fewerActorsThan(2)) {
                    picks[n].id = 0x19;
                    picks[n++].weight = 4;
                }
                picks[n].id = 0x1D;
                picks[n++].weight = 6;
            }
            if (mid) {
                picks[n].id = 0x17;
                if (low) {
                    picks[n].weight = 6;
                } else if (veryLow) {
                    picks[n].weight = 9;
                } else {
                    picks[n].weight = 12;
                }
                n++;
            }
        }
        picks[n].id = -1;
        state->lastAnim = WeightedTable_PickStatic(picks);
        state->counter = 0;
        CAI__SetMode(obj, ai, state->lastAnim);
    }
}
void MotherBoss_OnPhase2CinemaDone(void);
extern s32 Cinema_SetNextCallbacks(s32 id, void *arg1, void *arg2);

static __inline__ void enterPhase2(MotherBossState *s, CAI *ai) {
    s32 mask;

    s->minRadius = 0x200;
    s->phase = 2;
    s->maxRadius = 0x600;
    gMotherBoss.counter = 0;
    ai->m_VisBits = sMotherBossPhaseVisBits[gMotherBoss.phase];
    gMotherBoss.liveMask = sMotherBossPhaseLiveMasks[gMotherBoss.phase];
    mask = sMotherBossPhaseActiveMasks[gMotherBoss.phase];
    sMotherBossFadeTimer = 0.0f;
    sMotherBossCinemaStarted = 0;
    s->totalHealth = s->health = s->slots[13].health = 0x1F400;
    gMotherBoss.activeMask = mask;
    if (ai->m_Mode != 0x13) {
        resetSlots(gMotherBoss.activeMask, sMotherBossSpotResetScript);
    }
}

void MotherBoss_StartPhase2Cinema(void *obj, CAI *ai) {
    MotherBossState *state = &gMotherBoss;
    CGameObjectInstance *actor;
    s32 i;

    for (i = 0; i < state->count; i++) {
        actor = state->actors[i];
        actor->m_AI.m_Mother.m_GrowScale = 1.0f;
        actor->m_AI.m_Mother.m_PulseScale = 0.0f;
    }
    enterPhase2(&gMotherBoss, ai);
    Cinema_SetNextCallbacks(0xDC, MotherBoss_OnPhase2CinemaDone, NULL);
}

s32 MotherBoss_PickWarpAnim(char *arg0, char *arg1) {
    s32 flags = *(s32 *) (arg1 + 0xC);
    s32 alt = flags & 2;

    if (flags & 0x20) {
        return alt ? 0x5640 : 0x56C2;
    }
    return alt ? 0x56B8 : 0x56A4;
}
void MotherBoss_SetupPhase0(void *obj, CAI *ai);
void MotherBoss_OnIntroCinemaDone(void);

void MotherBoss_StartIntro(void *obj, void *ai) {
    MotherBossState *state = &gMotherBoss;
    CGameObjectInstance *actor;
    s32 i;

    state->count = CScene__FindInstances(&gScene, 0x44E, 9, -1, state->actors, 4);
    for (i = 0; i < state->count; i++) {
        actor = state->actors[i];
        actor->m_AI.m_pfDraw = MotherBoss_Draw;
        actor->m_AI.m_pfEffects = MotherBoss_PartEffects;
    }
    MotherBoss_SetupPhase0(obj, ai);
    Cinema_SetNextCallbacks(0xC8, MotherBoss_OnIntroCinemaDone, NULL);
}

void MotherBoss_SetupPhase0(void *obj, CAI *ai) {
    MotherBossState *state = &gMotherBoss;
    s32 mask;

    state->minRadius = 0x200;
    state->maxRadius = 0x600;
    state->phase = 0;
    state->counter = 0;
    ai->m_VisBits = sMotherBossPhaseVisBits[state->phase];
    state->liveMask = sMotherBossPhaseLiveMasks[state->phase];
    mask = sMotherBossPhaseActiveMasks[state->phase];
    state->slots[2].health = state->slots[3].health = state->slots[4].health = 0x2800;
    state->slots[6].health = state->slots[7].health = state->slots[8].health = 0x2800;
    state->totalHealth = state->health = 0xF000;
    state->activeMask = mask;
    if (ai->m_Mode != 0) {
        resetSlots(state->activeMask, sMotherBossSpotResetScript);
    }
}

/* AI state: once the anim is done, reset every active slot of the module state, then go to anim state 2. */

extern s32 gMotherBoss_activeMask; /* local type: the field's real type (or the member path) changes the ROM's code here */ /* active slot mask */

void MotherBoss_Mode01_FlashSpotsThenIdle(void *obj, CAI *ai) {
    MotherBossState *state;
    s32 mask;
    s32 i;

    if (ai->m_AnimRepeatedCount > 0) {
        mask = gMotherBoss_activeMask;
        state = &gMotherBoss;
        for (i = 0; i < 15; i++) {
            if ((mask >> i) & 1) {
                ScriptAnimator_SetScript(&state->slots[i].anim, sMotherBossSpotFlashScript);
            }
        }
        CAI__SetMode(obj, ai, 2);
    }
}

void MotherBoss_Mode04_TrackTarget(void *obj, CAI *ai) {
    CAITargets t;

    AI_UpdateTargets(obj, ai, &t, 0, 1);
    AI_TurnClamped(obj, ai, 0.261799425f, t.m_Sight.m_Angle);
    if (ai->m_AnimRepeatedCount) {
        CAI__SetMode(obj, ai, 2);
    }
}

void MotherBoss_Mode05_TrackTarget(void *obj, CAI *ai) {
    CAITargets t;

    AI_UpdateTargets(obj, ai, &t, 0, 1);
    AI_TurnClamped(obj, ai, 0.261799425f, t.m_Sight.m_Angle);
    if (ai->m_AnimRepeatedCount) {
        CAI__SetMode(obj, ai, 2);
    }
}

void MotherBoss_Mode06_RecallParts(CGameObjectInstance *obj, CAI *ai) {
    CAITargets t;
    CAITargets t2;
    MotherBossState *state;
    s32 i;

    AI_UpdateTargets(obj, ai, &t, 0, 1);
    if (t.m_Sight.m_pTarget != 0) {
        AI_UpdateTargets(obj, ai, &t2, 0, 1);
        AI_TurnClamped(obj, ai, 0.261799425f, t2.m_Sight.m_Angle);
    }
    if (ai->m_AnimRepeatedCount) {
        state = &gMotherBoss;
        for (i = 0; i < 6; i++) {
            CAI__SetMode(state->parts[i], &state->parts[i]->m_AI, 0);
        }
        CAI__SetMode(obj, ai, 2);
    }
}
void MotherBoss_OnPhase1CinemaDone(void);

void MotherBoss_StartPhase1Cinema(void *obj, void *ai) {
    MotherBossState *state = &gMotherBoss;
    CGameObjectInstance *actor;
    s32 i;

    for (i = 0; i < state->count; i++) {
        actor = state->actors[i];
        actor->m_AI.m_Mother.m_GrowScale = 0;
        actor->m_AI.m_Mother.m_PulseScale = 0;
    }
    MotherBoss_SetupPhase1(obj, ai);
    Cinema_SetNextCallbacks(0xD2, MotherBoss_OnPhase1CinemaDone, NULL);
}

void MotherBoss_Mode10_FlashSpotsThenIdle(void *obj, CAI *ai) {
    MotherBossState *state;
    s32 mask;
    s32 i;

    if (ai->m_AnimRepeatedCount > 0) {
        mask = gMotherBoss_activeMask;
        state = &gMotherBoss;
        for (i = 0; i < 15; i++) {
            if ((mask >> i) & 1) {
                ScriptAnimator_SetScript(&state->slots[i].anim, sMotherBossSpotFlashScript);
            }
        }
        CAI__SetMode(obj, ai, 0xC);
    }
}

void MotherBoss_Mode03_TrackTarget(void *obj, CAI *ai) {
    CAITargets t;

    AI_UpdateTargets(obj, ai, &t, 0, 1);
    AI_TurnClamped(obj, ai, 0.261799425f, t.m_Sight.m_Angle);
    if (ai->m_AnimRepeatedCount) {
        CAI__SetMode(obj, ai, 2);
    }
}

void func_0040B308(void) {

}

void MotherBoss_Mode07_Enter(CGameObjectInstance *arg0) {
    arg0->m_dwHotPointFlags = (s32) (arg0->m_dwHotPointFlags | 0x08000000);
}

void MotherBoss_Mode07_Step(CGameObjectInstance *arg0, CAI *arg1) {
    if (arg1->m_AnimRepeatedCount != 0) {
        arg0->m_dwHotPointFlags = (s32) (arg0->m_dwHotPointFlags & 0xF7FFFFFF);
        CAI__SetMode(arg0, arg1, 2);
    }
}

void MotherBoss_Mode08_Enter(CGameObjectInstance *arg0) {
    arg0->m_dwHotPointFlags = (s32) (arg0->m_dwHotPointFlags | 0x10000000);
}

void MotherBoss_Mode08_Step(CGameObjectInstance *arg0, CAI *arg1) {
    if (arg1->m_AnimRepeatedCount != 0) {
        arg0->m_dwHotPointFlags = (s32) (arg0->m_dwHotPointFlags & 0xEFFFFFFF);
        CAI__SetMode(arg0, arg1, 2);
    }
}

void MotherBoss_Mode11_TrackTarget(void *obj, CAI *ai) {
    CAITargets t;

    AI_UpdateTargets(obj, ai, &t, 0, 1);
    AI_TurnClamped(obj, ai, 0.261799425f, t.m_Sight.m_Angle);
    if (ai->m_AnimRepeatedCount) {
        CAI__SetMode(obj, ai, 0xC);
    }
}

void MotherBoss_Mode13_TrackTarget(void *obj, CAI *ai) {
    CAITargets t;

    AI_UpdateTargets(obj, ai, &t, 0, 1);
    AI_TurnClamped(obj, ai, 0.261799425f, t.m_Sight.m_Angle);
    if (ai->m_AnimRepeatedCount) {
        CAI__SetMode(obj, ai, 0xC);
    }
}

#ifdef TARGET_PORT
#include "structs/game_object.h"
#include "structs/boss.h"

/* Host body (round 9 x2): the ROM reads CAI::m_AnimRepeatedCount at N64 0xC7 and gMotherBoss.counter
 * at N64 0x67C; on the host both members are elsewhere (CAI and MotherBossState hold pointers), so the
 * test read other bytes, the counter never reached 31 and the Mother stayed in mode 16 (her warp
 * animation) for good after phase 1's first teleport attack: no more attacks, no more hits. Every
 * third repeat of the animation teleports the target (MotherBoss_TeleportTarget), after 31 mode 17. */
void MotherBoss_Mode16_TeleportTarget(sptr arg0, void *arg1) {
    CAI *ai = arg1;
    MotherBossState *s = &gMotherBoss;
    s32 count;

    if (ai->m_AnimRepeatedCount != 0) {
        if (s->counter == (s->counter / 3) * 3) {
            MotherBoss_TeleportTarget(arg0, s->counter == 0);
        }
        count = s->counter + 1;
        s->counter = count;
        if (count >= 0x1F) {
            CAI__SetMode((void *) arg0, ai, 0x11);
        }
    }
}
#else
void MotherBoss_Mode16_TeleportTarget(sptr arg0, void *arg1)
{
  s32 temp_v0;
  s32 *new_var;
  if ((*((s8 *) (((s8 *) arg1) + 0xC7))) != 0)
  {
    new_var = &gMotherBoss;
    if ((*((s32 *) (((s8 *) (&gMotherBoss)) + 0x67C))) == (((*((s32 *) (((s8 *) new_var) + 0x67C))) / 3) * 3))
    {
      MotherBoss_TeleportTarget(arg0, (*((s32 *) (((s8 *) (&gMotherBoss)) + 0x67C))) == 0);
    }
    temp_v0 = (*((s32 *) (((s8 *) (&gMotherBoss)) + 0x67C))) + 1;
    *((s32 *) (((s8 *) new_var) + 0x67C)) = temp_v0;
    if (temp_v0 >= 0x1F)
    {
      CAI__SetMode(arg0, arg1, 0x11);
    }
  }
}
#endif

void MotherBoss_Mode17_TrackTarget(void *obj, CAI *ai) {
    CAITargets t;

    gMotherBoss.counter = 0;
    AI_UpdateTargets(obj, ai, &t, 0, 1);
    AI_TurnClamped(obj, ai, 0.261799425f, t.m_Sight.m_Angle);
    if (ai->m_AnimRepeatedCount) {
        CAI__SetMode(obj, ai, 0xC);
    }
}

/* What this reads of the current mode entry (CAI::m_pModeCurrent). */
typedef struct {
    /* 0x0 */ u8 pad0[4];
    /* 0x4 */ s8 state;
} ModeAnim;

void MotherBoss_Mode14_ChargeStep(sptr arg0, CAI *arg1) {
    ((ModeAnim *) arg1->m_pModeCurrent->m_pData)->state = 7;
    if (arg1->m_AnimRepeatedCount != 0) {
        CAI__SetMode(arg0, arg1, 0xB);
    }
}

void MotherBoss_Mode18_TrackTarget(void *obj, CAI *ai) {
    CAITargets t;

    AI_UpdateTargets(obj, ai, &t, 0, 1);
    AI_TurnClamped(obj, ai, 0.261799425f, t.m_Sight.m_Angle);
    if (ai->m_AnimRepeatedCount) {
        CAI__SetMode(obj, ai, 0xC);
    }
}

void MotherBoss_SetupPhase2(void *obj, CAI *ai) {
    MotherBossState *state = &gMotherBoss;
    s32 mask;

    state->minRadius = 0x200;
    state->maxRadius = 0x600;
    state->phase = 2;
    state->counter = 0;
    ai->m_VisBits = sMotherBossPhaseVisBits[state->phase];
    state->liveMask = sMotherBossPhaseLiveMasks[state->phase];
    mask = sMotherBossPhaseActiveMasks[state->phase];
    state->totalHealth = state->health = state->slots[13].health = 0x1F400;
    state->activeMask = mask;
    if (ai->m_Mode != 0x13) {
        resetSlots(state->activeMask, sMotherBossSpotResetScript);
    }
}

void MotherBoss_Mode20_Step(sptr arg0, CAI *arg1) {
    if (arg1->m_AnimRepeatedCount != 0) {
        CAI__SetMode(arg0, arg1, 0x15);
    }
}

void MotherBoss_Mode21_FlashSpotsThenIdle(void *obj, CAI *ai) {
    MotherBossState *state;
    s32 mask;
    s32 i;

    if (ai->m_AnimRepeatedCount != 0) {
        mask = gMotherBoss_activeMask;
        state = &gMotherBoss;
        for (i = 0; i < 15; i++) {
            if ((mask >> i) & 1) {
                ScriptAnimator_SetScript(&state->slots[i].anim, sMotherBossSpotFlashScript);
            }
        }
        CAI__SetMode(obj, ai, 0x16);
    }
}

void MotherBoss_Mode23_Approach(void *obj, void *ai) {
    CAITargets t;
    MotherBossState *state;

    AI_UpdateTargets(obj, ai, &t, 1, 1);
    AI_TurnClamped(obj, ai, 0.261799425f, t.m_Path.m_Angle);
    state = &gMotherBoss;
    if (++state->counter > 100 ||
        (t.m_Path.m_pTarget != NULL && t.m_Sight.m_pTarget != NULL && t.m_Path.m_FlatDist < 460.8f &&
         (t.m_Sight.m_Angle < 0.0f ? -t.m_Sight.m_Angle <= 0.34906590f : t.m_Sight.m_Angle <= 0.34906590f))) {
        CAI__SetMode(obj, ai, 0x16);
    }
}

/* AI state: turn toward the point (0, 0, 512) and go to anim state 0x1C once within 51.2 of it. */

extern f32 AI_AngleToPoint(CGameObjectInstance *obj, CVector3 target);

void MotherBoss_Mode24_GoToCenter(CGameObjectInstance *obj, void *ai) {
    CVector3 d;

    d.x = 0.0f;
    d.y = 0.0f;
    d.z = 512.0f;
    AI_TurnClamped(obj, ai, 0.261799425f, AI_AngleToPoint(obj, d));
    CVector3__Subtract(&d, &d, &obj->header.m_vPos);
    if (func_00298470(d.x * d.x + d.y * d.y + d.z * d.z) < 51.199997f) {
        CAI__SetMode(obj, ai, 0x1C);
    }
}

void MotherBoss_Mode25_TrackTarget(void *obj, CAI *ai) {
    CAITargets t;

    AI_UpdateTargets(obj, ai, &t, 0, 1);
    AI_TurnClamped(obj, ai, 0.261799425f, t.m_Sight.m_Angle);
    if (ai->m_AnimRepeatedCount) {
        CAI__SetMode(obj, ai, 0x16);
    }
}

void MotherBoss_Mode26_TrackTarget(void *obj, CAI *ai) {
    CAITargets t;

    AI_UpdateTargets(obj, ai, &t, 0, 1);
    AI_TurnClamped(obj, ai, 0.261799425f, t.m_Sight.m_Angle);
    if (ai->m_AnimRepeatedCount) {
        CAI__SetMode(obj, ai, 0x16);
    }
}

void MotherBoss_Mode27_TrackTarget(void *obj, CAI *ai) {
    CAITargets t;

    AI_UpdateTargets(obj, ai, &t, 0, 1);
    AI_TurnClamped(obj, ai, 0.261799425f, t.m_Sight.m_Angle);
    if (ai->m_AnimRepeatedCount) {
        CAI__SetMode(obj, ai, 0x16);
    }
}

void MotherBoss_Mode28_Enter(void) {

}

void MotherBoss_Mode28_TeleportTarget(void *obj, CAI *ai) {
    CAITargets t;
    MotherBossState *state;

    AI_UpdateTargets(obj, ai, &t, 0, 1);
    AI_TurnClamped(obj, ai, 0.261799425f, t.m_Sight.m_Angle);
    state = &gMotherBoss;
    if (state->counter % 3 == 0) {
        MotherBoss_TeleportTarget(obj, state->counter == 0);
    }
    state->counter++;
    if (ai->m_AnimRepeatedCount) {
        state->counter = 0;
        CAI__SetMode(obj, ai, 0x16);
    }
}

void MotherBoss_Mode29_TrackTarget(void *obj, CAI *ai) {
    CAITargets t;

    AI_UpdateTargets(obj, ai, &t, 0, 1);
    AI_TurnClamped(obj, ai, 0.261799425f, t.m_Sight.m_Angle);
    if (ai->m_AnimRepeatedCount) {
        CAI__SetMode(obj, ai, 0x16);
    }
}

/* AI update for the module's actor: runs the AI step and the current anim's callback; in phase 0, a target
 * closer than 1228.8 makes the gScene_m_InstancePool_m_ActiveList actors drop their health; then updates the module (MotherBoss_UpdateSpotsAndPickups). */

extern void AI_TurnHeadToTarget(void *obj, CAI *ai, f32 pitchOffset);
void MotherBoss_KillMinions(void);

void MotherBoss_Step(void *obj, CAI *ai) {
    MotherBossState *state = &gMotherBoss;
    void *found;
    f32 dist;

    AI_TurnHeadToTarget(obj, ai, 0.0f);
    if (ai->m_pModeCurrent != NULL && ai->m_pModeCurrent->m_pfCallback != NULL) {
        ai->m_pModeCurrent->m_pfCallback(obj, ai);
    }
    if (state->phase == 0) {
        dist = MotherBoss_GetTargetDist(obj, &found);
        if (found != NULL && dist < 1228.7999f) {
            MotherBoss_KillMinions();
        }
    }
    MotherBoss_UpdateSpotsAndPickups(&gMotherBoss);
}

void MotherBoss_KillMinions(void) {
    CListedInstance *var_s0;

    var_s0 = gScene.m_InstancePool.m_ActiveList.m_pHead;
    if (var_s0 != NULL) {
        do {
            if (var_s0->m_Instance.m_AI.m_Health > 0) {
                var_s0->m_Instance.m_AI.m_Health = 0;
                CAI__SetMode(var_s0, &var_s0->m_Instance.m_AI, 0x34);
            }
            var_s0 = var_s0->m_pNext;
        } while (var_s0 != NULL);
    }
}

f32 MotherBoss_GetTargetDist(CGameObjectInstance *obj, CGameObjectInstance **found) {
    CAITargets t;
    f32 dist;

    dist = -1.0f;
    *found = NULL;
    AI_UpdateTargets(obj, &obj->m_AI, &t, 1, 0);
    if (t.m_Path.m_pTarget != NULL && (t.m_Path.m_pTarget->m_dwFlags & 0x300000)) {
        dist = t.m_Path.m_FlatDist;
        *found = t.m_Path.m_pTarget;
    }
    return dist;
}

void MotherBoss_StartSpotAnims(void *obj, void *src, s32 mask) {
    MotherBossState *state = &gMotherBoss;
    s32 i;

    for (i = 0; i < 15; i++) {
        if ((mask >> i) & 1) {
            ScriptAnimator_SetScript(&state->slots[i].anim, src);
        }
    }
}

/* AI state: calls CPlayerPool__SetInvincibility on the players with 900, then goes to anim state 0x1F once the anim is done. */

extern void CPlayerPool__SetInvincibility(void *players, f32 value);

void MotherBoss_Mode30_DeathStep(void *obj, CAI *ai) {
    CPlayerPool__SetInvincibility(&gEngineApp_m_Game.m_PlayerList, 900.0f);
    if (ai->m_AnimRepeatedCount > 0) {
        CAI__SetMode(obj, ai, 0x1F);
    }
}
void MotherBoss_OnDefeatCinemaDone(void);

void MotherBoss_StartDefeatCinema(void) {
    CPlayerPool__SetInvincibility(&gEngineApp_m_Game.m_PlayerList, 900.0f);
    Cinema_SetNextCallbacks(0xE6, MotherBoss_OnDefeatCinemaDone, NULL);
}

/* Draw callback of the module's actors: draws each body part (anim group) whose slot is active in the
 * module object's mask, refreshing the slot's state first unless a cinema path is playing. */


extern void CGameObjectInstance__DrawShadow(CGameObjectInstance *actor, void *ai, CDrawArgs *req);
extern void ScriptAnimator_Apply(void *slotData);
extern void ScreenTint_UpdateMode(void);

void MotherBoss_Draw(CGameObjectInstance *actor, void *ai, CDrawArgs *req) {
    MotherBossState *state;
    u8 *data;
    s32 mask;
    s32 i;
    s32 one;

    i = 0;
    one = 1;
    state = &gMotherBoss;
    mask = ((CGameObjectInstance *)state->obj)->m_AI.m_VisBits;
    data = (u8 *) &state->slots[0].anim;
    do {
        if (mask & (one << i)) {
            req->m_nModel = i;
            req->m_nShadowModel = i;
            CGameObjectInstance__DrawShadow(actor, ai, req);
            if (!Cinema_PathPlaying()) {
                ScriptAnimator_Apply(data);
                ScreenTint_UpdateMode();
            }
            Render_DrawModel(req->m_hModel, actor->m_pmtDrawMtxs, 1, &actor->m_Lights[gEvenOdd], 0, i, actor->m_AI.m_nTexture);
            gBlendState = 0;
        }
        i++;
        data += sizeof(BossWeakSpot);
    } while (i < 15);
}

extern void CVector3__Scale(CVector3 *out, CVector3 *in, f32 scale);
extern void DamageEvent_Set(DamageEvent *event, void *source, s32 damage, f32 radius, s32 flags, void *shield);
typedef void (*CAIHurtFn)(CGameObjectInstance *target, CAI *ai, DamageEvent *event);

void MotherBoss_Collision(CGameObjectInstance *obj, CAI *ai) {
    DamageEvent event;
    CVector3 dir;
    CGameObjectInstance *player = (CGameObjectInstance *) gCollisionResults.m_pInstance;

    if (player != NULL && player->header.m_Type == 1 && (player->m_dwFlags & 0x300000) && ai->m_Health != 0 &&
        (ai->m_Mode == 0xE || ai->m_Mode == 0xF) && (player == ai->m_pSightTarget || player == ai->m_pPathTarget)) {
        dir.x = 0.0f;
        dir.y = 0.0f;
        dir.z = 512.0f;
        CVector3__Subtract(&dir, &obj->header.m_vPos, &dir);
        CVector3__Normalize(&dir);
        CVector3__Scale(&dir, &dir, 1536.0f);
        dir.y = 409.59998f;
        CVector3__Add(&player->m_vVelocity, &player->m_vVelocity, &dir);
        DamageEvent_Set(&event, obj, 0x500, 102.399994f, 0x100000, NULL);
        ((CAIHurtFn) CAI__CallHurt)(player, &player->m_AI, &event);
    }
}

void MotherBoss_RandomArenaPoint(CVector3 *out) {
    MotherBossState *state = &gMotherBoss;
    f32 dist;
    f32 angle;
    f32 z;

    dist = Random_Blend((f32)state->minRadius, (f32)state->maxRadius);
    angle = Random_Blend(-3.1415927f, 3.1415927f);
    out->x = dist * func_002982F0(angle);
    z = dist * func_002974C0(angle);
    out->y = 0.0f;
    out->z = z;
}

void MotherBoss_RandomSpawnPoint(CVector3 *out) {
    CGameObjectInstance *found;
    s32 ok;
    s32 i;

    MotherBoss_GetTargetDist(gMotherBoss.obj, &found);
    do {
        ok = 1;
        i = Random_Blend(0.0f, 0.99999f) * 4.0f;
        if (found != NULL) {
            CVector3__Subtract(out, &found->header.m_vPos, &gMotherSpawnPoints[i].pos);
            if (func_00298470(out->x * out->x + out->y * out->y + out->z * out->z) < 460.8f) {
                ok = 0;
            }
        }
    } while (!ok);
    *out = gMotherSpawnPoints[i].pos;
}

void MotherBoss_SpawnPickup(s32 id, s32 lifetime, CVector3 pos) {
    CVector3 vel;
    sptr region;
    CPooledLevelActor *pickup;

    region = CScene__FindRegionAt(&gScene, &pos);
    vel.x = 0.0f;
    vel.y = 512.0f;
    vel.z = 0.0f;
    pickup = CLevelActorPool__Spawn(&gScene.m_ActorPool, NULL, id, vel, pos, region, lifetime);
    if (pickup != NULL) {
        Actor_SpawnParticle(pickup, 0x11D, pos, 0);
    }
}

/* MotherBoss_RandomArenaPoint */
static __inline__ void randomGroundPoint(CVector3 *out) {
    MotherBossState *state = &gMotherBoss;
    f32 dist;
    f32 angle;
    f32 z;

    dist = Random_Blend((f32)state->minRadius, (f32)state->maxRadius);
    angle = Random_Blend(-3.1415927f, 3.1415927f);
    out->x = dist * func_002982F0(angle);
    z = dist * func_002974C0(angle);
    out->y = 0.0f;
    out->z = z;
}

void MotherBoss_SpawnRandomPickup(s32 lifetime, Pick *table) {
    CVector3 pos;
    s32 id;

    id = WeightedTable_PickStatic(table);
    randomGroundPoint(&pos);
    spawnPickup(id, lifetime, pos);
}

/* Returns whether fewer than max actors are in the gScene_m_InstancePool_m_ActiveList list. */


s32 MotherBoss_FewerActorsThan(s32 max) {
    CListedInstance *actor;
    s32 count;

    count = 0;
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

/* Spawns effect 3 from obj at other's position, with params (0xA00, value << 8). */

typedef struct {
    /* 0x00 */ s16 upper;
    /* 0x02 */ s16 lower;
    /* 0x04 */ u8 extra[4];
} __attribute__((aligned(4))) ActionParams;
extern void ImpactEvent_Send(); /* (src, dst, kind, id, CVector3 pos, 8-byte params): MotherBoss_TeleportTarget's file names the params Part */
typedef void (*ImpactSendFn)(void *source, void *p, s32 effect, s32 arg3, CVector3 pos, ActionParams params);

void MotherBoss_SendImpactAt(CGameObjectInstance *obj, CGameObjectInstance *other, s32 value) {
    ActionParams params;

    params.upper = 0xA00;
    params.lower = value << 8;
    params.extra[0] = params.extra[1] = params.extra[2] = params.extra[3] = 0;
    ((ImpactSendFn) ImpactEvent_Send)(obj, obj, 3, -1, other->header.m_vPos, params);
}

void MotherBoss_TurnToTarget(void *obj, void *ai) {
    CAITargets t;

    AI_UpdateTargets(obj, ai, &t, 0, 1);
    AI_TurnClamped(obj, ai, 0.261799425f, t.m_Sight.m_Angle);
}

void MotherBoss_FlashActiveSpots(void) {
    MotherBossState *state = &gMotherBoss;

    resetSlots(state->activeMask, sMotherBossSpotFlashScript);
}

void MotherBoss_OnIntroCinemaDone(void) {
    CAI__SetMode(gMotherBoss.obj, &gMotherBoss.obj->m_AI, 1);
}

void MotherBoss_TurnThenPhase2Choose(void *obj, CAI *ai) {
    CAITargets t;

    AI_UpdateTargets(obj, ai, &t, 0, 1);
    AI_TurnClamped(obj, ai, 0.261799425f, t.m_Sight.m_Angle);
    if (ai->m_AnimRepeatedCount) {
        CAI__SetMode(obj, ai, 0x16);
    }
}

extern void CPlayer__SetPosition(CPlayer *player, void *arg1);
extern void CPlayer__UpdateOrbitCamera(CPlayer *player, CPlayer *other);

/* MotherBoss_GetTargetDist */
static __inline__ f32 getTargetDist_40C6F4(CGameObjectInstance *obj, CGameObjectInstance **found) {
    CAITargets t;
    f32 dist;

    dist = -1.0f;
    *found = NULL;
    AI_UpdateTargets(obj, &obj->m_AI, &t, 1, 0);
    if (t.m_Path.m_pTarget != NULL && (t.m_Path.m_pTarget->m_dwFlags & 0x300000)) {
        dist = t.m_Path.m_FlatDist;
        *found = t.m_Path.m_pTarget;
    }
    return dist;
}

void MotherBoss_OnPhase1CinemaDone(void) {
    CGameObjectInstance *obj = gMotherBoss.obj;
    CGameObjectInstance *found;
    CPlayer *player;

    player = NULL;
    getTargetDist_40C6F4(obj, &found);
    if (found != NULL) {
        player = ((CPlayer *)found->m_AI.m_pParent);
    }
    if (player != NULL) {
        CPlayer__SetPosition(player, sMotherBossPhase1PlayerPos);
        CPlayer__UpdateOrbitCamera(player, player);
        player->m_WeaponActor.m_RotY = player->m_CharacterActor.m_RotY = 3.141593f;
        obj->m_RotY = 0.0f;
    }
    CAI__SetMode(obj, &obj->m_AI, 0xA);
}

void MotherBoss_OnPhase2CinemaDone(void) {
    CAI__SetMode(gMotherBoss.obj, &gMotherBoss.obj->m_AI, 0x14);
}

s32 CPlayer__Warp(sptr, s32, s32, s32);

void MotherBoss_OnDefeatCinemaDone(void) {
    CPlayer *temp_v0;
    GameSettings *temp_v1;

    CAI__SetMode(gMotherBoss.obj, &gMotherBoss.obj->m_AI, 1);
    temp_v0 = CPlayerPool__GetFirstActive(&gEngineApp_m_Game.m_PlayerList);
    if (temp_v0 != NULL) {
        temp_v1 = &gEngineApp_m_Game.m_Settings;
        temp_v0->m_Data.m_bMotherBossComplete = 1;
        temp_v1->unlockedCheats = (s32) (temp_v1->unlockedCheats | 4);
        CPlayer__Warp((sptr) temp_v0, 0x1B57, 2, 0);
    }
}
