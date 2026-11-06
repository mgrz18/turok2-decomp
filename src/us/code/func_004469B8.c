/* Second-module copy of BlindOne_BossHurt (src/us/code/BlindOne_BossHurt.c), with its string/data addresses 0xE0 lower. */
#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "structs/collision.h"
#include "globals/boss.h"

/* Damage callback of the second boss: outside phase 9 it only flinches (anim state 6); in phase 9 the damage
 * is taken, a hit effect plays at most every 2.25 s, and at zero health it dies (anim state 9). */


extern BlindOneState gBlindOne;
extern void ScriptAnimator_SetScript(void *dst, void *src);
extern void CAI__SetMode(void *obj, CAI *ai, s32 state);
extern void CCache__SetFrameDelay(s32 arg0);

void BlindOne_BossHurt_2(void *obj, CAI *ai, DamageEvent *event) {
    BlindOneState *state;
    s32 health;

    if (ai->m_Health != 0 && ai->m_Mode != 1) {
        state = &gBlindOne;
        if (state->phase != 9) {
            ai->m_AnimRepeatedCount = 0;
            CAI__SetMode(obj, ai, 6);
        } else {
            health = ai->m_Health - event->damage;
            if (health < 0) {
                health = 0;
            }
            ai->m_Health = health;
            if (ai->m_BlindOne.m_HitTimer == 0.0f) {
                ai->m_BlindOne.m_HitTimer = 2.25f;
                ScriptAnimator_SetScript(&state->faders[0], sBlindOneHitScript_2);
            }
            if (ai->m_Health == 0) {
                CAI__SetMode(obj, ai, 9);
                CCache__SetFrameDelay(8);
            }
        }
    }
}
