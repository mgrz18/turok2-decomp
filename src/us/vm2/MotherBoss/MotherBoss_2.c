#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "structs/collision.h"
#include "globals/boss.h"
#include "protos/ai.h"
#include "globals/scene.h"
#include "globals/boss_ai.h"
#include "protos/scene.h"
#include "globals/collision.h"
#include "structs/vector.h"
/* The second VM module's copy of src/us/vm1/MotherBoss/func_00409A64.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
/* MotherBoss_TeleportTarget_2 and MotherBoss_AnimEvent_2: one file. MotherBoss_AnimEvent_2's pool (0x800C061C)
 * starts 4 past an 8-byte boundary and holds a jump table, which its own object
 * would 8-align; the file's .rodata opens with MotherBoss_TeleportTarget_2's pool at 0x800C05F8.
 * MotherBoss_TeleportTarget_2 is the first-module twin of src/us/code/MotherBoss_TeleportTarget_2.c.
 * FAKEMATCH (MotherBoss_AnimEvent_2): the actor count uses fewerActorsThan, the inline copy
 * of MotherBoss_FewerActorsThan_2 with identical if/else arms in its loop (the same fake as
 * src/us/vm1/MotherBoss/func_0040C4F4.c, func_0040A418.c and func_0040A8FC.c, where func_0040C4F4.c says why and what was
 * tried); the plain loop puts the counter in $v0 instead of $v1. */
#include "common.h"
#include "structs/player.h"
#include "protos/game.h"
/* The second VM module's copy of src/us/vm1/MotherBoss/func_0040A170.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
#include "structs/pick.h"
/* The second VM module's copy of src/us/vm1/MotherBoss/func_0040A25C.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
#include "globals/engine.h"
/* The second VM module's copy of src/us/vm1/MotherBoss/func_0040A418.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
/* FAKEMATCH: identical if/else arms in fewerActorsThan's counting loop (as in MotherBoss_FewerActorsThan_2,
 * src/us/vm1/MotherBoss/func_0040C4F4.c, which says why and what was tried). */
#include "common.h"
/* Second-module twin of MotherBoss_SetupPhase1 (VM module 2 is linked over module 1; same source, its own strings and pool). */
#include "common.h"
/* Second-module twin of MotherBoss_Phase1ChooseAttack (src/us/code); here the cut is whole (0x004436F0, 0x20C). Both flag masks are computed before the tests (found with the permuter). */
#include "common.h"
/* The second VM module's copy of src/us/vm1/MotherBoss/func_0040A8FC.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
/* FAKEMATCH: identical if/else arms in fewerActorsThan's counting loop (as in MotherBoss_FewerActorsThan_2,
 * src/us/vm1/MotherBoss/func_0040C4F4.c, which says why and what was tried); the plain
 * loop shifts its registers by one. */
#include "common.h"
/* Second-module twin of src/us/code/MotherBoss_StartPhase2Cinema.c (data 0xE0 lower; MotherBoss_OnPhase2CinemaDone is the same link address). */
/* gMotherBoss_phase/D0/D4/EC are fields of gMotherBoss (phase, activeMask, unk664, counter) that the ROM addresses
 * through their own symbols (as arrays: their loads are reloaded after the ai store). */
#include "common.h"
/* The second VM module's copy of src/us/vm1/MotherBoss/func_0040AD60.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
/* Second-module twin of MotherBoss_SetupPhase0 (VM module 2 is linked over module 1; same source, its own strings and pool). */
#include "common.h"
/* Second-module twin of MotherBoss_Mode01_FlashSpotsThenIdle (VM module 2 is linked over module 1; same source, its own strings and pool). */
#include "common.h"
/* The second VM module's copy of src/us/vm1/MotherBoss/func_0040AFD0.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
/* The second VM module's copy of src/us/vm1/MotherBoss/func_0040B040.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
/* The second VM module's copy of src/us/vm1/MotherBoss/func_0040B0B0.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
/* The second VM module's copy of src/us/vm1/MotherBoss/func_0040B180.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
/* Second-module twin of MotherBoss_Mode10_FlashSpotsThenIdle (VM module 2 is linked over module 1; same source, its own strings and pool). */
#include "common.h"
/* The second VM module's copy of src/us/vm1/MotherBoss/func_0040B298.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
#include "m2c_macros.h"
/* The second VM module's copy of src/us/vm1/MotherBoss/func_0040B3A8.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
/* The second VM module's copy of src/us/vm1/MotherBoss/func_0040B418.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
/* The second VM module's copy of src/us/vm1/MotherBoss/func_0040B524.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
/* The second VM module's copy of src/us/vm1/MotherBoss/func_0040B5D4.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
/* Second-module twin of MotherBoss_SetupPhase2 (VM module 2 is linked over module 1; same source, its own strings and pool). */
#include "common.h"
/* Second-module twin of MotherBoss_Mode21_FlashSpotsThenIdle (VM module 2 is linked over module 1; same source, its own strings and pool). */
#include "common.h"
/* The second VM module's copy of src/us/vm1/MotherBoss/func_0040B80C.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
/* The second VM module's copy of src/us/vm1/MotherBoss/func_0040B918.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
/* The second VM module's copy of src/us/vm1/MotherBoss/func_0040B9D8.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
/* The second VM module's copy of src/us/vm1/MotherBoss/func_0040BA48.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
/* The second VM module's copy of src/us/vm1/MotherBoss/func_0040BAB8.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
/* The second VM module's copy of src/us/vm1/MotherBoss/func_0040BB30.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
/* The second VM module's copy of src/us/vm1/MotherBoss/func_0040BBF4.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
/* The second VM module's copy of src/us/vm1/MotherBoss/func_0040BC64.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
/* The second VM module's copy of src/us/vm1/MotherBoss/func_0040BD60.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
/* The second VM module's copy of src/us/vm1/MotherBoss/func_0040BDD0.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
/* The second VM module's copy of src/us/vm1/MotherBoss/func_0040BE50.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
#include "structs/game.h"
#include "globals/game.h"
/* The second VM module's copy of src/us/vm1/MotherBoss/func_0040BEA8.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
/* The second VM module's copy of src/us/vm1/MotherBoss/func_0040BEE4.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
#include "structs/light.h"
#include "structs/model.h"
/* The second VM module's copy of src/us/vm1/MotherBoss/func_0040BFF0.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
/* The second VM module's copy of src/us/vm1/MotherBoss/func_0040C110.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
/* Second-module twin of MotherBoss_RandomSpawnPoint (VM module 2 is linked over module 1; same source, its own strings and pool). */
#include "common.h"
/* The second VM module's copy of src/us/vm1/MotherBoss/func_0040C2C4.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
#include "structs/level_actor.h"
/* The second VM module's copy of src/us/vm1/MotherBoss/func_0040C394.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
/* The second VM module's copy of src/us/vm1/MotherBoss/func_0040C4F4.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
/* FAKEMATCH: identical if/else arms in the counting loop (found by the permuter); the plain loop puts the
 * counter in $v0 instead of $a1. Why it works: the arms' test loads m_pNext into a temporary that lives in
 * one block, local-alloc gives it $v0, so every value live across the loop conflicts with $v0 and the
 * counter falls to $a1; cross-jumping merges the arms only after reload, and the dead load goes with them.
 * Natural forms tried in the L pass (all give the counter $v0): for/while/do-while/comma-for loops, a `next`
 * pointer, a break on the last actor, an inline count function, `count < max ? 1 : 0`, if/return pairs,
 * `register` locals, `max` copied to a local, a ternary next, and a dead second counter or `last` pointer
 * (deleted before allocation, so they leave no temporary). Only arms that survive to reload do it. */
#include "common.h"
/* The second VM module's copy of src/us/vm1/MotherBoss/func_0040C518.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
/* The second VM module's copy of src/us/vm1/MotherBoss/func_0040C584.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
/* Second-module copy of MotherBoss_FlashActiveSpots (src/us/code/MotherBoss_FlashActiveSpots.c), with its string/data addresses 0xE0 lower. */
#include "common.h"
/* The second VM module's copy of src/us/vm1/MotherBoss/func_0040C684.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
/* Second-module copy of MotherBoss_OnPhase1CinemaDone (src/us/code/MotherBoss_OnPhase1CinemaDone.c), with its string/data addresses 0xE0 lower. */
#include "common.h"
#include "protos/player.h"
#include "structs/twin_motherboss.h"

void MotherBoss_StartSpotAnims_2(void *obj, void *src, s32 mask);

void MotherBoss_Hurt_2(void *obj, CAI *ai, DamageEvent *hit) {
    MotherBossState *state;
    HitFrame *part;
    s32 mask;
    s32 idx;
    s32 i;
    s32 group;
    s32 sound;
    s32 slot;
    s32 bit;

    part = hit->frame;
    if (part == NULL) {
        return;
    }
    state = &gMotherBoss;
    i = 0;
    idx = -1;
    mask = state->activeMask & part->unk_6C;
    for (; i < 15; i++) {
        if (idx >= 0) {
            break;
        }
        bit = 1 << i;
        if (bit & mask) {
            idx = i;
        }
    }
    switch (idx) {
        case 10:
        case 11:
            group = 0;
            sound = 0xC00;
            break;
        case 12:
        case 14:
            group = 1;
            sound = 0x5000;
            break;
        case 13:
            group = 2;
            sound = 0x2000;
            break;
        default:
            return;
    }
    slot = -1;
    switch (state->phase) {
        case 0:
            if (group == 0) {
                if (state->slots[2].health > 0) {
                    slot = 2;
                } else if (state->slots[3].health > 0) {
                    slot = 3;
                } else if (state->slots[4].health > 0) {
                    slot = 4;
                }
            } else if (group == 1) {
                if (state->slots[6].health > 0) {
                    slot = 6;
                } else if (state->slots[7].health > 0) {
                    slot = 7;
                } else if (state->slots[8].health > 0) {
                    slot = 8;
                }
            }
            break;
        case 1:
            if (group == 0) {
                if (state->slots[1].health > 0) {
                    slot = 1;
                }
            } else if (group == 1) {
                if (state->slots[5].health > 0) {
                    slot = 5;
                }
            }
            break;
        case 2:
            if (group == 2) {
                if (state->slots[13].health > 0) {
                    slot = 13;
                }
            }
            break;
    }
    if (slot == -1) {
        return;
    }
    /* the damage dealt, capped at the slot health */
    idx = state->slots[slot].health >= hit->damage ? hit->damage : state->slots[slot].health;
    state->slots[slot].health -= idx;
    state->health -= idx;
    MotherBoss_StartSpotAnims_2(obj, sMotherBossHurtScript_2, sound);
    if (state->slots[slot].health > 0) {
        return;
    }
    if (state->phase == 0) {
        state->partsLeft--;
        switch (group) {
            case 0:
                CAI__SetMode(obj, ai, 7);
                break;
            case 1:
                CAI__SetMode(obj, ai, 8);
                break;
        }
    }
    bit = 1 << slot;
    if (state->phase != 2) {
        ai->m_VisBits &= ~bit;
    }
    state->liveMask &= ~bit;
    if (state->liveMask == 0) {
        switch (state->phase) {
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
}

/* Module init: find the module actors (0x1DB1) and the controller (0x1DB2), set up the AI (mode table and callbacks),
 * then attach every slot to the object. */

extern void ScriptAnimator_Start(void *slot, void *obj);
extern void EnemyAI_HeadNodeCallback();
extern void func_00441FF8();
void MotherBoss_Step_2(void *obj, CAI *ai);
void MotherBoss_Draw_2(CGameObjectInstance *actor, void *ai, CDrawArgs *req);
void MotherBoss_Collision_2(CGameObjectInstance *obj, CAI *ai);

void MotherBoss_Init_2(CGameObjectInstance *obj, CAI *ai) {
    MotherBossState *state = &gMotherBoss;
    s32 i;

    gScene_m_LevelID = 0x1DB1;
    state->pickupTimer = 0;
    state->dropTimer = 0;
    state->partCount = CScene__FindInstances(&gScene, 0x650, 10, 0x1DB1, state->parts, 6);
    for (i = 0; i < state->partCount; i++) {
        state->parts[i]->m_AI.m_Mother.m_bActive = 1;
    }
    state->partsLeft = state->partCount;
    CScene__FindInstances(&gScene, 0x650, 10, 0x1DB2, &state->controller, 1);
    state->controller->m_AI.m_Mother.m_bActive = 1;
    state->intel = CScene__FindIntelligence(&gScene, 1, 0x1DB1);
    ai->m_pModeTable = (struct CAIModeInfo *) sMotherBossModeTable_2;
    ai->m_pfStep = MotherBoss_Step_2;
    ai->m_pfHurt = (u8 *)func_00441FF8 + 8;
    ai->m_pfEffects = EnemyAI_HeadNodeCallback;
    ai->m_pfCollision = MotherBoss_Collision_2;
    ai->m_pfDraw = MotherBoss_Draw_2;
    CAI__SetMode(obj, ai, 0);
    gMotherBoss.obj = obj;
    gCollisionInfoGravity.m_dwFlags &= ~0x300;
    obj->m_dwFlags |= 0x04000000;
    for (i = 0; i < 15; i++) {
        ScriptAnimator_Start(&gMotherBoss.slots[i].anim, obj);
    }
}
f32 MotherBoss_GetTargetDist_2(CGameObjectInstance *obj, CGameObjectInstance **found);
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

void MotherBoss_DropPickupsAtSpawnPoints_2(s32 arg0, void *arg1) {
    CVector3 v;
    CGameObjectInstance *found;
    s32 i;
    s32 type;
    s32 dx;
    s32 dz;

    MotherBoss_GetTargetDist_2(gMotherBoss.obj, &found);
    for (i = 0; i < 4; i++) {
        type = (s16)WeightedTable_PickStatic(arg1);
        dx = Random_Blend(-15.0f, 15.0f);
        dz = Random_Blend(-15.0f, 15.0f);
        dx = dx * 10.24f;
        dz = dz * 10.24f;
        if (found != NULL) {
            CVector3__Subtract(&v, &found->header.m_vPos, &gMotherSpawnPoints_2[i].pos);
            if (func_00298470(v.x * v.x + v.y * v.y + v.z * v.z) > 460.8f) {
                v = gMotherSpawnPoints_2[i].pos;
                v.y = 30.72f;
                v.x += dx;
                v.z += dz;
                SpawnAt(arg0, type, v, 512.0f);
            }
        } else {
            v = gMotherSpawnPoints_2[i].pos;
            v.y = 30.72f;
            v.x += dx;
            v.z += dz;
            SpawnAt(arg0, type, v, 512.0f);
        }
    }
}

/* Module update: ticks every slot, then (outside cinematics) counts the two phase timers: the first spawns
 * one actor at a random spot around the origin, the second starts a burst of spawns at the phase points. */

extern void Animator_Update(void *slot);
extern s32 Cinema_PathPlaying(void);
extern f32 func_002982F0(f32 angle); /* sin */
extern f32 func_002974C0(f32 angle); /* cos */

static __inline__ void SpawnAt_442784(s32 delay, s16 type, CVector3 pos, f32 speed) {
    CVector3 vel;
    sptr region;
    void *actor;

    region = CScene__FindRegionAt(&gScene, &pos);
    vel.x = 0.0f;
    vel.y = speed;
    vel.z = 0.0f;
    actor = CLevelActorPool__Spawn(&gScene.m_ActorPool, 0, type, vel, pos, region, delay);
    if (actor != NULL) {
        Actor_SpawnParticle(actor, 0x11D, pos, 0);
    }
}

void MotherBoss_UpdateSpotsAndPickups_2(MotherBossState *state) {
    CVector3 v;
    s32 i;
    s32 spawn = 0;
    s32 burst = 0;
    s32 delay = 0;
    s32 count = 0;
    void *points = NULL;
    s32 type;
    f32 r;
    f32 angle;

    for (i = 0; i < 15; i++) {
        Animator_Update(&state->slots[i].anim);
    }
    if (Cinema_PathPlaying()) {
        return;
    }
    state->pickupTimer++;
    state->dropTimer++;
    switch (state->phase) {
        case 1:
            if (state->pickupTimer >= 76) {
                spawn = 1;
                delay = 600;
            }
            if (state->dropTimer >= 526) {
                burst = 1;
                points = sMotherBossDropTablePhase1_2;
                count = 330;
            }
            break;
        case 2:
            if (state->pickupTimer >= 151) {
                spawn = 1;
                delay = 450;
            }
            if (state->dropTimer >= 526) {
                burst = 1;
                points = sMotherBossDropTablePhase2_2;
                count = 330;
            }
            break;
        case 0:
            if (state->pickupTimer >= 151) {
                spawn = 1;
                delay = 450;
            }
            break;
    }
    if (Cinema_PathPlaying()) {
        return;
    }
    if (spawn) {
        state->pickupTimer = 0;
        type = (s16)WeightedTable_PickStatic(sMotherBossPickupTable_2);
        r = Random_Blend(gMotherBoss.minRadius, gMotherBoss_maxRadius);
        angle = Random_Blend(-3.1415927f, 3.1415927f);
        v.x = r * func_002982F0(angle);
        v.z = r * func_002974C0(angle);
        v.y = 0.0f;
        SpawnAt_442784(delay, type, v, 512.0f);
    }
    if (points != NULL && burst) {
        state->dropTimer = 0;
        MotherBoss_DropPickupsAtSpawnPoints_2(count, points);
    }
}

extern void AI_UpdateTargets(void *obj, void *ai, CAITargets *t, s32 f1, s32 f2);

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
void MotherBoss_TeleportTarget_2(CListedInstance *obj, s32 nearSelf) {
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
        CVector3__Subtract(&p, &target->m_Instance.header.m_vPos, &gMotherSpawnPoints_2[i].pos);
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
                if (DistXZ(p, &gMotherSpawnPoints_2[i].pos) < 460.8f) {
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

/* MotherBoss_FewerActorsThan_2 */
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

/* MotherBoss_GetTargetDist_2 */
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
void MotherBoss_AnimEvent_2(s32 arg0, void *self, s32 arg2, AnimEvent ev) {
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

s16 MotherBoss_PickPhaseAnim_2(void *obj, CAI *ai) {
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

extern void CMtxF__ScaleAxes(void *arg0, f32 r, f32 g, f32 b);

void MotherBoss_PartEffects_2(void *arg0, PartInfo *info) {
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

/* MotherBoss_GetTargetDist_2 */
static __inline__ f32 getTargetDist_443418(CGameObjectInstance *obj, CListedInstance **found) {
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

void MotherBoss_Phase0ChooseAttack_2(CGameObjectInstance *obj, CAI *ai) {
    Pick picks[8];
    CListedInstance *found;
    MotherBossState *state = &gMotherBoss;
    s32 n;
    f32 dist;

    n = 0;
    dist = getTargetDist_443418(obj, &found);
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

void MotherBoss_SetupPhase1_2(CGameObjectInstance *obj, CAI *ai) {
    MotherBossState *state = &gMotherBoss;
    s32 mask;
    s32 i;

    state->minRadius = 0x600;
    state->maxRadius = 0x900;
    state->phase = 1;
    state->counter = 0;
    obj->m_dwHotPointFlags &= ~0x18000000;
    ai->m_VisBits = sMotherBossPhaseVisBits_2[state->phase];
    state->liveMask = sMotherBossPhaseLiveMasks_2[state->phase];
    mask = sMotherBossPhaseActiveMasks_2[state->phase];
    state->slots[1].health = state->slots[5].health = 0x9600;
    state->totalHealth = state->health = 0x12C00;
    state->activeMask = mask;
    if (ai->m_Mode != 9) {
        resetSlots(state->activeMask, sMotherBossSpotResetScript_2);
        for (i = 0; i < 3; i++) {
            MotherBoss_DropPickupsAtSpawnPoints_2(0, sMotherBossPhase1SpawnDrops_2);
        }
    }
}

/* MotherBoss_GetTargetDist */
static __inline__ f32 getTargetDist_4436F0(CGameObjectInstance *obj, CGameObjectInstance **found) {
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

void MotherBoss_Phase1ChooseAttack_2(CGameObjectInstance *obj, CAI *ai) {
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
    dist = getTargetDist_4436F0(obj, &found);
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

/* MotherBoss_GetTargetDist_2 */
static __inline__ f32 getTargetDist_4438FC(CGameObjectInstance *obj, CListedInstance **found) {
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

void MotherBoss_Phase2ChooseAttack_2(CGameObjectInstance *obj, CAI *ai) {
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
    dist = getTargetDist_4438FC(obj, &found);
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
void MotherBoss_OnPhase2CinemaDone_2(void);
extern s32 Cinema_SetNextCallbacks(s32 id, void *arg1, void *arg2);

static __inline__ void enterPhase2(MotherBossState *s, CAI *ai) {
    s32 mask;

    s->minRadius = 0x200;
    s->phase = 2;
    s->maxRadius = 0x600;
    gMotherBoss.counter = 0;
    ai->m_VisBits = sMotherBossPhaseVisBits_2[gMotherBoss.phase];
    gMotherBoss.liveMask = sMotherBossPhaseLiveMasks_2[gMotherBoss.phase];
    mask = sMotherBossPhaseActiveMasks_2[gMotherBoss.phase];
    sMotherBossFadeTimer = 0.0f;
    sMotherBossCinemaStarted = 0;
    s->totalHealth = s->health = s->slots[13].health = 0x1F400;
    gMotherBoss.activeMask = mask;
    if (ai->m_Mode != 0x13) {
        resetSlots(gMotherBoss.activeMask, sMotherBossSpotResetScript_2);
    }
}

void MotherBoss_StartPhase2Cinema_2(void *obj, CAI *ai) {
    MotherBossState *state = &gMotherBoss;
    CGameObjectInstance *actor;
    s32 i;

    for (i = 0; i < state->count; i++) {
        actor = state->actors[i];
        actor->m_AI.m_Mother.m_GrowScale = 1.0f;
        actor->m_AI.m_Mother.m_PulseScale = 0.0f;
    }
    enterPhase2(&gMotherBoss, ai);
    Cinema_SetNextCallbacks(0xDC, MotherBoss_OnPhase2CinemaDone_2, NULL);
}

typedef struct {
    char pad0[0xC];
    s32 flags;                  /* 0x0C */
} Item;

s32 MotherBoss_PickWarpAnim_2(void *unused, Item *item) {
    s32 flags = item->flags;
    s32 bit2 = flags & 2;
    s32 result;

    if (flags & 0x20) {
        if (!bit2) {
            result = 0x56C2;
        } else {
            return 0x5640;
        }
    } else if (bit2) {
        result = 0x56B8;
    } else {
        result = 0x56A4;
    }
    return result;
}
void MotherBoss_SetupPhase0_2(void *obj, CAI *ai);
void MotherBoss_OnIntroCinemaDone_2(void);

void MotherBoss_StartIntro_2(void *obj, void *ai) {
    MotherBossState *state = &gMotherBoss;
    CGameObjectInstance *actor;
    s32 i;

    state->count = CScene__FindInstances(&gScene, 0x44E, 9, -1, state->actors, 4);
    for (i = 0; i < state->count; i++) {
        actor = state->actors[i];
        actor->m_AI.m_pfDraw = MotherBoss_Draw_2;
        actor->m_AI.m_pfEffects = MotherBoss_PartEffects_2;
    }
    MotherBoss_SetupPhase0_2(obj, ai);
    Cinema_SetNextCallbacks(0xC8, MotherBoss_OnIntroCinemaDone_2, NULL);
}

void MotherBoss_SetupPhase0_2(void *obj, CAI *ai) {
    MotherBossState *state = &gMotherBoss;
    s32 mask;

    state->minRadius = 0x200;
    state->maxRadius = 0x600;
    state->phase = 0;
    state->counter = 0;
    ai->m_VisBits = sMotherBossPhaseVisBits_2[state->phase];
    state->liveMask = sMotherBossPhaseLiveMasks_2[state->phase];
    mask = sMotherBossPhaseActiveMasks_2[state->phase];
    state->slots[2].health = state->slots[3].health = state->slots[4].health = 0x2800;
    state->slots[6].health = state->slots[7].health = state->slots[8].health = 0x2800;
    state->totalHealth = state->health = 0xF000;
    state->activeMask = mask;
    if (ai->m_Mode != 0) {
        resetSlots(state->activeMask, sMotherBossSpotResetScript_2);
    }
}

/* AI state: once the anim is done, reset every active slot of the module state, then go to anim state 2. */

extern s32 gMotherBoss_activeMask; /* local type: the field's real type (or the member path) changes the ROM's code here */ /* active slot mask */

void MotherBoss_Mode01_FlashSpotsThenIdle_2(void *obj, CAI *ai) {
    MotherBossState *state;
    s32 mask;
    s32 i;

    if (ai->m_AnimRepeatedCount > 0) {
        mask = gMotherBoss_activeMask;
        state = &gMotherBoss;
        for (i = 0; i < 15; i++) {
            if ((mask >> i) & 1) {
                ScriptAnimator_SetScript(&state->slots[i].anim, sMotherBossSpotFlashScript_2);
            }
        }
        CAI__SetMode(obj, ai, 2);
    }
}

void MotherBoss_Mode04_TrackTarget_2(void *obj, CAI *ai) {
    CAITargets t;

    AI_UpdateTargets(obj, ai, &t, 0, 1);
    AI_TurnClamped(obj, ai, 0.261799425f, t.m_Sight.m_Angle);
    if (ai->m_AnimRepeatedCount) {
        CAI__SetMode(obj, ai, 2);
    }
}

void MotherBoss_Mode05_TrackTarget_2(void *obj, CAI *ai) {
    CAITargets t;

    AI_UpdateTargets(obj, ai, &t, 0, 1);
    AI_TurnClamped(obj, ai, 0.261799425f, t.m_Sight.m_Angle);
    if (ai->m_AnimRepeatedCount) {
        CAI__SetMode(obj, ai, 2);
    }
}

void MotherBoss_Mode06_RecallParts_2(CGameObjectInstance *obj, CAI *ai) {
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
void MotherBoss_OnPhase1CinemaDone_2(void);

void MotherBoss_StartPhase1Cinema_2(void *obj, void *ai) {
    MotherBossState *state = &gMotherBoss;
    CGameObjectInstance *actor;
    s32 i;

    for (i = 0; i < state->count; i++) {
        actor = state->actors[i];
        actor->m_AI.m_Mother.m_GrowScale = 0;
        actor->m_AI.m_Mother.m_PulseScale = 0;
    }
    MotherBoss_SetupPhase1_2(obj, ai);
    Cinema_SetNextCallbacks(0xD2, MotherBoss_OnPhase1CinemaDone_2, NULL);
}

void MotherBoss_Mode10_FlashSpotsThenIdle_2(void *obj, CAI *ai) {
    MotherBossState *state;
    s32 mask;
    s32 i;

    if (ai->m_AnimRepeatedCount > 0) {
        mask = gMotherBoss_activeMask;
        state = &gMotherBoss;
        for (i = 0; i < 15; i++) {
            if ((mask >> i) & 1) {
                ScriptAnimator_SetScript(&state->slots[i].anim, sMotherBossSpotFlashScript_2);
            }
        }
        CAI__SetMode(obj, ai, 0xC);
    }
}

void MotherBoss_Mode03_TrackTarget_2(void *obj, CAI *ai) {
    CAITargets t;

    AI_UpdateTargets(obj, ai, &t, 0, 1);
    AI_TurnClamped(obj, ai, 0.261799425f, t.m_Sight.m_Angle);
    if (ai->m_AnimRepeatedCount) {
        CAI__SetMode(obj, ai, 2);
    }
}

void func_00444308(void) {

}

void MotherBoss_Mode07_Enter_2(CGameObjectInstance *arg0) {
    arg0->m_dwHotPointFlags = (s32) (arg0->m_dwHotPointFlags | 0x08000000);
}

void MotherBoss_Mode07_Step_2(CGameObjectInstance *arg0, CAI *arg1) {
    if (arg1->m_AnimRepeatedCount != 0) {
        arg0->m_dwHotPointFlags = (s32) (arg0->m_dwHotPointFlags & 0xF7FFFFFF);
        CAI__SetMode(arg0, arg1, 2);
    }
}

void MotherBoss_Mode08_Enter_2(CGameObjectInstance *arg0) {
    arg0->m_dwHotPointFlags = (s32) (arg0->m_dwHotPointFlags | 0x10000000);
}

void MotherBoss_Mode08_Step_2(CGameObjectInstance *arg0, CAI *ai) {
    if (ai->m_AnimRepeatedCount != 0) {
        arg0->m_dwHotPointFlags = (s32) (arg0->m_dwHotPointFlags & 0xEFFFFFFF);
        CAI__SetMode(arg0, ai, 2);
    }
}

void MotherBoss_Mode11_TrackTarget_2(void *obj, CAI *ai) {
    CAITargets t;

    AI_UpdateTargets(obj, ai, &t, 0, 1);
    AI_TurnClamped(obj, ai, 0.261799425f, t.m_Sight.m_Angle);
    if (ai->m_AnimRepeatedCount) {
        CAI__SetMode(obj, ai, 0xC);
    }
}

void MotherBoss_Mode13_TrackTarget_2(void *obj, CAI *ai) {
    CAITargets t;

    AI_UpdateTargets(obj, ai, &t, 0, 1);
    AI_TurnClamped(obj, ai, 0.261799425f, t.m_Sight.m_Angle);
    if (ai->m_AnimRepeatedCount) {
        CAI__SetMode(obj, ai, 0xC);
    }
}

void MotherBoss_Mode16_TeleportTarget_2(s32 arg0, CAI *ai)
{
  MotherBossState *new_var;
  s32 temp_v0;
  if (ai->m_AnimRepeatedCount != 0)
  {
    new_var = &gMotherBoss;
    if (gMotherBoss.counter == ((gMotherBoss.counter / 3) * 3))
    {
      MotherBoss_TeleportTarget_2(arg0, new_var->counter == 0);
    }
    temp_v0 = gMotherBoss.counter + 1;
    gMotherBoss.counter = temp_v0;
    if (temp_v0 >= 0x1F)
    {
      CAI__SetMode(arg0, ai, 0x11);
    }
  }
}

void MotherBoss_Mode17_TrackTarget_2(void *obj, CAI *ai) {
    CAITargets t;

    gMotherBoss.counter = 0;
    AI_UpdateTargets(obj, ai, &t, 0, 1);
    AI_TurnClamped(obj, ai, 0.261799425f, t.m_Sight.m_Angle);
    if (ai->m_AnimRepeatedCount) {
        CAI__SetMode(obj, ai, 0xC);
    }
}

typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s8 state;
} ModeData;

void MotherBoss_Mode14_ChargeStep_2(s32 arg0, CAI *ai) {
    ((ModeData *) ai->m_pModeCurrent->m_pData)->state = 7;
    if (ai->m_AnimRepeatedCount != 0) {
        CAI__SetMode(arg0, ai, 0xB);
    }
}

void MotherBoss_Mode18_TrackTarget_2(void *obj, CAI *ai) {
    CAITargets t;

    AI_UpdateTargets(obj, ai, &t, 0, 1);
    AI_TurnClamped(obj, ai, 0.261799425f, t.m_Sight.m_Angle);
    if (ai->m_AnimRepeatedCount) {
        CAI__SetMode(obj, ai, 0xC);
    }
}

void MotherBoss_SetupPhase2_2(void *obj, CAI *ai) {
    MotherBossState *state = &gMotherBoss;
    s32 mask;

    state->minRadius = 0x200;
    state->maxRadius = 0x600;
    state->phase = 2;
    state->counter = 0;
    ai->m_VisBits = sMotherBossPhaseVisBits_2[state->phase];
    state->liveMask = sMotherBossPhaseLiveMasks_2[state->phase];
    mask = sMotherBossPhaseActiveMasks_2[state->phase];
    state->totalHealth = state->health = state->slots[13].health = 0x1F400;
    state->activeMask = mask;
    if (ai->m_Mode != 0x13) {
        resetSlots(state->activeMask, sMotherBossSpotResetScript_2);
    }
}

void MotherBoss_Mode20_Step_2(s32 arg0, CAI *arg1) {
    if (arg1->m_AnimRepeatedCount != 0) {
        CAI__SetMode(arg0, arg1, 0x15);
    }
}

void MotherBoss_Mode21_FlashSpotsThenIdle_2(void *obj, CAI *ai) {
    MotherBossState *state;
    s32 mask;
    s32 i;

    if (ai->m_AnimRepeatedCount != 0) {
        mask = gMotherBoss_activeMask;
        state = &gMotherBoss;
        for (i = 0; i < 15; i++) {
            if ((mask >> i) & 1) {
                ScriptAnimator_SetScript(&state->slots[i].anim, sMotherBossSpotFlashScript_2);
            }
        }
        CAI__SetMode(obj, ai, 0x16);
    }
}

void MotherBoss_Mode23_Approach_2(void *obj, void *ai) {
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

void MotherBoss_Mode24_GoToCenter_2(CGameObjectInstance *obj, void *ai) {
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

void MotherBoss_Mode25_TrackTarget_2(void *obj, CAI *ai) {
    CAITargets t;

    AI_UpdateTargets(obj, ai, &t, 0, 1);
    AI_TurnClamped(obj, ai, 0.261799425f, t.m_Sight.m_Angle);
    if (ai->m_AnimRepeatedCount) {
        CAI__SetMode(obj, ai, 0x16);
    }
}

void MotherBoss_Mode26_TrackTarget_2(void *obj, CAI *ai) {
    CAITargets t;

    AI_UpdateTargets(obj, ai, &t, 0, 1);
    AI_TurnClamped(obj, ai, 0.261799425f, t.m_Sight.m_Angle);
    if (ai->m_AnimRepeatedCount) {
        CAI__SetMode(obj, ai, 0x16);
    }
}

void MotherBoss_Mode27_TrackTarget_2(void *obj, CAI *ai) {
    CAITargets t;

    AI_UpdateTargets(obj, ai, &t, 0, 1);
    AI_TurnClamped(obj, ai, 0.261799425f, t.m_Sight.m_Angle);
    if (ai->m_AnimRepeatedCount) {
        CAI__SetMode(obj, ai, 0x16);
    }
}

void MotherBoss_Mode28_Enter_2(void) {

}

void MotherBoss_Mode28_TeleportTarget_2(void *obj, CAI *ai) {
    CAITargets t;
    MotherBossState *state;

    AI_UpdateTargets(obj, ai, &t, 0, 1);
    AI_TurnClamped(obj, ai, 0.261799425f, t.m_Sight.m_Angle);
    state = &gMotherBoss;
    if (state->counter % 3 == 0) {
        MotherBoss_TeleportTarget_2(obj, state->counter == 0);
    }
    state->counter++;
    if (ai->m_AnimRepeatedCount) {
        state->counter = 0;
        CAI__SetMode(obj, ai, 0x16);
    }
}

void MotherBoss_Mode29_TrackTarget_2(void *obj, CAI *ai) {
    CAITargets t;

    AI_UpdateTargets(obj, ai, &t, 0, 1);
    AI_TurnClamped(obj, ai, 0.261799425f, t.m_Sight.m_Angle);
    if (ai->m_AnimRepeatedCount) {
        CAI__SetMode(obj, ai, 0x16);
    }
}

/* AI update for the module's actor: runs the AI step and the current anim's callback; in phase 0, a target
 * closer than 1228.8 makes the gScene_m_InstancePool_m_ActiveList actors drop their health; then updates the module (MotherBoss_UpdateSpotsAndPickups_2). */

extern void AI_TurnHeadToTarget(void *obj, CAI *ai, f32 pitchOffset);
void MotherBoss_KillMinions_2(void);

void MotherBoss_Step_2(void *obj, CAI *ai) {
    MotherBossState *state = &gMotherBoss;
    void *found;
    f32 dist;

    AI_TurnHeadToTarget(obj, ai, 0.0f);
    if (ai->m_pModeCurrent != NULL && ai->m_pModeCurrent->m_pfCallback != NULL) {
        ai->m_pModeCurrent->m_pfCallback(obj, ai);
    }
    if (state->phase == 0) {
        dist = MotherBoss_GetTargetDist_2(obj, &found);
        if (found != NULL && dist < 1228.7999f) {
            MotherBoss_KillMinions_2();
        }
    }
    MotherBoss_UpdateSpotsAndPickups_2(&gMotherBoss);
}

void MotherBoss_KillMinions_2(void) {
    CListedInstance *var_s0;

    var_s0 = gScene.m_InstancePool.m_ActiveList.m_pHead;
    if (var_s0 != NULL) {
        do {
            if (var_s0->m_Instance.m_AI.m_Health > 0) {
                var_s0->m_Instance.m_AI.m_Health = 0;
                CAI__SetMode(&var_s0->m_Instance, &var_s0->m_Instance.m_AI, 0x34);
            }
            var_s0 = var_s0->m_pNext;
        } while (var_s0 != NULL);
    }
}

f32 MotherBoss_GetTargetDist_2(CGameObjectInstance *obj, CGameObjectInstance **found) {
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

void MotherBoss_StartSpotAnims_2(void *obj, void *src, s32 mask) {
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

void MotherBoss_Mode30_DeathStep_2(void *obj, CAI *ai) {
    CPlayerPool__SetInvincibility(&gEngineApp_m_Game.m_PlayerList, 900.0f);
    if (ai->m_AnimRepeatedCount > 0) {
        CAI__SetMode(obj, ai, 0x1F);
    }
}
void MotherBoss_OnDefeatCinemaDone_2(void);

void MotherBoss_StartDefeatCinema_2(void) {
    CPlayerPool__SetInvincibility(&gEngineApp_m_Game.m_PlayerList, 900.0f);
    Cinema_SetNextCallbacks(0xE6, MotherBoss_OnDefeatCinemaDone_2, NULL);
}

/* Draw callback of the module's actors: draws each body part (anim group) whose slot is active in the
 * module object's mask, refreshing the slot's state first unless a cinema path is playing. */


extern void CGameObjectInstance__DrawShadow(CGameObjectInstance *actor, void *ai, CDrawArgs *req);
extern void ScriptAnimator_Apply(void *slotData);
extern void ScreenTint_UpdateMode(void);

void MotherBoss_Draw_2(CGameObjectInstance *actor, void *ai, CDrawArgs *req) {
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

void MotherBoss_Collision_2(CGameObjectInstance *obj, CAI *ai) {
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

void MotherBoss_RandomArenaPoint_2(CVector3 *out) {
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

void MotherBoss_RandomSpawnPoint_2(CVector3 *out) {
    CGameObjectInstance *found;
    s32 ok;
    s32 i;

    MotherBoss_GetTargetDist_2(gMotherBoss.obj, &found);
    do {
        ok = 1;
        i = Random_Blend(0.0f, 0.99999f) * 4.0f;
        if (found != NULL) {
            CVector3__Subtract(out, &found->header.m_vPos, &gMotherSpawnPoints_2[i].pos);
            if (func_00298470(out->x * out->x + out->y * out->y + out->z * out->z) < 460.8f) {
                ok = 0;
            }
        }
    } while (!ok);
    *out = gMotherSpawnPoints_2[i].pos;
}

void MotherBoss_SpawnPickup_2(s32 id, s32 lifetime, CVector3 pos) {
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

/* MotherBoss_RandomArenaPoint_2 */
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

/* MotherBoss_SpawnPickup_2 */
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

void MotherBoss_SpawnRandomPickup_2(s32 lifetime, Pick *table) {
    CVector3 pos;
    s32 id;

    id = WeightedTable_PickStatic(table);
    randomGroundPoint(&pos);
    spawnPickup(id, lifetime, pos);
}

/* Returns whether fewer than max actors are in the gScene_m_InstancePool_m_ActiveList list. */


s32 MotherBoss_FewerActorsThan_2(s32 max) {
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

extern void ImpactEvent_Send(); /* (src, dst, kind, id, CVector3 pos, 8-byte params): MotherBoss_TeleportTarget_2's file names the params Part */
typedef void (*ImpactSendFn)(void *source, void *p, s32 effect, s32 arg3, CVector3 pos, ActionParams params);

void MotherBoss_SendImpactAt_2(CGameObjectInstance *obj, CGameObjectInstance *other, s32 value) {
    ActionParams params;

    params.upper = 0xA00;
    params.lower = value << 8;
    params.extra[0] = params.extra[1] = params.extra[2] = params.extra[3] = 0;
    ((ImpactSendFn) ImpactEvent_Send)(obj, obj, 3, -1, other->header.m_vPos, params);
}

void MotherBoss_TurnToTarget_2(void *obj, void *ai) {
    CAITargets t;

    AI_UpdateTargets(obj, ai, &t, 0, 1);
    AI_TurnClamped(obj, ai, 0.261799425f, t.m_Sight.m_Angle);
}

void MotherBoss_FlashActiveSpots_2(void) {
    MotherBossState *state = &gMotherBoss;

    resetSlots(state->activeMask, sMotherBossSpotFlashScript_2);
}

void MotherBoss_OnIntroCinemaDone_2(void) {
    CAI__SetMode(gMotherBoss.obj, &gMotherBoss.obj->m_AI, 1);
}

void MotherBoss_TurnThenPhase2Choose_2(void *obj, CAI *ai) {
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
static __inline__ f32 getTargetDist_4456F4(CGameObjectInstance *obj, CGameObjectInstance **found) {
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

void MotherBoss_OnPhase1CinemaDone_2(void) {
    CGameObjectInstance *obj = gMotherBoss.obj;
    CGameObjectInstance *found;
    CPlayer *player;

    player = NULL;
    getTargetDist_4456F4(obj, &found);
    if (found != NULL) {
        player = ((CPlayer *)found->m_AI.m_pParent);
    }
    if (player != NULL) {
        CPlayer__SetPosition(player, sMotherBossPhase1PlayerPos_2);
        CPlayer__UpdateOrbitCamera(player, player);
        player->m_WeaponActor.m_RotY = player->m_CharacterActor.m_RotY = 3.141593f;
        obj->m_RotY = 0.0f;
    }
    CAI__SetMode(obj, &obj->m_AI, 0xA);
}

void MotherBoss_OnPhase2CinemaDone_2(void) {
    CAI__SetMode(gMotherBoss.obj, &gMotherBoss.obj->m_AI, 0x14);
}

s32 CPlayer__Warp(s32, s32, s32, s32);

void MotherBoss_OnDefeatCinemaDone_2(void) {
    CPlayer *temp_v0;
    GameSettings *temp_v1;

    CAI__SetMode(gMotherBoss.obj, &gMotherBoss.obj->m_AI, 1);
    temp_v0 = CPlayerPool__GetFirstActive(&gEngineApp_m_Game.m_PlayerList);
    if (temp_v0 != NULL) {
        temp_v1 = &gEngineApp_m_Game.m_Settings;
        temp_v0->m_Data.m_bMotherBossComplete = 1;
        temp_v1->unlockedCheats |= 4;
        CPlayer__Warp((sptr) temp_v0, 0x1B57, 2, 0);
    }
}
