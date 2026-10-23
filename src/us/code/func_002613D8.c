#include "common.h"

typedef struct {
    /* 0x00 */ void *target;
    /* 0x04 */ s32 damage;
    /* 0x08 */ f32 radius;
    /* 0x0C */ s32 flags;
    /* 0x10 */ void *shield;
} DamageEvent;

typedef struct {
    /* 0x000 */ u8 pad0[0x140];
    /* 0x140 */ u8 ai[1];
} Actor;

typedef struct {
    /* 0x00 */ Actor *player;
} PlayerState;

typedef struct {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ s32 active;
    /* 0x08 */ u8 pad8[0x34 - 0x8];
    /* 0x34 */ s8 mode;
    /* 0x35 */ u8 pad35[0x80 - 0x35];
    /* 0x80 */ Actor *target;
    /* 0x84 */ u8 pad84[0x4];
    /* 0x88 */ Actor *target2;
} AI;

extern PlayerState D_800F1CE0;

void func_00245828(DamageEvent *event, Actor *source, s32 damage, f32 radius, s32 flags, void *shield);
void func_002465C8(Actor *actor, void *ai, DamageEvent *event);
void func_00243414(Actor *self, AI *ai, s32 mode);

void func_002613D8(Actor *self, AI *ai) {
    DamageEvent event;
    PlayerState *ps;

    if (ai->active != 0 && ai->mode == 0xE) {
        ps = &D_800F1CE0;
        if (ps->player != NULL && (ps->player == ai->target || ps->player == ai->target2)) {
            func_00245828(&event, self, 0x500, 25.599998f, 0x100000, NULL);
            func_002465C8(ps->player, ps->player->ai, &event);
            func_00243414(self, ai, 4);
        }
    }
}
