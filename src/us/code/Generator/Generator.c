#include "common.h"
#include "globals/game.h"

/* Whether the unlock flag behind an id is set; ids without a flag count as set. */
s32 Generator_IsPickupUnlocked(s32 id) {
    switch (id) {
        default:
            return 1;
        case 0x5EB:
        case 0x842:
            return gEngineApp_m_Settings.weaponEnabled[1] == 1;
        case 0x5EC:
        case 0x843:
            return gEngineApp_m_Settings.weaponEnabled[2] == 1;
        case 0x5ED:
        case 0x844:
            return gEngineApp_m_Settings.weaponEnabled[3] == 1;
        case 0x5EE:
        case 0x845:
            return gEngineApp_m_Settings.weaponEnabled[4] == 1;
        case 0x5EF:
        case 0x846:
            return gEngineApp_m_Settings.weaponEnabled[5] == 1;
        case 0x5F0:
        case 0x847:
            return gEngineApp_m_Settings.weaponEnabled[6] == 1;
        case 0x5F1:
        case 0x848:
            return gEngineApp_m_Settings.weaponEnabled[7] == 1;
        case 0x5F3:
        case 0x84A:
            break;
    }
    return gEngineApp_m_Settings.weaponEnabled[9] == 1;
}

#include "structs/pick.h"
#include "structs/game_object.h"
#include "structs/vector.h"
#include "structs/intelligence.h"
#include "structs/game.h"
#include "structs/level_actor.h"
#include "globals/scene.h"
#include "globals/ai.h"
#include "protos/player.h"
#include "protos/game.h"
#include "protos/scene.h"
#define GAME_SETTINGS_OFF T2_OFFSETOF(CGame, m_Settings)

/* Drops an actor's loot at its feet (raised to the ground height): kind 0
 * spawns the definition's object (or, for a pickup-type entry, a pool pickup
 * if it is unlocked), kind 1 a particle effect, kind 2 a pickup picked at
 * random from the weapon/ammo/item tables the definition enables. */

extern f32 CGameRegion__GetHeight(sptr region, f32 x, f32 z);
#ifdef TARGET_PORT
#define getObjectTypeID CScene__GetObjectTypeID /* ABI: returns u16 (the host leaves the upper bits of the return register undefined) */
#else
/* The ROM's caller takes the u16 result as an s32 (no andi): called through a type of its own. */
#define getObjectTypeID ((s32 (*)(struct CScene *, s32))CScene__GetObjectTypeID)
#endif

void *CLevelActorPool__Spawn(CLevelActorPool *pool, void *refCount, s32 id, CVector3 vel, CVector3 pos, sptr region, f32 lifetime);
void Actor_SpawnParticle(void *actor, s32 event, CVector3 pos, s32 arg);
extern s32 CPlayer__HasRoomForAmmo(void *player, s32 id);
extern s32 CPlayer__IsBelowFullHealth(void *player, s32 id);
extern s16 WeightedTable_PickStatic(Pick *table);
extern void WeightedTable_Clear(PickTable *t);
extern s32 WeightedTable_Add(PickTable *t, s16 id, s16 weight);
extern s16 WeightedTable_Pick(PickTable *t);

/* Spawns pool pickup id at pos. */
static __inline__ void spawnPickup(CGameObjectInstance *actor, CAI *ai, CVector3 pos, sptr region, s32 id) {
    CPooledLevelActor *pickup;

    pickup = CLevelActorPool__Spawn(&gScene.m_ActorPool, &ai->m_Generator.m_nActive, id, actor->m_vVelocity, pos, region, 0.0f);
    if (pickup != NULL) {
        ai->m_Generator.m_nRemaining--;
        Actor_SpawnParticle(pickup, 0x11D, pickup->m_Actor.header.m_vPos, 0);
    }
}

/* Spawns the object model (an intelligence entry) at pos. */
static __inline__ void spawnObject(CGameObjectInstance *actor, CAI *ai, CVector3 pos, CVector3 scale, sptr region, s32 type, s32 *model) {
    if (CInstancePool__Spawn(&gScene.m_InstancePool, type, 0, model, region, pos, actor->m_RotY, actor->m_vVelocity, scale, &ai->m_Generator.m_nActive)) {
        ai->m_Generator.m_nRemaining--;
    }
}

/* Spawns the effect at pos. */
static __inline__ void spawnEffect(CGameObjectInstance *actor, CAI *ai, CVector3 pos, sptr region, s32 type) {
    ai->m_Generator.m_nRemaining -= CParticleSystem__Spawn(&gScene.m_ParticleSystem, actor, actor, &ai->m_Generator.m_nActive, 0, type, actor->m_vVelocity, actor->m_qGround, pos, region,
                               -1, 0);
}

void Generator_SpawnEnter(CGameObjectInstance *actor, CAI *ai) {
    CVector3 pos;
    CVector3 scale;
    PickTable picks;
    PickTable unlocked;
    CGeneratorIntelligence *info;
    sptr region;
    s32 *model;
    s32 id;
    f32 ground;
    s32 weapon;
    s32 ammo;
    s32 item;
    u16 pickId; /* loaded as u16, passed as s16: the ROM's lhu/sll/sra */
    GameSettings *flags;
    Pick *e;
    void *player;

    info = &actor->header.m_pIntelligence->m_Generator;
    pos = actor->header.m_vPos;
    region = actor->header.m_pCurrentRegion;
    if (region != 0) {
        ground = CGameRegion__GetHeight(region, actor->header.m_vPos.x, actor->header.m_vPos.z);
        if (pos.y < ground) {
            pos.y = ground;
        }
    }
    switch (info->m_Type) {
    case 0:
        model = (s32 *)CScene__GetIntelligence(&gScene, info->m_VariationIndex);
        switch (*model) {
        case 8:
            id = getObjectTypeID(&gScene, info->m_ObjectIndex);
            if (Generator_IsPickupUnlocked(id)) {
                spawnPickup(actor, ai, pos, region, id);
            }
            break;
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 9:
        case 10:
            scale.x = info->m_ScaleX;
            scale.y = info->m_ScaleY;
            scale.z = info->m_ScaleZ;
            spawnObject(actor, ai, pos, scale, region, info->m_ObjectIndex, model);
            break;
        }
        break;
    case 1:
        spawnEffect(actor, ai, pos, region, info->m_ParticleType);
        break;
    case 2:
        weapon = -1;
        ammo = -1;
        item = -1;
        flags = &gEngineApp_m_Settings;
        if (flags->multiplayer) {
            if (info->m_dwFlags & 0x20) {
                weapon = WeightedTable_PickStatic(sGeneratorMultiAmmoPicks);
            }
            if (info->m_dwFlags & 0x40) {
                ammo = WeightedTable_PickStatic(sGeneratorMultiHealthPicks);
            }
            if (info->m_dwFlags & 0x80) {
                WeightedTable_Clear(&unlocked);
                for (e = sGeneratorMultiItemPicks; e->id != -1; e++) {
                    pickId = e->id;
                    if (Generator_IsPickupUnlocked((s16)pickId)) {
                        WeightedTable_Add(&unlocked, e->id, e->weight);
                    }
                }
                item = WeightedTable_Pick(&unlocked);
            }
        } else {
            player = CPlayerPool__GetFirstActive((u8 *)flags - GAME_SETTINGS_OFF); /* the player list, gEngineApp_m_Game */
            if (player == NULL) {
                break;
            }
            if (info->m_dwFlags & 0x20) {
                WeightedTable_Clear(&picks);
                for (e = sGeneratorAmmoPicks; e->id != -1; e++) {
                    pickId = e->id;
                    if (CPlayer__HasRoomForAmmo(player, (s16)pickId)) {
                        WeightedTable_Add(&picks, e->id, e->weight);
                    }
                }
                weapon = WeightedTable_Pick(&picks);
            }
            if (info->m_dwFlags & 0x40) {
                WeightedTable_Clear(&picks);
                e = sGeneratorHealthPicks;
                if (gScene.m_bStreamAll) {
                    e = sGeneratorHealthPicksReduced;
                }
                for (; e->id != -1; e++) {
                    pickId = e->id;
                    if (CPlayer__IsBelowFullHealth(player, (s16)pickId)) {
                        WeightedTable_Add(&picks, e->id, e->weight);
                    }
                }
                ammo = WeightedTable_Pick(&picks);
            }
            if (info->m_dwFlags & 0x80) {
                item = WeightedTable_PickStatic(sGeneratorItemPicks);
            }
        }
        WeightedTable_Clear(&picks);
        if (weapon != -1) {
            WeightedTable_Add(&picks, weapon, 100);
        }
        if (ammo != -1) {
            WeightedTable_Add(&picks, ammo, 100);
        }
        if (item != -1) {
            WeightedTable_Add(&picks, item, 100);
        }
        if (picks.count != 0) {
            spawnPickup(actor, ai, pos, region, WeightedTable_Pick(&picks));
        }
        break;
    }
}

/* Turok 2: def 0x14, type id 0xB8, flags 0xD4 (Rage Wars 0x18 / 0xE4 / 0x100). */
/* Turok 2: update hook at 0xF0 (Rage Wars 0x108). */
#ifdef TARGET_PORT
/* The port: the view at the host offsets of CAI (m_pModeTable, m_pfStep). */
typedef struct ActorMotionState {
    union {
        struct { u8 _hp_table[__builtin_offsetof(CAI, m_pModeTable)]; void *table; };
        struct { u8 _hp_update[__builtin_offsetof(CAI, m_pfStep)]; void *update; };
    };
} ActorMotionState;
#else
typedef struct ActorMotionState {
    char pad0[0x2C];
    void *table;
    char pad30[0xC0];
    void *update;
} ActorMotionState;
#endif

void Generator_Step(sptr arg0, CAI *ai);

void Generator_Setup(void *arg0, void *arg1) {
    CGameObjectInstance *actor = arg0;
    ActorMotionState *state = arg1;
    CDestructibleIntelligence *inner = &actor->header.m_pIntelligence->m_Destructible;
    state->table = gGeneratorModeTable;
    state->update = &Generator_Step;
    if ((actor->m_nTypeFlag == 0x644) ||
        (inner->m_dwFlags & 8)) {
        actor->m_dwFlags &= ~0x2000;
    }
}

#include "protos/ai.h"
#include "structs/word.h"
#include "structs/generator.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00205F18.c, func_00205F18.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

f32 Random_Blend(f32 a, f32 b);

void Generator_Advance(CGameObjectInstance *actor, CAI *state) {
    CGameObjectInstance *a2;
    B01Info *new_var;
    B01Info *info;
    a2 = actor;
    new_var = (B01Info *)(((u8 *)a2->header.m_pIntelligence) + 0x14);
    if (!(a2->header.m_pIntelligence->m_dwTypeFlags & 0x10) || (gEngineApp_m_Settings.multiplayer != 0 && gEngineApp_m_Settings.playerCount == 1)) {
        if (state) {
            state->m_ConditionTimer = Random_Blend(45.0f, 60.0f);
            info = new_var;
            state->m_VarS32[0] = 0;
            state->m_VarS32[1] = info->unk8;
        } else {
            info = new_var;
            state->m_ConditionTimer = Random_Blend(45.0f, 60.0f);
            state->m_VarS32[0] = 0;
            state->m_VarS32[1] = info->unk8;
        }
        if (a2->m_nTypeFlag == 0x644 || (info->flags & 8)) {
            a2->m_dwFlags &= ~0x2000;
        }
        CAI__SetMode(a2, state, 0);
        if ((info->flags & 0x10) && gEngineApp_m_Settings.playerCount != 1) {
            goto clear;
        }
    } else {
clear:
        a2->m_dwFlags &= ~0x100;
    }
}

void Generator_Step(sptr arg0, CAI *ai) {
    s32 (*temp_v0_2)(sptr, void *);
    CAIModeInfo *temp_v0;

    if ((CScene__GetInstanceSavedBit(&gScene, (void *) arg0) ^ 1) == 0) {
        temp_v0 = ai->m_pModeCurrent;
        if (temp_v0 != NULL) {
            temp_v0_2 = temp_v0->m_pfCallback;
            if (temp_v0_2 != NULL) {
                temp_v0_2(arg0, ai);
            }
        }
    }
}

extern f32 Random_Blend(f32 lo, f32 hi); /* random in range */

/* AI init: a random delay from the definition's range, shortened by 25% per
 * player beyond two in multiplayer. */
void Generator_WaitEnter(CGameObjectInstance *actor, CAI *ai) {
    CGeneratorIntelligence *trigger = &actor->header.m_pIntelligence->m_Generator;
    f32 delay;
    GameSettings *opts;

    delay = Random_Blend(trigger->m_IntervalTime1, trigger->m_IntervalTime2);
    ai->m_ConditionTimer = delay;
    opts = &gEngineApp_m_Game.m_Settings;
    if (opts->multiplayer && (u32) gEngineApp_m_Game.m_PlayerList.m_ActiveList.m_nCount >= 3) {
        ai->m_ConditionTimer = delay * 0.01f * (f32) (u32) (100 - ((u32) gEngineApp_m_Game.m_PlayerList.m_ActiveList.m_nCount - 2) * 25);
    }
}

void Generator_WaitUpdate(CGameObjectInstance *actor, CAI *state) {
    B01Info *info = ((B01Info *)((u8 *)(actor->header.m_pIntelligence) + 0x14));
    if (info->unk4 == 0) {
        if (info->unk14 == -1) {
            state->m_ModeTime = 0.0f;
            return;
        }
        if (info->unk16 == -1) {
            state->m_ModeTime = 0.0f;
            return;
        }
    }
    if (state->m_VarS32[0] == info->unkA) {
        state->m_ModeTime = 0.0f;
        return;
    }
    if ((info->flags & 1) || state->m_VarS32[1] > 0) {
        if ((info->flags & 2) && (actor->m_dwFlags & 0x200)) {
            state->m_ModeTime = 0.0f;
            return;
        }
        if (state->m_ModeTime >= state->m_ConditionTimer) {
            CAI__SetMode(actor, state, 1);
        }
    }
}

void Generator_SpawnUpdate(sptr arg0, CAI *ai) {
    if (ai->m_AnimRepeatedCount != 0) {
        CAI__SetMode(arg0, ai, 0);
    }
}

s32 Generator_IsOn(s32 arg0) {
    return CScene__GetInstanceSavedBit(&gScene, arg0) == 1;
}

void Generator_EventOn(sptr arg0) {
    CScene__SetInstanceSavedBit(&gScene, arg0, 1);
}

#include "structs/event.h"

void Generator_EventOnRemote(LevelEvent *arg0) {
    CScene__SetInstanceSavedBitInLevel(&gScene, arg0->level, arg0->actor, 1);
}

void Generator_EventOff(sptr arg0) {
    CScene__SetInstanceSavedBit(&gScene, arg0, 0);
}

void Generator_EventOffRemote(LevelEvent *arg0) {
    CScene__SetInstanceSavedBitInLevel(&gScene, arg0->level, arg0->actor, 0);
}

void Generator_SpawnPickup(CGameObjectInstance *actor, CAI *ai, CVector3 pos, sptr region, s32 id) {
    CPooledLevelActor *pickup;

    pickup = CLevelActorPool__Spawn(&gScene.m_ActorPool, &ai->m_Generator.m_nActive, id, actor->m_vVelocity, pos, region, 0.0f);
    if (pickup != NULL) {
        ai->m_Generator.m_nRemaining -= 1;
        Actor_SpawnParticle(pickup, 0x11D, pickup->m_Actor.header.m_vPos, 0);
    }
}

void Generator_SpawnObject(CGameObjectInstance *actor, CAI *ai, CVector3 pos, CVector3 scale, sptr region, s32 type, s32 *model) {
    if (CInstancePool__Spawn(&gScene.m_InstancePool, type, 0, model, (void *)region, pos, actor->m_RotY, actor->m_vVelocity, scale, &ai->m_Generator.m_nActive) != 0) {
        ai->m_Generator.m_nRemaining -= 1;
    }
}

void Generator_SpawnEffect(u8 *actor, u8 *state, CVector3 pos, s32 arg, s32 kind) {
    *(s32 *) (state + 0x110) -= CParticleSystem__Spawn(&gScene.m_ParticleSystem, actor, actor, state + 0x10C, 0, kind,
                                              *(CVector3 *) (actor + 0x18), *(CQuaternion *) (actor + 0x40), pos,
                                              arg, -1, 0);
}
