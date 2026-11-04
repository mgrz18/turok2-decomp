/* Second-module copy of func_0040D9B8 (src/us/code/func_0040D9B8.c), with its string/data addresses 0xE0 lower. */
#include "common.h"
#include "structs/game_object.h"
#include "structs/script.h"

/* Damage callback of the second boss: outside phase 9 it only flinches (anim state 6); in phase 9 the damage
 * is taken, a hit effect plays at most every 2.25 s, and at zero health it dies (anim state 9). */


typedef struct {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ s32 damage;
} DamageEvent;

extern BlindOneState D_80130200;
extern u8 D_00432384[];
extern void func_00236314(void *dst, void *src);
extern void func_00243414(void *obj, CAI *ai, s32 state);
extern void func_00206068(s32 arg0);

void func_004469B8(void *obj, CAI *ai, DamageEvent *event) {
    BlindOneState *state;
    s32 health;

    if (ai->m_Health != 0 && ai->m_Mode != 1) {
        state = &D_80130200;
        if (state->phase != 9) {
            ai->m_AnimRepeatedCount = 0;
            func_00243414(obj, ai, 6);
        } else {
            health = ai->m_Health - event->damage;
            if (health < 0) {
                health = 0;
            }
            ai->m_Health = health;
            if (ai->m_VarF32[3] == 0.0f) {
                ai->m_VarF32[3] = 2.25f;
                func_00236314(&state->faders[0], D_00432384);
            }
            if (ai->m_Health == 0) {
                func_00243414(obj, ai, 9);
                func_00206068(8);
            }
        }
    }
}
