#include "common.h"
#include "structs/game_object.h"
#include "structs/game.h"
#include "globals/game.h"
#include "globals/onscreen.h"
#include "globals/scene.h"
#include "globals/engine.h"

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

typedef struct {
    /* 0x00 */ BossObj *obj;
    /* 0x04 */ BossDef *def;
    /* 0x08 */ s32 state;
    /* 0x0C */ f32 timer;
    /* 0x10 */ s32 count;
    /* 0x14 */ s32 warned;
} BossFight;

extern u8 D_800B6684[];

void *CPlayerPool__GetFirstActive(void *);
void CCameraPool__AddTimedText(void *, s32, void *, f32);
void Sound__PlaySpeech(s32);
void COnScreen__SetBossPercent(void *, s32);
void COnScreen__SetBossPercent2(void *, s32);
s8 CGameObjectInstance__PickRandomAnim(void *, s32, s32);
void TotemDefense_Fail(BossFight *);
void TotemDefense_Complete(BossFight *);

static __inline__ s32 toInt(f32 f) {
    s32 value;

    value = 0;
    if (!(f < 0.0f)) {
        value = f;
    }
    return value;
}

void TotemDefense_UpdateFight(BossFight *fight) {
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
    if ((f32) (*(s32 *) (fight->obj->def + 0x54) * 15) <= fight->timer) {
        CCameraPool__AddTimedText(gEngineApp_m_Game.m_CameraPool, *(s32 *) ((u8 *) CPlayerPool__GetFirstActive(&gEngineApp_m_Game.m_PlayerList) + 0x51C), D_800B6684, 4.0f);
        Sound__PlaySpeech(0x182);
    }
    bar = &gEngineApp_m_OnScreen;
    COnScreen__SetBossPercent(bar, toInt(fight->timer * (1.0f / 15) / (f32) fight->def->time * 100.0f));
    COnScreen__SetBossPercent2(bar, toInt((f32) (fight->def->kills - fight->count) / (f32) fight->def->kills * 100.0f));
    if (fight->state == 0) {
        if (fight->timer < 450.0f && fight->warned != 1) {
            obj = (u8 *) fight->obj;
            ai = obj + 0x140;
            ai[0xC6] = CGameObjectInstance__PickRandomAnim(obj, 0x5335, -1);
            *(s16 *) (ai + 0xC4) = 0x5335;
            *(s32 *) (obj + 0x140) &= ~1;
            ai[0xC7] = 0;
            fight->warned = 1;
        }
        timer = fight->timer - gFrameIncrement;
        left = fight->def->kills - fight->count;
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
                    if (n < fight->def->maxHelpers) {
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
