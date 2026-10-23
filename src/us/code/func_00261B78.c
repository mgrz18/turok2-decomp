#include "common.h"

typedef struct { f32 x, y, z; } Vec3;

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
    /* 0x04 */ u8 pad4[0x9C - 0x4];
    /* 0x9C */ s32 unk9C;
} PlayerState;

typedef struct {
    /* 0x00 */ u8 pad0[0x34];
    /* 0x34 */ s8 mode;
    /* 0x35 */ u8 pad35[0x88 - 0x35];
    /* 0x88 */ Actor *target;
    /* 0x8C */ u8 pad8C[0x9C - 0x8C];
    /* 0x9C */ Vec3 dest;
    /* 0xA8 */ u8 padA8[0x4];
    /* 0xAC */ Vec3 home;
} AI;

extern PlayerState D_800F1CE0;

void func_00245828(DamageEvent *event, Actor *source, s32 damage, f32 radius, s32 flags, void *shield);
void func_002465C8(Actor *actor, void *ai, DamageEvent *event);
void func_00243414(Actor *self, AI *ai, s32 mode);

void func_00261B78(Actor *self, AI *ai) {
    DamageEvent event;

    if (D_800F1CE0.unk9C != 0) {
        ai->home = ai->dest;
    }
    if (D_800F1CE0.player != NULL && ai->mode == 0x10 && D_800F1CE0.player == ai->target) {
        func_00245828(&event, self, 0x200, 25.599998f, 0x100000, NULL);
        func_002465C8(D_800F1CE0.player, D_800F1CE0.player->ai, &event);
        func_00243414(self, ai, 6);
    }
}
