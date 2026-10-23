#include "common.h"

typedef struct { f32 x, y, z; } Vec3;

typedef struct {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ Vec3 pos;
} Actor;

typedef struct {
    /* 0x00 */ u8 pad0[0x64];
    /* 0x64 */ f32 timer;
} AI;

typedef struct {
    /* 0x00 */ void *target;
    /* 0x04 */ s32 damage;
    /* 0x08 */ f32 radius;
    /* 0x0C */ s32 flags;
    /* 0x10 */ void *shield;
} DamageEvent;

extern f32 D_800B6D28; /* frame delta */

void func_00245828(DamageEvent *event, void *target, s32 damage, f32 radius, s32 flags, void *shield);
void func_002465C8(Actor *self, AI *ai, DamageEvent *event);
void func_00275544(s16 sound, Vec3 pos, s32 arg4, s32 arg5);
void func_00243414(Actor *self, AI *ai, s32 mode);

void func_0025FAD0(Actor *self, AI *ai) {
    DamageEvent event;

    if (ai->timer > 300.0f) {
        ai->timer = 300.0f;
    }
    func_00245828(&event, NULL, D_800B6D28 * 34.133335f, 51.199997f, 0x100000, NULL);
    func_002465C8(self, ai, &event);
    func_00275544(0x227, self->pos, 0, -1);
    ai->timer -= D_800B6D28;
    if (ai->timer <= 0.0f) {
        func_00243414(self, ai, 10);
    }
}
