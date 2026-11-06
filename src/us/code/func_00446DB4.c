/* Second-module copy of BlindOne_ActorHurt (src/us/code/BlindOne_ActorHurt.c), with its string/data addresses 0xE0 lower. */
#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "structs/collision.h"

/* Damage callback of the second boss' minions: takes the damage, plays the minion's hit effect at most every
 * 2.25 s, and at zero health goes to anim state 6. */


extern BlindOneState gBlindOne;
extern u8 sBlindOneHitScript_2[];
extern void ScriptAnimator_SetScript(void *dst, void *src);
extern void CAI__SetMode(void *obj, CAI *ai, s32 state);

void BlindOne_ActorHurt_2(void *obj, CAI *ai, DamageEvent *event) {
    BlindOneState *state = &gBlindOne;
    s32 health;

    if (ai->m_Health != 0) {
        health = ai->m_Health - event->damage;
        if (health < 0) {
            health = 0;
        }
        ai->m_Health = health;
        if (ai->m_BlindOne.m_HitTimer == 0.0f) {
            ai->m_BlindOne.m_HitTimer = 2.25f;
            ScriptAnimator_SetScript(&state->faders[ai->m_BlindOne.m_nFader], sBlindOneHitScript_2);
        }
        if (ai->m_Health == 0) {
            CAI__SetMode(obj, ai, 6);
        }
    }
}
