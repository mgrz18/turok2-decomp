#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "structs/game.h"
#include "globals/game.h"
#include "globals/onscreen.h"
#include "globals/scene.h"
#include "globals/engine.h"
#include "protos/player.h"
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

extern u8 D_800B6684[];

void CCameraPool__AddTimedText(void *, sptr, void *, f32);
void Sound__PlaySpeech(s32);
void COnScreen__SetBossPercent(void *, s32);
void COnScreen__SetBossPercent2(void *, s32);
s8 CGameObjectInstance__PickRandomAnim(void *, sptr, sptr);
void TotemDefense_Fail(TotemDefenseState *);
void TotemDefense_Complete(TotemDefenseState *);

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
#ifdef TARGET_PORT
    /* the port (round 9 x1): the mission time and the player's camera by member (N64 0x54 of the
     * definition, 0x51C of CPlayer: as an s32 the camera pointer was cut) */
    if ((f32) (fight->obj->header.m_pIntelligence->m_Destructible.m_TotemMissionDuration * 15) <= fight->timer) {
        CCameraPool__AddTimedText(gEngineApp_m_Game.m_CameraPool,
                                  (sptr) ((CPlayer *) CPlayerPool__GetFirstActive(&gEngineApp_m_Game.m_PlayerList))->m_pCamera, D_800B6684, 4.0f);
        Sound__PlaySpeech(0x182);
    }
#else
    if ((f32) (*(s32 *) (((BossObj *) fight->obj)->def + 0x54) * 15) <= fight->timer) {
        CCameraPool__AddTimedText(gEngineApp_m_Game.m_CameraPool, *(s32 *) ((u8 *) CPlayerPool__GetFirstActive(&gEngineApp_m_Game.m_PlayerList) + 0x51C), D_800B6684, 4.0f);
        Sound__PlaySpeech(0x182);
    }
#endif
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
