#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "structs/game.h"
#include "structs/player.h"
#include "globals/game.h"
#include "globals/scene.h"
#include "globals/onscreen.h"
#include "protos/scene.h"
#include "protos/player.h"

typedef struct {
    /* 0x00 */ u8 pad0[0xC4];
    /* 0xC4 */ s16 animId;
    /* 0xC6 */ s8 anim;
    /* 0xC7 */ s8 unkC7;
} BossAI;


extern u8 D_800B6660[];

void CCameraPool__AddText(void *hud, void *camera, u8 *msg);
void Sound__PlaySpeech(s32 sound);
void CPlayerPool__SetInvincibility(CPlayerPool *list, f32 value);
s8 CGameObjectInstance__PickRandomAnim(CListedInstance *actor, sptr id, sptr);
void COnScreen__ClearBoss(void *);
void COnScreen__ClearBoss2(void *);

static __inline__ void setAnim(CListedInstance *actor, s32 id) {
    BossAI *ai;
    s8 anim = CGameObjectInstance__PickRandomAnim(actor, id, -1);

    ai = (BossAI *) &actor->m_Instance.m_AI.m_dwFlags;
    ai->anim = anim;
    ai->animId = id;
    actor->m_Instance.m_AI.m_dwFlags &= ~1;
    ai->unkC7 = 0;
}

/* The boss is beaten: message, sound, its death animation, the helpers stop. */
void TotemDefense_Complete(TotemDefenseState *fight) {
    CGame *game = &gEngineApp_m_Game;
    CPlayer *player;
    CListedInstance *a;
    GameSettings *flags;

    fight->state = 1;
    player = CPlayerPool__GetFirstActive(&game->m_PlayerList);
    CCameraPool__AddText(game->m_CameraPool, player->m_pCamera, D_800B6660);
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
