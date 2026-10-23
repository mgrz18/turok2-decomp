#include "common.h"

typedef struct { f32 x, y, z; } Vec3;

typedef struct {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ Vec3 pos;
} Actor;

typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 id;
} Def30;

typedef struct {
    /* 0x00 */ u8 pad0[0x30];
    /* 0x30 */ Def30 *def;
    /* 0x34 */ u8 pad34[0x54 - 0x34];
    /* 0x54 */ Vec3 target;
    /* 0x60 */ u8 pad60[0xAC - 0x60];
    /* 0xAC */ Vec3 home;
} AI;

s32 func_00267E74(void);
f32 func_00268534(Actor *);
f32 func_002119FC(f32, f32);
f32 func_002974C0(f32);
f32 func_002982F0(f32);
void func_00239F00(Actor *, Vec3, s32);

void func_0025F504(Actor *self, AI *ai) {
    f32 radius;
    f32 dist;
    f32 angle;
    Vec3 pos;

    if (func_00267E74()) {
        radius = func_00268534(self);
    } else {
        radius = 2048.0f;
    }
    dist = func_002119FC(0.0f, radius);
    angle = func_002119FC(-3.141593f, 3.141593f);
    pos.x = ai->home.x + dist * func_002974C0(angle);
    pos.y = ai->home.y;
    pos.z = ai->home.z + dist * func_002982F0(angle);
    func_00239F00(self, pos, ai->def->id);
    ai->target = self->pos;
}
