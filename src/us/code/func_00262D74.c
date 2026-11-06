#include "common.h"
#include "structs/pick.h"
#include "structs/game_object.h"
#include "structs/vector.h"
#include "structs/intelligence.h"
#include "structs/game.h"
#include "structs/level_actor.h"
#include "globals/game.h"
#include "globals/scene.h"
#include "globals/ai.h"

/* Drops an actor's loot at its feet (raised to the ground height): kind 0
 * spawns the definition's object (or, for a pickup-type entry, a pool pickup
 * if it is unlocked), kind 1 a particle effect, kind 2 a pickup picked at
 * random from the weapon/ammo/item tables the definition enables. */

extern f32 CGameRegion__GetHeight(s32 region, f32 x, f32 z);
extern s32 *CScene__GetIntelligence(void *level, s32 model);
extern s32 CScene__GetObjectTypeID(void *level, s32 id);
extern s32 Generator_IsPickupUnlocked(s32 id);
extern CPooledLevelActor *CLevelActorPool__Spawn(CLevelActorPool *pool, s32 *refCount, s32 id, CVector3 vel, CVector3 pos, s32 region, f32 lifetime);
extern void *CInstancePool__Spawn(void *world, s32 model, s32 type, void *intelligence, s32 region, CVector3 pos, f32 rotY,
                           CVector3 vel, CVector3 scale, s32 *owner);
extern s32 CParticleSystem__Spawn(CParticleSystem *sys, CGameObjectInstance *p, CGameObjectInstance *owner, s32 *ownerId, s32 target, s32 type, CVector3 dir,
                         CQuaternion rot, CVector3 pos, s32 region, s32 arg10, s32 flags);
extern void Actor_SpawnParticle(CPooledLevelActor *actor, s32 sound, CVector3 pos, s32 arg);
extern void *CPlayerPool__GetFirstActive(void *players);
extern s32 CPlayer__HasRoomForAmmo(void *player, s32 id);
extern s32 CPlayer__IsBelowFullHealth(void *player, s32 id);
extern s16 WeightedTable_PickStatic(Pick *table);
extern void WeightedTable_Clear(PickTable *t);
extern s32 WeightedTable_Add(PickTable *t, s16 id, s16 weight);
extern s16 WeightedTable_Pick(PickTable *t);

/* Spawns pool pickup id at pos. */
static __inline__ void spawnPickup(CGameObjectInstance *actor, CAI *ai, CVector3 pos, s32 region, s32 id) {
    CPooledLevelActor *pickup;

    pickup = CLevelActorPool__Spawn(&gScene.m_ActorPool, &ai->m_Generator.m_nActive, id, actor->m_vVelocity, pos, region, 0.0f);
    if (pickup != NULL) {
        ai->m_Generator.m_nRemaining--;
        Actor_SpawnParticle(pickup, 0x11D, pickup->m_Actor.header.m_vPos, 0);
    }
}

/* Spawns the object model (an intelligence entry) at pos. */
static __inline__ void spawnObject(CGameObjectInstance *actor, CAI *ai, CVector3 pos, CVector3 scale, s32 region, s32 type, s32 *model) {
    if (CInstancePool__Spawn(&gScene.m_InstancePool, type, 0, model, region, pos, actor->m_RotY, actor->m_vVelocity, scale, &ai->m_Generator.m_nActive)) {
        ai->m_Generator.m_nRemaining--;
    }
}

/* Spawns the effect at pos. */
static __inline__ void spawnEffect(CGameObjectInstance *actor, CAI *ai, CVector3 pos, s32 region, s32 type) {
    ai->m_Generator.m_nRemaining -= CParticleSystem__Spawn(&gScene.m_ParticleSystem, actor, actor, &ai->m_Generator.m_nActive, 0, type, actor->m_vVelocity, actor->m_qGround, pos, region,
                               -1, 0);
}

void Generator_SpawnEnter(CGameObjectInstance *actor, CAI *ai) {
    CVector3 pos;
    CVector3 scale;
    PickTable picks;
    PickTable unlocked;
    CGeneratorIntelligence *info;
    s32 region;
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
        model = CScene__GetIntelligence(&gScene, info->m_VariationIndex);
        switch (*model) {
        case 8:
            id = CScene__GetObjectTypeID(&gScene, info->m_ObjectIndex);
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
            player = CPlayerPool__GetFirstActive((u8 *)flags - 0x1268); /* the player list, gEngineApp_m_Game */
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
