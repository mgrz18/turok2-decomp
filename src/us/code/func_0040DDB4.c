#include "common.h"

/* Damage callback of the second boss' minions: takes the damage, plays the minion's hit effect at most every
 * 2.25 s, and at zero health goes to anim state 6. */

typedef struct {
    /* 0x000 */ u8 pad0[0xD4];
    /* 0x0D4 */ s32 flags;
    /* 0x0D8 */ u8 padD8[0x24C - 0xD8];
    /* 0x24C */ s32 unk24C;
} LevelActor;

typedef struct {
    /* 0x00 */ s32 id;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
} ScriptStep;

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ u8 pad4[0x18 - 0x4];
} Mover;

typedef struct {
    /* 0x00 */ u8 data[0x64];
} Slot2;

struct State2;

typedef struct {
    /* 0x00 */ void (*init)(struct State2 *state);
    /* 0x04 */ void (*update)(struct State2 *state);
} PhaseFuncs;

typedef struct Obj2 {
    /* 0x000 */ u8 pad0[0x140];
    /* 0x140 */ u8 ai[1];
} Obj2;

typedef struct State2 {
    /* 0x000 */ Obj2 *obj;
    /* 0x004 */ s32 count1;
    /* 0x008 */ LevelActor *actors1[4];
    /* 0x018 */ s32 count2;
    /* 0x01C */ LevelActor *actors2[4];
    /* 0x02C */ s32 count3;
    /* 0x030 */ LevelActor *actors3[3];
    /* 0x03C */ Slot2 slots[12];
    /* 0x4EC */ s32 slotCount;
    /* 0x4F0 */ u8 pad4F0[0x4F8 - 0x4F0];
    /* 0x4F8 */ ScriptStep *script;
    /* 0x4FC */ s32 phase;
    /* 0x500 */ u8 pad500[0x518 - 0x500];
    /* 0x518 */ Mover movers[3];
} State2;

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

extern State2 D_80130200;
extern u8 D_00432464[];
extern void func_00236314(void *dst, void *src);
extern void func_00243414(void *obj, CAI *ai, s32 state);

void func_0040DDB4(void *obj, CAI *ai, DamageEvent *event) {
    State2 *state = &D_80130200;
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
