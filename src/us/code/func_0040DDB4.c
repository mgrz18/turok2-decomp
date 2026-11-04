#include "common.h"
#include "structs/boss.h"

/* Damage callback of the second boss' minions: takes the damage, plays the minion's hit effect at most every
 * 2.25 s, and at zero health goes to anim state 6. */

typedef struct {
    /* 0x000 */ u8 pad0[0xD4];
    /* 0x0D4 */ s32 flags;
    /* 0x0D8 */ u8 padD8[0x24C - 0xD8];
    /* 0x24C */ s32 unk24C;
} LevelActor;

struct BlindOneState;

typedef struct {
    /* 0x00 */ void (*init)(struct BlindOneState *state);
    /* 0x04 */ void (*update)(struct BlindOneState *state);
} PhaseFuncs;

typedef struct Obj2 {
    /* 0x000 */ u8 pad0[0x140];
    /* 0x140 */ u8 ai[1];
} Obj2;

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

extern u8 D_00432464[];
extern void func_00236314(void *dst, void *src);
extern void func_00243414(void *obj, CAI *ai, s32 state);

void func_0040DDB4(void *obj, CAI *ai, DamageEvent *event) {
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
            func_00236314(&state->slots[ai->slot], D_00432464);
        }
        if (ai->health == 0) {
            func_00243414(obj, ai, 6);
        }
    }
}
