#include "common.h"
#include "m2c_macros.h"
#include "globals/boss.h"

s32 TotemDefense_GetState_2(void) {
    return gTotemDefense_state;
}

#include "structs/boss.h"

void TotemDefense_AddKill_2(void) {
    s32 *kills = &gTotemDefense.kills; /* through a pointer: `gTotemDefense.kills++` loads it with %lo, the ROM builds the address first */

    *kills += 1;
}

#include "structs/game_object.h"
#include "structs/model.h"
#include "globals/scene.h"
#include "protos/cache.h"

void CGameObjectInstance__GetDeathModels(CGameObjectInstance *, s32, s32 *, s32 *);
void CGameObjectInstance__DrawShadow(CGameObjectInstance *, sptr, CDrawArgs *);
void Render_QueueModelInstance(sptr, sptr, s32, sptr, char *, s32, s32);
void Model_PulseVertexColors(char *, char *);
void CGameObjectInstance__ConstructLitVertices();

void TotemDefense_DrawTotem_2(CGameObjectInstance *actor, s32 arg1, CDrawArgs *params) {
    s32 **resource;
    s32 *header;
    char *object;
    char *data;

    CGameObjectInstance__GetDeathModels(actor, params->m_nVariant, &params->m_nModel, &params->m_nShadowModel);
    actor->m_AI.m_VisBits = 1 << params->m_nModel;
    if (params->m_bShadow != 0) {
        CGameObjectInstance__DrawShadow(actor, arg1, params);
    }
    if (params->m_bDraw == 0) {
        return;
    }
    resource = CCache__RequestDecompression(0, actor->m_dwUniqueID | gLitVerticesKey, params->m_hModel, 0, 0, actor,
                             CGameObjectInstance__ConstructLitVertices, "lit vertices", 1);
    if (resource == NULL) {
        return;
    }
    CCache__LockForDisplayList(0, resource);
    header = *resource;
    if (header[0] == 1) {
        object = (char *) header + header[1];
        Model_PulseVertexColors(object, (char *) header + header[2]);
        data = object;
    } else {
        data = (char *) (header + 2);
    }
    Render_QueueModelInstance(params->m_hModel, ((sptr)actor->m_pmtDrawMtxs), 1, 0, data, gTotemDefense.warned, -1);
    CCache__Unlock(0, resource);
}

#include "globals/menu_text.h"
#include "globals/onscreen.h"

/* The state this sets up for its object. */
typedef struct {
    /* 0x00 */ CGameObjectInstance *obj;
    /* 0x04 */ s32 info;                /* &obj's intelligence + 0x14 */
    /* 0x08 */ s32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
} ObjState;
sptr CGameObjectInstance__PickRandomAnim(sptr, sptr, sptr);
s32 COnScreen__SetBoss(sptr, sptr, sptr);
void COnScreen__SetBoss2(void *, void *, void *);         /* extern */

void TotemDefense_InitFight_2(ObjState *arg0, CGameObjectInstance *arg1)
{
  CAI *temp_a3;
  CGameObjectInstance *temp_s0;
  int new_var;
  arg0->obj = arg1;
  arg0->info = (sptr) &arg1->header.m_pIntelligence->m_Destructible;
  new_var = arg1->header.m_pIntelligence->m_Destructible.m_TotemMissionDuration * 0xF;
  arg0->unk8 = 0;
  arg0->unk10 = 0;
  arg0->unkC = (f32) new_var;
  arg1->m_AI.m_pfDraw = TotemDefense_DrawTotem_2;
  temp_s0 = arg0->obj;
  arg0->unk14 = 0;
  temp_a3 = &temp_s0->m_AI;
  temp_a3->m_AnimIndex = CGameObjectInstance__PickRandomAnim((sptr) temp_s0, 0x5334, -1);
  temp_a3->m_AnimType = 0x5334;
  temp_s0->m_AI.m_dwFlags = temp_s0->m_AI.m_dwFlags & (~1);
  temp_a3->m_AnimRepeatedCount = 0;
  COnScreen__SetBoss((sptr) &gEngineApp_m_OnScreen, (sptr) arg1, (sptr) sTotemDefenseText_Enemies);
  COnScreen__SetBoss2(&gEngineApp_m_OnScreen, arg1, sTotemDefenseText_Time);
}

#include "structs/boss.h"

/* The kills still wanted to save the totem. */
s32 TotemDefense_KillsLeft_2(TotemDefenseState *state) {
    return ((CDestructibleIntelligence *) state->def)->m_EnemiesToKillToSaveTotem - state->kills;
}

#include "globals/game.h"
#include "structs/twin_totemdefense.h"


/* Point the player actors at a target (mode 0), or back at the player. */
void TotemDefense_RetargetEnemies_2(TotemDefenseState *state, s32 force) {
    CListedInstance *actor;
    void *player;
    s32 count;

    if (state->state == 1) {
        return;
    }
    actor = gScene.m_InstancePool.m_ActiveList.m_pHead;
    player = gEngineApp_m_Game.m_PlayerList.m_ActiveList.m_pHead;
    count = 0;
    for (; actor != NULL; actor = actor->m_pNext) {
        if (actor->m_Instance.header.m_pIntelligence->m_Type != 1 || actor->m_Instance.m_AI.m_pTargetOverride != 0) {
            continue;
        }
        if (state->state == 0) {
            if (count < ((CDestructibleIntelligence *) state->def)->m_EnemiesAttackingTurok || force) {
                count++;
                actor->m_Instance.m_AI.m_pPathTarget = player;
                actor->m_Instance.m_AI.m_pSightTarget = player;
                actor->m_Instance.m_AI.m_dwFlags |= 0x20000000;
                actor->m_Instance.m_AI.m_dwFlags |= 0x40000000;
            } else {
                actor->m_Instance.m_AI.m_pSightTarget = state->obj;
                actor->m_Instance.m_AI.m_pPathTarget = state->obj;
                actor->m_Instance.m_AI.m_dwFlags |= 0x20000000;
                actor->m_Instance.m_AI.m_dwFlags |= 0x40000000;
            }
        } else if (actor->m_Instance.m_AI.m_pSightTarget == state->obj) {
            actor->m_Instance.m_AI.m_pPathTarget = player;
            actor->m_Instance.m_AI.m_pSightTarget = player;
            actor->m_Instance.m_AI.m_dwFlags |= 0x20000000;
            actor->m_Instance.m_AI.m_dwFlags |= 0x40000000;
        }
    }
}

#include "structs/collision.h"

extern f32 Random_Float(f32 scale);                                      /* random 0..scale */
extern void Actor_SpawnParticle(void *actor, s32 event, CVector3 pos, s32 arg);
extern void DamageEvent_Set(DamageEvent *event, void *source, s32 damage, f32 radius, s32 flags, void *shield);
extern void EnemyAI_OnDamage(CListedInstance *actor, void *ai, DamageEvent *event);

/* One time in four, hurt the first live player actor. */
void TotemDefense_TryHitEnemy_2(void) {
    DamageEvent event;
    CListedInstance *actor;

    if (Random_Float(100.0f) > 25.0f) {
        return;
    }
    for (actor = gScene.m_InstancePool.m_ActiveList.m_pHead; actor != NULL; actor = actor->m_pNext) {
        if (actor->m_Instance.header.m_pIntelligence->m_Type == 1 && actor->m_Instance.m_AI.m_Health != 0) {
            Actor_SpawnParticle(actor, 0x17C, actor->m_Instance.header.m_vPos, 0);
            DamageEvent_Set(&event, NULL, 0xFF00, 51.199997f, 0x100000, NULL);
            EnemyAI_OnDamage(actor, &actor->m_Instance.m_AI.m_dwFlags, &event);
            return;
        }
    }
}

#include "structs/player.h"
#include "structs/game.h"

void TotemDefense_StopEnemies_2(void) {
    CListedInstance *actor;

#ifdef TARGET_PORT
    gEngineApp_m_Game.m_PlayerList.m_ActiveList.m_pHead->m_FleeTime = 450.0f; /* N64 0xAAC */
#else
    *(f32 *) ((u8 *) gEngineApp_m_Game_m_PlayerList_m_ActiveList_m_pHead + 0xAAC) = 450.0f; /* the current player */
#endif
    for (actor = gScene.m_InstancePool.m_ActiveList.m_pHead; actor != NULL; actor = actor->m_pNext) {
        if (actor->m_Instance.header.m_pIntelligence->m_Type == 1 && actor->m_Instance.m_AI.m_Health != 0) {
            actor->m_Instance.m_AI.m_pPathTarget = NULL;
            actor->m_Instance.m_AI.m_pSightTarget = NULL;
            actor->m_Instance.m_AI.m_dwFlags |= 0x20000000;
            actor->m_Instance.m_AI.m_dwFlags |= 0x40000000;
        }
    }
}

#include "protos/scene.h"

void TotemDefense_StopGenerators_2(void) {
    CScene__UnmarkGeneratorsSaved(&gScene);
}

s32 TotemDefense_AnyEnemyAlive_2(void) {
    CListedInstance *actor;

    for (actor = gScene.m_InstancePool.m_ActiveList.m_pHead; actor != 0; actor = actor->m_pNext) {
        if (actor->m_Instance.header.m_pIntelligence->m_Type == 1 && actor->m_Instance.m_AI.m_Health != 0) {
            return 1;
        }
    }
    return 0;
}

#include "globals/engine.h"


/* After a delay, one time in four, hurt the first live player actor. */
void TotemDefense_TryHitEnemyAfterDelay_2(HurtTimer *h) {
    DamageEvent event;
    CListedInstance *actor;

    if (h->timer > 0.0f) {
        h->timer -= gFrameIncrement;
        return;
    }
    if (Random_Float(100.0f) > 25.0f) {
        return;
    }
    for (actor = gScene.m_InstancePool.m_ActiveList.m_pHead; actor != NULL; actor = actor->m_pNext) {
        if (actor->m_Instance.header.m_pIntelligence->m_Type == 1 && actor->m_Instance.m_AI.m_Health != 0) {
            Actor_SpawnParticle(actor, 0x17C, actor->m_Instance.header.m_vPos, 0);
            DamageEvent_Set(&event, NULL, 0xFF00, 51.199997f, 0x100000, NULL);
            EnemyAI_OnDamage(actor, &actor->m_Instance.m_AI.m_dwFlags, &event);
            return;
        }
    }
}

/* Looks for a live player actor; the result is not used. */
void TotemDefense_FindLiveEnemy_2(void) {
    CListedInstance *actor;

    for (actor = gScene.m_InstancePool.m_ActiveList.m_pHead; actor != NULL; actor = actor->m_pNext) {
        if (actor->m_Instance.header.m_pIntelligence->m_Type == 1 && actor->m_Instance.m_AI.m_Health != 0) {
            break;
        }
    }
}

void TotemDefense_OnEvent_2(void) {

}
