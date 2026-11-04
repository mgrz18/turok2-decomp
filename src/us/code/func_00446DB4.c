/* Second-module copy of func_0040DDB4 (src/us/code/func_0040DDB4.c), with its string/data addresses 0xE0 lower. */
#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "structs/collision.h"

/* Damage callback of the second boss' minions: takes the damage, plays the minion's hit effect at most every
 * 2.25 s, and at zero health goes to anim state 6. */


extern BlindOneState D_80130200;
extern u8 D_00432384[];
extern void func_00236314(void *dst, void *src);
extern void func_00243414(void *obj, CAI *ai, s32 state);

void func_00446DB4(void *obj, CAI *ai, DamageEvent *event) {
    BlindOneState *state = &D_80130200;
    s32 health;

    if (ai->m_Health != 0) {
        health = ai->m_Health - event->damage;
        if (health < 0) {
            health = 0;
        }
        ai->m_Health = health;
        if (ai->m_BlindOne.m_HitTimer == 0.0f) {
            ai->m_BlindOne.m_HitTimer = 2.25f;
            func_00236314(&state->faders[ai->m_BlindOne.m_nFader], D_00432384);
        }
        if (ai->m_Health == 0) {
            func_00243414(obj, ai, 6);
        }
    }
}
