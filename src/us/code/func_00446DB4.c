/* Second-module copy of func_0040DDB4 (src/us/code/func_0040DDB4.c), with its string/data addresses 0xE0 lower. */
#include "common.h"
#include "structs/script.h"

/* Damage callback of the second boss' minions: takes the damage, plays the minion's hit effect at most every
 * 2.25 s, and at zero health goes to anim state 6. */

typedef struct {
    /* 0x000 */ u8 pad0[0x4];
    /* 0x004 */ s32 health;
    /* 0x008 */ u8 pad8[0x114 - 0x8];
    /* 0x114 */ s32 slot;
    /* 0x118 */ f32 hitTimer;
} CAI;

typedef struct {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ s32 damage;
} DamageEvent;

extern BlindOneState D_80130200;
extern u8 D_00432384[];
extern void func_00236314(void *dst, void *src);
extern void func_00243414(void *obj, CAI *ai, s32 state);

void func_00446DB4(void *obj, CAI *ai, DamageEvent *event) {
    BlindOneState *state = &D_80130200;
    s32 health;

    if (ai->health != 0) {
        health = ai->health - event->damage;
        if (health < 0) {
            health = 0;
        }
        ai->health = health;
        if (ai->hitTimer == 0.0f) {
            ai->hitTimer = 2.25f;
            func_00236314(&state->faders[ai->slot], D_00432384);
        }
        if (ai->health == 0) {
            func_00243414(obj, ai, 6);
        }
    }
}
