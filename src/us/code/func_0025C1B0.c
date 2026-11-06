#include "common.h"
#include "structs/game_object.h"
#include "globals/engine.h"

/* AI idle-sound callback: while the actor moves fast the timer stays reset;
 * otherwise it counts down and, once idle in state 4, plays the ambient
 * animation (0x26 or 0x28 depending on the mode) every 30-75 frames. */

typedef struct {
    /* 0x00 */ u8 pad0[0x60];
    /* 0x60 */ f32 speed;
} Motion;


extern void CAI__SetMode(CGameObjectInstance *actor, CAI *ai, s32 anim);
extern f32 Random_Blend(f32 lo, f32 hi);

void EnemyAI_IdleSoundCallback(CGameObjectInstance *actor, CAI *ai, Motion *motion) {
    s32 modeIndex = ai->m_Enemy.m_BehaviorIndex;
    f32 speed;

    if (!(ai->m_ModeFlags & 0x10000) && (actor->m_dwFlags & 0x400)) {
        return;
    }
    speed = motion->speed;
    if (speed < 0.0f ? -speed > 102.399994f : speed > 102.399994f) {
        ai->m_Enemy.m_IdleTimer = 30.0f;
        if (ai->m_Enemy.m_Behavior[1] && modeIndex == 0) {
            CAI__SetMode(actor, ai, 0x26);
        }
        return;
    }
    ai->m_Enemy.m_IdleTimer -= gFrameIncrement;
    if (ai->m_Enemy.m_IdleTimer < 0.0f) {
        ai->m_Enemy.m_IdleTimer = 0.0f;
    }
    if (ai->m_Health && ai->m_Mode == 4 && ai->m_Enemy.m_IdleTimer == 0.0f) {
        ai->m_Enemy.m_IdleTimer = Random_Blend(30.0f, 75.0f);
        if (modeIndex == 0) {
            if (ai->m_Enemy.m_Behavior[1]) {
                CAI__SetMode(actor, ai, 0x26);
            }
        } else if (modeIndex == 1) {
            if (ai->m_Enemy.m_Behavior[0]) {
                CAI__SetMode(actor, ai, 0x28);
            }
        }
    }
}
