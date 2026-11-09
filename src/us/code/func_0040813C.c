#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "structs/player.h"
#include "structs/game.h"
#include "globals/game.h"
#include "globals/onscreen.h"
#include "globals/scene.h"
#include "protos/scene.h"
#include "protos/player.h"

typedef struct {
    /* 0x00 */ u8 pad0[0xC4];
    /* 0xC4 */ s16 animId;
    /* 0xC6 */ s8 anim;
    /* 0xC7 */ s8 unkC7;
} BossAI;


extern u8 D_800B6674[];

void CCameraPool__AddText(void *hud, void *camera, u8 *msg);
void Sound__PlaySpeech(s32 sound);
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

/* The fight starts: message, the boss's intro animation, and the helpers
 * (those not dead and, unless the fight is over, targeting the boss) turn on
 * the current player. */
void TotemDefense_Fail(TotemDefenseState *fight) {
    CGame *game = &gEngineApp_m_Game;
    CPlayer *player;
    CListedInstance *a;
    CPlayer *current;

    player = CPlayerPool__GetFirstActive(&game->m_PlayerList);
    CCameraPool__AddText(game->m_CameraPool, player->m_pCamera, D_800B6674);
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
