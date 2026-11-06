#include "common.h"
#include "structs/game_object.h"
#include "structs/vector.h"
#include "structs/game.h"
#include "structs/player.h"
#include "globals/game.h"

/* Boss fight AI tick: runs the fight phase (timer and helpers, then the
 * random stun of a live helper), and once the boss has waited long enough
 * outside the fight starts the level's boss music. */

typedef struct {
    /* 0x00 */ void *obj;
    /* 0x04 */ void *def;
    /* 0x08 */ s32 state;
    /* 0x0C */ f32 timer;
} BossFight;

extern BossFight gTotemDefense;
extern CListedInstance *gScene_m_InstancePool_m_ActiveList;
extern f32 gFrameIncrement;
extern s32 gScene_m_LevelID;

extern void TotemDefense_DrawTotem();
extern void TotemDefense_UpdateFight(BossFight *fight);
extern f32 Random_Float(f32 range);
extern void Actor_SpawnParticle(CListedInstance *actor, s32 sound, CVector3 pos, s32 arg);
extern void DamageEvent_Set(void *out, s32 a, s32 b, f32 c, s32 d, s32 e);
extern void EnemyAI_OnDamage(CListedInstance *actor, void *ai, void *anim);
extern CPlayer *CPlayerPool__GetFirstActive(void *players);
extern void CPlayer__Warp(CPlayer *player, s32 track, s32 a, s32 b);

static __inline__ s32 helperAlive(void) {
    CListedInstance *actor;

    for (actor = gScene_m_InstancePool_m_ActiveList; actor != NULL; actor = actor->m_pNext) {
        if (actor->m_Instance.header.m_pIntelligence->m_Type == 1 && actor->m_Instance.m_AI.m_Health != 0) {
            return 1;
        }
    }
    return 0;
}

static __inline__ void stunHelper(void) {
    u8 anim[0x18];
    CListedInstance *actor;

    if (gTotemDefense.timer > 0.0f) {
        gTotemDefense.timer -= gFrameIncrement;
    } else if (!(Random_Float(100.0f) > 25.0f)) {
        for (actor = gScene_m_InstancePool_m_ActiveList; actor != NULL; actor = actor->m_pNext) {
            if (actor->m_Instance.header.m_pIntelligence->m_Type == 1 && actor->m_Instance.m_AI.m_Health != 0) {
                Actor_SpawnParticle(actor, 0x17C, actor->m_Instance.header.m_vPos, 0);
                DamageEvent_Set(anim, 0, 0xFF00, 51.199997f, 0x100000, 0);
                EnemyAI_OnDamage(actor, &actor->m_Instance.m_AI, anim);
                break;
            }
        }
    }
}

void TotemDefense_Update(void *obj, CAI *ai) {
    BossFight *fight = &gTotemDefense;
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
