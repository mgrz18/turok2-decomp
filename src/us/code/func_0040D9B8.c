#include "common.h"
#include "structs/boss.h"

/* Damage callback of the second boss: outside phase 9 it only flinches (anim state 6); in phase 9 the damage
 * is taken, a hit effect plays at most every 2.25 s, and at zero health it dies (anim state 9). */

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
    /* 0x008 */ u8 pad8[0x34 - 0x8];
    /* 0x034 */ s8 state;
    /* 0x035 */ u8 pad35[0xC7 - 0x35];
    /* 0x0C7 */ s8 animDone;
    /* 0x0C8 */ u8 padC8[0x118 - 0xC8];
    /* 0x118 */ f32 hitTimer;
} CAI;

typedef struct {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ s32 damage;
} DamageEvent;

extern u8 D_00432464[];
extern void func_00236314(void *dst, void *src);
extern void func_00243414(void *obj, CAI *ai, s32 state);
extern void func_00206068(s32 arg0);

void func_0040D9B8(void *obj, CAI *ai, DamageEvent *event) {
    BlindOneState *state;
    s32 health;

    if (ai->health != 0 && ai->state != 1) {
        state = &D_80130200;
        if (state->phase != 9) {
            ai->animDone = 0;
            func_00243414(obj, ai, 6);
        } else {
            health = ai->health - event->damage;
            if (health < 0) {
                health = 0;
            }
            ai->health = health;
            if (ai->hitTimer == 0.0f) {
                ai->hitTimer = 2.25f;
                func_00236314(&state->slots[0], D_00432464);
            }
            if (ai->health == 0) {
                func_00243414(obj, ai, 9);
                func_00206068(8);
            }
        }
    }
}
