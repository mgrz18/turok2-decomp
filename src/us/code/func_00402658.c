#include "common.h"
#include "structs/vector.h"

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 visible;
    /* 0x08 */ f32 angle;
    /* 0x0C */ u8 padC[0x24 - 0xC];
    /* 0x24 */ f32 dist;
    /* 0x28 */ u8 pad28[0x44 - 0x28];
} Target;

typedef struct {
    /* 0x00 */ Target a;
    /* 0x44 */ Target b;
} Targets;

typedef struct {
    /* 0x00 */ u8 pad0[0x40];
} Path;

typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 unkC;
} Arena;

typedef struct {
    /* 0x000 */ u8 pad0[0x214];
    /* 0x214 */ Arena *arena;
    /* 0x218 */ u8 pad218[0x250 - 0x218];
    /* 0x250 */ s32 unk250;
} Boss;

typedef struct {
    /* 0x000 */ u8 pad0[4];
    /* 0x004 */ CVector3 pos;
    /* 0x010 */ u8 pad10[0xD4 - 0x10];
    /* 0x0D4 */ u32 flags;
} Actor;

typedef struct CAI CAI;

extern Boss D_8012F3A0;
extern s32 D_8012F5F4;

extern void func_00246108(Actor *actor, CAI *ai, Targets *t, s32 f1, s32 f2);
extern void func_0024599C(Actor *actor, CAI *ai, Path *path);
extern void func_00245BAC(Actor *actor, CAI *ai, f32 angle, f32 dist);
extern s32 func_002451AC(Actor *actor, CAI *ai, s32 visible, f32 angle);
extern void func_00243414(Actor *actor, CAI *ai, s32 anim);
extern f32 func_00298470(f32); /* sqrtf */

void func_00402658(Actor *actor, CAI *ai) {
    Targets t;
    Path path;
    CVector3 v;
    Arena *arena;
    Boss *boss = &D_8012F3A0;
    f32 turn;
    s32 side;
    s32 visible;

    arena = boss->arena;
    actor->flags |= 0x2000;
    turn = 0.17453295f;
    if (boss->unk250) {
        turn = 0.3490659f;
    }
    func_00246108(actor, ai, &t, 1, 1);
    func_0024599C(actor, ai, &path);
    func_00245BAC(actor, ai, turn, t.b.angle);
    visible = t.a.visible;
    if (D_8012F5F4 == 0) {
        side = 0;
    } else {
        side = func_002451AC(actor, ai, visible, 0.785398245f);
    }
    if (side == -1) {
        func_00243414(actor, ai, 11);
    } else if (side == 1) {
        func_00243414(actor, ai, 12);
    } else if (boss->unk250) {
        v.x = actor->pos.x;
        v.y = 0.0f;
        v.z = actor->pos.z;
        if (func_00298470(v.x * v.x + v.z * v.z) < 204.79999f) {
            func_00243414(actor, ai, 4);
        }
    } else if (t.b.visible && t.a.visible) {
        if (t.a.angle < 0.0f ? -t.a.angle <= 0.52359885f : t.a.angle <= 0.52359885f) {
            if (t.b.dist > 1228.7999f && arena->unkC) {
                func_00243414(actor, ai, 9);
            } else if (t.b.dist < 921.6f) {
                func_00243414(actor, ai, 10);
            }
        }
    }
}
