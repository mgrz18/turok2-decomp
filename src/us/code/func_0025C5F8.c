#include "common.h"
#include "structs/vector.h"
#include "structs/intelligence.h"
#include "structs/game_object.h"
#include "structs/ai_behavior.h"
#include "structs/game.h"
#include "globals/game.h"
#include "globals/engine.h"
#include "globals/ai.h"
#include "protos/ai.h"

/* Typed on CGameObjectInstance / CAI (m2c's Object/AI views had the N64 offsets:
 * the behaviour index at 0x133 was m_Enemy.m_BehaviorIndex, the current mode row at 0x30). */

/* The attached looping sound (only its id). */
typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 id;
} Sound;

void *CPlayerPool__FindNearest(CPlayerPool *players, CVector3 pos);
f32 Random_Blend(f32 lo, f32 hi);
s32 EnemyAI_TickTimers(CGameObjectInstance *obj, CAI *ai);
void AI_TurnHeadToTarget(CGameObjectInstance *obj, CAI *ai, f32);
s32 Sound__Play(s16 sound, CVector3 pos, sptr, sptr);
void AI_StartSound(CGameObjectInstance *obj, CAI *ai, s32 sound);
void AI_StopSound(CGameObjectInstance *obj, CAI *ai);

/* AI update: age, scale the frame time, run the current mode handler (its
 * enter callback when the animation changes), count down the timers, start
 * idle animations and the swim/land switches, run the behaviour, and keep
 * the burning effect and its sound going while burnTime lasts. */
void EnemyAI_Update(CGameObjectInstance *obj, CAI *ai) {
    CGameObjectInstance *target;
    CEnemyBehaviorInfo *handler;
    s32 index;
    s32 mode;
    CAIModeInfo *anim;
    s32 state;
    s32 swimming;

    if (!(obj->m_dwFlags & 0x2000)) {
        obj->m_dwFlags &= ~0x100;
        return;
    }
    target = ai->m_pSightTarget;
    if (target != NULL && target->header.m_pIntelligence->m_Type == 1 && (target->m_AI.m_Mode == 0x3C || !(target->m_dwFlags & 0x100))) {
        ai->m_dwFlags &= ~0x20000000;
        ai->m_dwFlags &= ~0x40000000;
    }
    if (ai->m_LastHurtSFXTimer < 100) {
        ai->m_LastHurtSFXTimer++;
    }
    gFrameIncrement *= gEnemyTimeScale;
    CPlayerPool__FindNearest(&gEngineApp_m_Game.m_PlayerList, obj->header.m_vPos);
    mode = ai->m_Enemy.m_Behavior[ai->m_Enemy.m_BehaviorIndex];
    index = ai->m_Enemy.m_BehaviorIndex;
    handler = gEnemyBehaviorInfo[mode];
    swimming = mode == 2;
    if (handler != NULL) {
        switch (index) {
            case 0:
                anim = handler->m_pGroundModeTable;
                break;
            case 1:
                anim = handler->m_pAirModeTable;
                break;
            case 2:
                anim = handler->m_pUnderwaterModeTable;
                break;
            default:
                anim = NULL;
                break;
        }
        if (ai->m_pModeTable != anim) {
            ai->m_pModeTable = anim;
            if (handler->m_pfSetup != NULL) {
                handler->m_pfSetup(obj, ai);
            }
        }
        if (handler->m_pfUpdate != NULL) {
            handler->m_pfUpdate(obj, ai);
        }
    }
    if (!(ai->m_Mode == 0x11 || ai->m_Mode == 0x12)) {
        ai->m_Enemy.m_AttackDelay -= gFrameIncrement;
        if (ai->m_Enemy.m_AttackDelay < 0.0f) {
            ai->m_Enemy.m_AttackDelay = 0.0f;
        }
    }
    index = ai->m_Enemy.m_BehaviorIndex;
    if (obj->header.m_pIntelligence->m_dwTypeFlags & 0x40) {
        ai->m_VarF32[6] -= gFrameIncrement;
        if (ai->m_VarF32[6] < 0.0f) {
            ai->m_VarF32[6] = 0.0f;
        }
        if (ai->m_Health && ai->m_VarF32[6] <= 0.0f) {
            state = ai->m_Mode;
            if (state == 4 || state == 0xE || state == 0xF || state == 0x10 || state == 0x11 || state == 0x12
                || state == 0xA) {
                ai->m_VarF32[6] = Random_Blend(75.0f, 150.0f);
                CAI__SetMode(obj, ai, 0x17);
            }
        }
    }
    EnemyAI_TickTimers(obj, ai);
    if (ai->m_Health) {
        if (index != 2 && (ai->m_dwFlags & 0x80) && ai->m_Mode != 0x27 && ai->m_Mode != 0x29 && ai->m_Enemy.m_Behavior[2] != 0) {
            if (index == 0) {
                CAI__SetMode(obj, ai, 0x27);
            } else if (index == 1) {
                CAI__SetMode(obj, ai, 0x29);
            }
        }
        if (ai->m_Health && index == 2 && !(ai->m_dwFlags & 0x80) && ai->m_Mode != 0x2A && ai->m_Mode != 0x2B
            && ai->m_Enemy.m_Behavior[0] != 0) {
            CAI__SetMode(obj, ai, 0x2A);
        }
    }
    if (ai->m_pModeCurrent != NULL && ai->m_pModeCurrent->m_pfCallback != NULL) {
        ai->m_pModeCurrent->m_pfCallback(obj, ai);
    }
    if (ai->m_Health && swimming) {
        ai->m_vDesiredPos.x = ai->m_vInitialPos.x;
        ai->m_vDesiredPos.z = ai->m_vInitialPos.z;
        obj->m_dwFlags &= ~0x20000;
    }
    AI_TurnHeadToTarget(obj, ai, 0.0f);
    if (ai->m_OnFireTimer > 0.0f) {
        ai->m_OnFireTimer -= gFrameIncrement;
        obj->m_dwHotPointFlags |= 2;
        Sound__Play(0x3D1, obj->header.m_vPos, 0, -1);
        if (ai->m_Health) {
            AI_StartSound(obj, ai, 0x3D2);
        } else if (ai->m_pAttachedLoopingSound != NULL && ((Sound *) ai->m_pAttachedLoopingSound)->id != 0x3D2) {
            AI_StopSound(obj, ai);
        }
    } else {
        obj->m_dwHotPointFlags &= ~2;
    }
    if (ai->m_Mode == 6) {
        obj->m_dwFlags |= 0x800000;
    } else {
        obj->m_dwFlags &= ~0x800000;
    }
}
