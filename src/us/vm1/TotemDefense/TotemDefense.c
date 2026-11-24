#include "common.h"
#include "structs/game_object.h"
#include "globals/onscreen.h"

/* The actor's CAI (at 0x140), with the fields this sets. */
typedef struct {
    /* 0x00 */ u32 flags;
    /* 0x04 */ u8 pad04[0xC0];
    /* 0xC4 */ s16 animType;
    /* 0xC6 */ s8 animIndex;
    /* 0xC7 */ s8 animRepeats;
} BossAI;
/* An actor, with the fields this sets (CAI at 0x140). */
typedef struct {
    /* 0x000 */ u8 pad000[0x14];
    /* 0x014 */ CIntelligence *def;
    /* 0x018 */ u8 pad018[0x128];
    /* 0x140 */ BossAI ai;
    /* 0x208 */ u8 pad208[0x2C];
    /* 0x234 */ void *onDraw;
} BossActor;
typedef struct {
    /* 0x00 */ u8 pad00[0xF0];
    /* 0xF0 */ void *onUpdate;
    /* 0xF4 */ u8 padF4[0x4];
    /* 0xF8 */ void *onEvent;
} BossEvents;
void TotemDefense_Init(BossActor *obj_, BossEvents *events_);
sptr CGameObjectInstance__PickRandomAnim(void *, sptr, sptr);
void COnScreen__SetBoss(void *, void *, char *);
/* the ROM passes the CAI in a3 too (the definition takes three) */
#define COnScreen__SetBoss_4 ((void (*)(void *, void *, char *, void *)) COnScreen__SetBoss)

void COnScreen__SetBoss2(void *, void *, char *);
void TotemDefense_Update(void *obj, CAI *ai);
s32 TotemDefense_GetState(void);
void TotemDefense_AddKill(void);
#include "structs/model.h"
void TotemDefense_DrawTotem(CGameObjectInstance *actor, s32 arg1, CDrawArgs *params);
void TotemDefense_InitFight(void *arg0, void *arg1);
typedef struct {
    /* 0x00 */ u8 pad0[0x46];
    /* 0x46 */ s16 unk46;
} Ref00408BFC;
typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ Ref00408BFC *ref;
    /* 0x08 */ u8 pad8[0x10 - 0x8];
    /* 0x10 */ s32 unk10;
} Unk00408BFC;
s32 TotemDefense_KillsLeft(Unk00408BFC *arg0);
#include "structs/twin_totemdefense.h"
void TotemDefense_RetargetEnemies(TargetInfo *info, s32 force);
void TotemDefense_TryHitEnemy(void);
void TotemDefense_StopEnemies(void);
void TotemDefense_StopGenerators(void);
s32 TotemDefense_AnyEnemyAlive(void);
void TotemDefense_TryHitEnemyAfterDelay(HurtTimer *h);
void TotemDefense_FindLiveEnemy(void);
void TotemDefense_OnEvent(void);
/* Boss fight state (0x8012FB50 on). */
#ifdef TARGET_PORT
#include "structs/boss.h"
#include "globals/boss.h"
#else
/* one-element array views of gTotemDefense and its field symbols (globals/boss.h): through them the ROM's
 * code; asm labels, so the header's names keep their types */
extern BossActor *gTotemDefense_obj_v[] __asm__("gTotemDefense");
extern void *gTotemDefense_def_v[] __asm__("gTotemDefense_def");
extern s32 gTotemDefense_state_v[] __asm__("gTotemDefense_state");
extern f32 gTotemDefense_timer_v[] __asm__("gTotemDefense_timer");
extern s32 gTotemDefense_kills_v[] __asm__("gTotemDefense_kills");
extern s32 gTotemDefense_warned_v[] __asm__("gTotemDefense_warned");
#endif
#ifndef TARGET_PORT
static __inline__ void startBossAnim(BossActor *obj) {
    BossActor *boss = gTotemDefense_obj_v[0];
    BossAI *ai = &boss->ai;

    gTotemDefense_warned_v[0] = 0;
    ai->animIndex = CGameObjectInstance__PickRandomAnim(boss, 0x5334, -1);
    ai->animType = 0x5334;
    ai->flags &= ~1;
    ai->animRepeats = 0;
    COnScreen__SetBoss_4(&gEngineApp_m_OnScreen, obj, "Enemies", ai);
}

#endif
#ifdef TARGET_PORT
/* The port (round 9 x1): Destructible_Setup's totem (type 0x64D) by member: the AI callbacks
 * (N64 0xF0/0xF8 of the CAI: m_pfStep, m_pfHurt), the definition's body (intelligence + 0x14 =
 * m_Destructible), the draw callback (N64 0x234 = m_AI.m_pfDraw), the totem's animation on its
 * CAI and gTotemDefense by member. Through the N64 views the host wrote the callbacks and the
 * animation into other CAI fields: the Harbor Totem's defense never ran its own update. */
DEFERRED void TotemDefense_Init(BossActor *obj_, BossEvents *events_) {
    CGameObjectInstance *obj = (CGameObjectInstance *) obj_;
    CAI *events = (CAI *) events_;
    CAI *ai;

    events->m_pfStep = (void *) TotemDefense_Update;
    events->m_pfHurt = (void *) TotemDefense_OnEvent;
    gTotemDefense.obj = obj;
    gTotemDefense.def = &obj->header.m_pIntelligence->m_Destructible;
    gTotemDefense.timer = (f32) (obj->header.m_pIntelligence->m_Destructible.m_TotemMissionDuration * 15);
    gTotemDefense.state = 0;
    gTotemDefense.kills = 0;
    obj->m_AI.m_pfDraw = (void *) TotemDefense_DrawTotem;
    gTotemDefense.warned = 0;
    ai = &gTotemDefense.obj->m_AI;
    ai->m_AnimIndex = CGameObjectInstance__PickRandomAnim((BossActor *) gTotemDefense.obj, 0x5334, -1);
    ai->m_AnimType = 0x5334;
    ai->m_dwFlags &= ~1;
    ai->m_AnimRepeatedCount = 0;
    COnScreen__SetBoss(&gEngineApp_m_OnScreen, obj_, "Enemies");
    COnScreen__SetBoss2(&gEngineApp_m_OnScreen, obj_, "Time");
}
#else
DEFERRED void TotemDefense_Init(BossActor *obj, BossEvents *events) {
    events->onUpdate = TotemDefense_Update;
    events->onEvent = TotemDefense_OnEvent;
    gTotemDefense_obj_v[0] = obj;
    gTotemDefense_def_v[0] = (u8 *) obj->def + 0x14;
    gTotemDefense_timer_v[0] = obj->def->m_Destructible.m_TotemMissionDuration * 15;
    gTotemDefense_state_v[0] = 0;
    gTotemDefense_kills_v[0] = 0;
    obj->onDraw = TotemDefense_DrawTotem;
    startBossAnim(obj);
    COnScreen__SetBoss2(&gEngineApp_m_OnScreen, obj, "Time");
}
#endif
#include "m2c_macros.h"
#include "globals/boss.h"

DEFERRED s32 TotemDefense_GetState(void) {
    return gTotemDefense_state;
}

extern s32 gTotemDefense_kills_v004089C8[] __asm__("gTotemDefense_kills"); /* local type: gTotemDefense.kills (s32, globals/boss.h); the array keeps the ROM's code */

DEFERRED void TotemDefense_AddKill(void) {
    gTotemDefense_kills_v004089C8[0] += 1;
}

#include "globals/scene.h"
#include "protos/cache.h"
/* As CGameObjectInstance__DrawModelLitVertices (engine), for a model drawn from its cached "lit vertices"
 * resource: the region sets come from CGameObjectInstance__GetDeathModels and the colour from the
 * totem defense state (gTotemDefense.warned). */

#include "structs/boss.h"

void CGameObjectInstance__GetDeathModels(CGameObjectInstance *, s32, s32 *, s32 *);
void CGameObjectInstance__DrawShadow(CGameObjectInstance *, sptr, CDrawArgs *);
void Render_QueueModelInstance(sptr, sptr, s32, sptr, char *, s32, s32);
void Model_PulseVertexColors(char *, char *);
void CGameObjectInstance__ConstructLitVertices();

DEFERRED void TotemDefense_DrawTotem(CGameObjectInstance *actor, s32 arg1, CDrawArgs *params) {
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
#ifdef TARGET_PORT
#else
#endif
#ifdef TARGET_PORT
#include "structs/game_object.h"
#include "structs/boss.h"

#define T2_TEXT(s) ((void *) (s))

/* The port: the same on TotemDefenseState / CGameObjectInstance (the N64 code reads both at raw
 * offsets: the definition 0x14, the AI's m_pfDraw 0x234, m_AnimType/m_AnimIndex/
 * m_AnimRepeatedCount 0x204/0x206/0x207, m_dwFlags 0x140; written after module 2's typed twin
 * TotemDefense_InitFight_2, TotemDefense_InitFight_2). state->def is the definition's body pointer-wide. */
DEFERRED void TotemDefense_InitFight(void *arg0, void *arg1) {
    TotemDefenseState *state = (TotemDefenseState *) arg0;
    CGameObjectInstance *obj = (CGameObjectInstance *) arg1;
    CGameObjectInstance *totem;
    CAI *ai;

    state->obj = obj;
    state->def = &obj->header.m_pIntelligence->m_Destructible;
    state->state = 0;
    state->kills = 0;
    state->timer = (f32) (obj->header.m_pIntelligence->m_Destructible.m_TotemMissionDuration * 0xF);
    obj->m_AI.m_pfDraw = (void *) TotemDefense_DrawTotem;
    totem = state->obj;
    state->warned = 0;
    ai = &totem->m_AI;
    ai->m_AnimIndex = CGameObjectInstance__PickRandomAnim(totem, 0x5334, -1);
    ai->m_AnimType = 0x5334;
    totem->m_AI.m_dwFlags &= ~1;
    ai->m_AnimRepeatedCount = 0;
    COnScreen__SetBoss(&gEngineApp_m_OnScreen, arg1, T2_TEXT(sTotemDefenseText_Enemies));
    COnScreen__SetBoss2((s32 *) &gEngineApp_m_OnScreen, arg1, &sTotemDefenseText_Time);
}
#else
DEFERRED void TotemDefense_InitFight(void *arg0, void *arg1)
{
  s32 new_var;
  void *temp_a3;
  void *temp_s0;
  *((void **) (((s8 *) arg0) + 0)) = arg1;
  *((s32 *) (((s8 *) arg0) + 4)) = (sptr) ((*((void **) (((s8 *) arg1) + 0x14))) + 0x14);
  new_var = *((s32 *) (((s8 *) (*((void **) (((s8 *) arg1) + 0x14)))) + 0x54));
  *((s32 *) (((s8 *) arg0) + 8)) = 0;
  *((s32 *) (((s8 *) arg0) + 0x10)) = 0;
  *((f32 *) (((s8 *) arg0) + 0xC)) = (f32) (new_var * 0xF);
  *((s32 (**)(s32, s32, s32)) (((s8 *) arg1) + 0x234)) = TotemDefense_DrawTotem;
  temp_s0 = *((void **) (((s8 *) arg0) + 0));
  *((s32 *) (((s8 *) arg0) + 0x14)) = 0;
  temp_a3 = temp_s0 + 0x140;
  *((s8 *) (((s8 *) temp_a3) + 0xC6)) = CGameObjectInstance__PickRandomAnim(temp_s0, 0x5334, -1);
  *((s16 *) (((s8 *) temp_a3) + 0xC4)) = 0x5334;
  *((s32 *) (((s8 *) temp_s0) + 0x140)) = (s32) ((*((s32 *) (((s8 *) temp_s0) + 0x140))) & (~1));
  *((s8 *) (((s8 *) temp_a3) + 0xC7)) = 0;
  COnScreen__SetBoss(&gEngineApp_m_OnScreen, arg1, sTotemDefenseText_Enemies);
  COnScreen__SetBoss2(&gEngineApp_m_OnScreen, arg1, &sTotemDefenseText_Time);
}
#endif

DEFERRED s32 TotemDefense_KillsLeft(Unk00408BFC *arg0) {
    return arg0->ref->unk46 - arg0->unk10;
}

#include "globals/game.h"

/* Point the player actors at a target (mode 0), or back at the player. */
DEFERRED void TotemDefense_RetargetEnemies(TargetInfo *info, s32 force) {
    CListedInstance *actor;
    void *player;
    s32 count;

    if (info->mode == 1) {
        return;
    }
    actor = gScene.m_InstancePool.m_ActiveList.m_pHead;
    player = gEngineApp_m_Game.m_PlayerList.m_ActiveList.m_pHead;
    count = 0;
    for (; actor != NULL; actor = actor->m_pNext) {
        if (actor->m_Instance.header.m_pIntelligence->m_Type != 1 || actor->m_Instance.m_AI.m_pTargetOverride != 0) {
            continue;
        }
        if (info->mode == 0) {
            if (count < info->def->limit || force) {
                count++;
                actor->m_Instance.m_AI.m_pPathTarget = player;
                actor->m_Instance.m_AI.m_pSightTarget = player;
                actor->m_Instance.m_AI.m_dwFlags |= 0x20000000;
                actor->m_Instance.m_AI.m_dwFlags |= 0x40000000;
            } else {
                actor->m_Instance.m_AI.m_pSightTarget = info->target;
                actor->m_Instance.m_AI.m_pPathTarget = info->target;
                actor->m_Instance.m_AI.m_dwFlags |= 0x20000000;
                actor->m_Instance.m_AI.m_dwFlags |= 0x40000000;
            }
        } else if (actor->m_Instance.m_AI.m_pSightTarget == info->target) {
            actor->m_Instance.m_AI.m_pPathTarget = player;
            actor->m_Instance.m_AI.m_pSightTarget = player;
            actor->m_Instance.m_AI.m_dwFlags |= 0x20000000;
            actor->m_Instance.m_AI.m_dwFlags |= 0x40000000;
        }
    }
}

#include "structs/collision.h"

extern f32 Random_Float(f32 scale);                                      /* random 0..scale */
void Actor_SpawnParticle(void *, s32, CVector3, s32);
void DamageEvent_Set(DamageEvent *, void *, s32, f32, s32, void *);
void EnemyAI_OnDamage(void *, void *, DamageEvent *);

/* One time in four, hurt the first live player actor. */
DEFERRED void TotemDefense_TryHitEnemy(void) {
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

#ifdef TARGET_PORT
#include "structs/player.h"
#include "structs/game.h"
#include "globals/game.h"
#else
extern u8 *gEngineApp_m_Game_m_PlayerList_m_ActiveList_m_pHead_v00408DCC __asm__("gEngineApp_m_Game_m_PlayerList_m_ActiveList_m_pHead"); /* local type: globals/game.h declares it; this view (its type here) is what keeps the ROM's code */          /* current player */
#endif

DEFERRED void TotemDefense_StopEnemies(void) {
    CListedInstance *actor;

#ifdef TARGET_PORT
    gEngineApp_m_Game.m_PlayerList.m_ActiveList.m_pHead->m_FleeTime = 450.0f; /* N64 0xAAC */
#else
    *(f32 *) (gEngineApp_m_Game_m_PlayerList_m_ActiveList_m_pHead_v00408DCC + 0xAAC) = 450.0f;
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

DEFERRED void TotemDefense_StopGenerators(void) {
    CScene__UnmarkGeneratorsSaved(&gScene);
}

/* Is any actor whose definition kind is 1 still alive? */
DEFERRED s32 TotemDefense_AnyEnemyAlive(void) {
    CListedInstance *actor;

    for (actor = gScene.m_InstancePool.m_ActiveList.m_pHead; actor != NULL; actor = actor->m_pNext) {
        if (actor->m_Instance.header.m_pIntelligence->m_Type == 1 && actor->m_Instance.m_AI.m_Health != 0) {
            return 1;
        }
    }
    return 0;
}

#include "globals/engine.h"

/* After a delay, one time in four, hurt the first live player actor. */
DEFERRED void TotemDefense_TryHitEnemyAfterDelay(HurtTimer *h) {
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
DEFERRED void TotemDefense_FindLiveEnemy(void) {
    CListedInstance *actor;

    for (actor = gScene.m_InstancePool.m_ActiveList.m_pHead; actor != NULL; actor = actor->m_pNext) {
        if (actor->m_Instance.header.m_pIntelligence->m_Type == 1 && actor->m_Instance.m_AI.m_Health != 0) {
            break;
        }
    }
}

DEFERRED void TotemDefense_OnEvent(void) {

}

#include "globals/strings.h"
#include "structs/game.h"
#include "structs/player.h"
#include "protos/player.h"

void CCameraPool__AddText(void *hud, void *camera, u8 *msg);
void Sound__PlaySpeech(s32 sound);
void CPlayerPool__SetInvincibility(CPlayerPool *list, f32 value);
void COnScreen__ClearBoss(void *);
void COnScreen__ClearBoss2(void *);

static __inline__ void setAnim(CListedInstance *actor, s32 id) {
    CAI *ai;
    s8 anim = CGameObjectInstance__PickRandomAnim(actor, id, -1);

    ai = &actor->m_Instance.m_AI;
    ai->m_AnimIndex = anim;
    ai->m_AnimType = id;
    actor->m_Instance.m_AI.m_dwFlags &= ~1;
    ai->m_AnimRepeatedCount = 0;
}

/* The boss is beaten: message, sound, its death animation, the helpers stop. */
void TotemDefense_Complete(TotemDefenseState *fight) {
    CGame *game = &gEngineApp_m_Game;
    CPlayer *player;
    CListedInstance *a;
    GameSettings *flags;

    fight->state = 1;
    player = CPlayerPool__GetFirstActive(&game->m_PlayerList);
    CCameraPool__AddText(game->m_CameraPool, player->m_pCamera, sTotemDefenseText_MissionComplete);
    Sound__PlaySpeech(0x168);
    CPlayerPool__SetInvincibility(&game->m_PlayerList, 900.0f);
    setAnim((CListedInstance *) fight->obj, 0x5337);
    CScene__UnmarkGeneratorsSaved(&gScene);
    gEngineApp_m_Game.m_PlayerList.m_ActiveList.m_pHead->m_FleeTime = 450.0f;
    for (a = gScene.m_InstancePool.m_ActiveList.m_pHead; a != NULL; a = a->m_pNext) {
        if (a->m_Instance.header.m_pIntelligence->m_Type == 1 && a->m_Instance.m_AI.m_Health != 0) {
            a->m_Instance.m_AI.m_pPathTarget = NULL;
            a->m_Instance.m_AI.m_pSightTarget = NULL;
            a->m_Instance.m_AI.m_dwFlags |= 0x20000000;
            a->m_Instance.m_AI.m_dwFlags |= 0x40000000;
        }
    }
    fight->timer = 60.0f;
    COnScreen__ClearBoss(&gEngineApp_m_OnScreen);
    COnScreen__ClearBoss2(&gEngineApp_m_OnScreen);
    switch (gScene_m_LevelID) {
    case 0x1BBC:
        player->m_Data.m_bLevel1TotemFlags = 1;
        flags = &gEngineApp_m_Settings;
        flags->unlockedCheats |= 0x8000;
        break;
    case 0x1C20:
        player->m_Data.m_bLevel2TotemFlags = 1;
        flags = &gEngineApp_m_Settings;
        flags->unlockedCheats |= 0x200;
        break;
    case 0x1C84:
        player->m_Data.m_bLevel3TotemFlags = 1;
        flags = &gEngineApp_m_Settings;
        flags->unlockedCheats |= 0x400;
        break;
    case 0x1CE8:
        player->m_Data.m_bLevel4TotemFlags = 1;
        flags = &gEngineApp_m_Settings;
        flags->unlockedCheats |= 0x80;
        break;
    case 0x1D4C:
        player->m_Data.m_bLevel5TotemFlags = 1;
        flags = &gEngineApp_m_Settings;
        flags->unlockedCheats |= 0x40;
        break;
    }
}

/* The fight starts: message, the boss's intro animation, and the helpers
 * (those not dead and, unless the fight is over, targeting the boss) turn on
 * the current player. */
void TotemDefense_Fail(TotemDefenseState *fight) {
    CGame *game = &gEngineApp_m_Game;
    CPlayer *player;
    CListedInstance *a;
    CPlayer *current;

    player = CPlayerPool__GetFirstActive(&game->m_PlayerList);
    CCameraPool__AddText(game->m_CameraPool, player->m_pCamera, sTotemDefenseText_MissionFailed);
    player->m_InvincibilityTime = 900.0f;
    fight->state = 2;
    fight->warned = 2;
    setAnim((CListedInstance *) fight->obj, 0x5336);
    COnScreen__ClearBoss(&gEngineApp_m_OnScreen);
    COnScreen__ClearBoss2(&gEngineApp_m_OnScreen);
    CScene__UnmarkGeneratorsSaved(&gScene);
    if (fight->state != 1) {
        current = gEngineApp_m_Game_m_PlayerList_m_ActiveList_m_pHead;
        for (a = gScene.m_InstancePool.m_ActiveList.m_pHead; a != NULL; a = a->m_pNext) {
            if (a->m_Instance.header.m_pIntelligence->m_Type == 1 && a->m_Instance.m_AI.m_pTargetOverride == 0 && (fight->state == 0 || a->m_Instance.m_AI.m_pSightTarget == fight->obj)) {
                a->m_Instance.m_AI.m_pPathTarget = current;
                a->m_Instance.m_AI.m_pSightTarget = current;
                a->m_Instance.m_AI.m_dwFlags |= 0x20000000;
                a->m_Instance.m_AI.m_dwFlags |= 0x40000000;
            }
        }
    }
}

#ifdef TARGET_PORT
#include "structs/player.h"
#endif

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ u8 *def;
} BossObj;
typedef struct {
    /* 0x00 */ u8 pad0[0x40];
    /* 0x40 */ s32 time;
    /* 0x44 */ s16 maxHelpers;
    /* 0x46 */ s16 kills;
} BossDef;
void CCameraPool__AddTimedText(void *, sptr, void *, f32);
void Sound__PlaySpeech(s32);
void COnScreen__SetBossPercent(void *, s32);
void COnScreen__SetBossPercent2(void *, s32);

static __inline__ s32 toInt(f32 f) {
    s32 value;

    value = 0;
    if (!(f < 0.0f)) {
        value = f;
    }
    return value;
}

void TotemDefense_UpdateFight(TotemDefenseState *fight) {
    f32 timer;
    u8 *bar;
    void *player;
    s32 left;
    s32 left2;
    s32 n;
    CListedInstance *actor;
    u8 *obj;
    u8 *ai;

    if (gEngineApp_m_Game.m_Settings.mode != 13 || gScene.m_Mode != 4) {
        return;
    }
    /* the port (round 9 x1): the mission time and the player's camera by member (N64 0x54 of the
     * definition, 0x51C of CPlayer: as an s32 the camera pointer was cut) */
    if ((f32) (fight->obj->header.m_pIntelligence->m_Destructible.m_TotemMissionDuration * 15) <= fight->timer) {
        CCameraPool__AddTimedText(gEngineApp_m_Game.m_CameraPool,
                                  (sptr) ((CPlayer *) CPlayerPool__GetFirstActive(&gEngineApp_m_Game.m_PlayerList))->m_pCamera, sTotemDefenseText_DefendTheTotem, 4.0f);
        Sound__PlaySpeech(0x182);
    }
    bar = &gEngineApp_m_OnScreen;
    COnScreen__SetBossPercent(bar, toInt(fight->timer * (1.0f / 15) / (f32) ((BossDef *) fight->def)->time * 100.0f));
    COnScreen__SetBossPercent2(bar, toInt((f32) (((BossDef *) fight->def)->kills - fight->kills) / (f32) ((BossDef *) fight->def)->kills * 100.0f));
    if (fight->state == 0) {
        if (fight->timer < 450.0f && fight->warned != 1) {
#ifdef TARGET_PORT
            /* the totem's warning animation on its CAI by member (N64 0x140 + 0xC4/0xC6/0xC7) */
            obj = (u8 *) fight->obj;
            ai = obj;
            fight->obj->m_AI.m_AnimIndex = CGameObjectInstance__PickRandomAnim(obj, 0x5335, -1);
            fight->obj->m_AI.m_AnimType = 0x5335;
            fight->obj->m_AI.m_dwFlags &= ~1;
            fight->obj->m_AI.m_AnimRepeatedCount = 0;
#else
            obj = (u8 *) fight->obj;
            ai = obj + 0x140;
            ai[0xC6] = CGameObjectInstance__PickRandomAnim(obj, 0x5335, -1);
            *(s16 *) (ai + 0xC4) = 0x5335;
            *(s32 *) (obj + 0x140) &= ~1;
            ai[0xC7] = 0;
#endif
            fight->warned = 1;
        }
        timer = fight->timer - gFrameIncrement;
        left = ((BossDef *) fight->def)->kills - fight->kills;
        left2 = left;
        fight->timer = timer;
        if (timer <= 0.0f) {
            if (left > 0) {
                TotemDefense_Fail(fight);
            } else {
                TotemDefense_Complete(fight);
            }
        } else if (left2 <= 0) {
            TotemDefense_Complete(fight);
        } else {
            fight->state = 0;
        }
    }
    if (fight->state != 1) {
        actor = gScene.m_InstancePool.m_ActiveList.m_pHead;
        player = gEngineApp_m_Game.m_PlayerList.m_ActiveList.m_pHead;
        n = 0;
        for (; actor != NULL; actor = actor->m_pNext) {
            if (actor->m_Instance.header.m_pIntelligence->m_Type == 1 && actor->m_Instance.m_AI.m_pTargetOverride == 0) {
                if (fight->state == 0) {
                    if (n < ((BossDef *) fight->def)->maxHelpers) {
                        /* the first helpers keep chasing the player */
                        n++;
                        actor->m_Instance.m_AI.m_pPathTarget = player;
                        actor->m_Instance.m_AI.m_pSightTarget = player;
                        actor->m_Instance.m_AI.m_dwFlags |= 0x20000000;
                        actor->m_Instance.m_AI.m_dwFlags |= 0x40000000;
                    } else {
                        actor->m_Instance.m_AI.m_pSightTarget = fight->obj;
                        actor->m_Instance.m_AI.m_pPathTarget = fight->obj;
                        actor->m_Instance.m_AI.m_dwFlags |= 0x20000000;
                        actor->m_Instance.m_AI.m_dwFlags |= 0x40000000;
                    }
                } else if (actor->m_Instance.m_AI.m_pSightTarget == fight->obj) {
                    actor->m_Instance.m_AI.m_pPathTarget = player;
                    actor->m_Instance.m_AI.m_pSightTarget = player;
                    actor->m_Instance.m_AI.m_dwFlags |= 0x20000000;
                    actor->m_Instance.m_AI.m_dwFlags |= 0x40000000;
                }
            }
        }
    }
}

#include "structs/vector.h"

extern f32 Random_Float(f32 range);
extern void CPlayer__Warp(CPlayer *player, s32 track, s32 a, s32 b);

static __inline__ s32 helperAlive(void) {
    CListedInstance *actor;

    for (actor = gScene.m_InstancePool.m_ActiveList.m_pHead; actor != NULL; actor = actor->m_pNext) {
        if (actor->m_Instance.header.m_pIntelligence->m_Type == 1 && actor->m_Instance.m_AI.m_Health != 0) {
            return 1;
        }
    }
    return 0;
}

static __inline__ void stunHelper(void) {
    /* a DamageEvent: 0x18 bytes on the N64, 0x28 on the host (two pointers); the byte buffer
     * overflowed into the caller's stack (macOS: __stack_chk_fail once the totem defence was
     * won and the helpers got stunned, round 10 y4) */
    DamageEvent anim[1];
    CListedInstance *actor;

    if (gTotemDefense.timer > 0.0f) {
        gTotemDefense.timer -= gFrameIncrement;
    } else if (!(Random_Float(100.0f) > 25.0f)) {
        for (actor = gScene.m_InstancePool.m_ActiveList.m_pHead; actor != NULL; actor = actor->m_pNext) {
            if (actor->m_Instance.header.m_pIntelligence->m_Type == 1 && actor->m_Instance.m_AI.m_Health != 0) {
                Actor_SpawnParticle(actor, 0x17C, actor->m_Instance.header.m_vPos, 0);
                DamageEvent_Set(anim, NULL, 0xFF00, 51.199997f, 0x100000, NULL);
                EnemyAI_OnDamage(actor, &actor->m_Instance.m_AI, anim);
                break;
            }
        }
    }
}

void TotemDefense_Update(void *obj, CAI *ai) {
    TotemDefenseState *fight = &gTotemDefense;
    CPlayer *player;
    s32 track;

    if (ai->m_pfDraw != TotemDefense_DrawTotem) {
        ai->m_pfDraw = TotemDefense_DrawTotem;
    }
    switch (fight->state) {
    case 0:
        TotemDefense_UpdateFight(fight);
        break;
    case 1:
        stunHelper();
        break;
    case 2:
        helperAlive();
        break;
    }
    if (fight->state == 0) {
        ai->m_ModeTime = 0.0f;
        return;
    }
    if (fight->state == 1) {
        if (helperAlive()) {
            ai->m_ModeTime = 0.0f;
            return;
        }
        if (ai->m_ModeTime < 90.0f) {
            return;
        }
    } else if (ai->m_ModeTime < 75.0f) {
        return;
    }
    player = CPlayerPool__GetFirstActive(&gEngineApp_m_Game.m_PlayerList);
    if (player == NULL) {
        return;
    }
    switch (gScene_m_LevelID) {
    case 0x1BBC:
        track = 0x7CF;
        break;
    case 0x1C20:
        track = 0xBB7;
        break;
    case 0x1C84:
        track = 0xF9F;
        break;
    case 0x1CE8:
        track = 0x1CE9;
        if (player->m_Data.m_bBlindBossComplete) {
            track = 0x1387;
        }
        break;
    case 0x1D4C:
        track = 0x1D4D;
        if (player->m_Data.m_bQueenBossComplete) {
            track = 0x176F;
        }
        break;
    default:
        track = gScene_m_LevelID;
        break;
    }
    player->m_InvincibilityTime = 0.0f;
    CPlayer__Warp(player, track, 2, 0);
}
