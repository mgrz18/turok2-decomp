#include "common.h"

typedef struct Vec3 {
    f32 x, y, z;
} Vec3;

typedef f32 Mtx4[4][4];

typedef struct MoverDef {
    s32 type;          /* 0x00 */
} MoverDef;

typedef struct Mover {
    u8 kind;           /* 0x00 */
    char pad1[3];
    Vec3 pos;          /* 0x04 */
    char pad10[4];
    MoverDef *def;     /* 0x14 */
} Mover;

typedef struct Actor {
    char pad0[4];
    Vec3 pos;          /* 0x04 */
    char pad10[0x18 - 0x10];
    Vec3 velocity;     /* 0x18 */
    char pad24[0x50 - 0x24];
    f32 yaw;           /* 0x50 */
} Actor;

typedef struct Dest {
    u32 flags;         /* 0x00 */
} Dest;

extern char D_800F1E10[];

Mover *func_00267800(Actor *actor);
void func_00268798(Mover *mover, Vec3 *move);
f32 func_002687FC(Mover *mover);
void func_0020EEF8(Vec3 *out, Vec3 *a, Vec3 *b);
void func_0020EF2C(Vec3 *out, Vec3 *a, Vec3 *b);
void func_002105A4(Mtx4 *mtx, f32 angle);
void func_0020F908(Mtx4 *mtx, Vec3 *in, Vec3 *out, s32 count);
void func_00239F00(Actor *actor, Vec3 pos, void *arg);
void func_002640D4(Mover *mover, Actor *actor);

/* Carries an actor standing on a moving platform along with it. */
void func_00242C00(Actor *actor, Dest *dst) {
    Vec3 vel;
    Vec3 pos;
    Vec3 delta;
    Mtx4 mtx;
    Mover *mover;
    f32 turn;

    mover = func_00267800(actor);
    if (mover != NULL) {
        if (mover->def->type == 2) {
            dst->flags = (dst->flags & ~0x80000) | 0x40000;
            pos = actor->pos;
            func_00268798(mover, &vel);
            if (!(dst->flags & 0x80) || vel.y > 0.0f) {
                func_0020EEF8(&pos, &pos, &vel);
            }
            vel = actor->velocity;
            actor->velocity.x = 0.0f;
            actor->velocity.y = 0.0f;
            actor->velocity.z = 0.0f;
            func_0020EF2C(&delta, &pos, &mover->pos);
            turn = func_002687FC(mover);
            func_002105A4(&mtx, turn);
            func_0020F908(&mtx, &delta, &delta, 1);
            func_0020EEF8(&delta, &delta, &mover->pos);
            func_0020EF2C(&delta, &delta, &pos);
            actor->yaw += turn;
            func_0020EEF8(&pos, &pos, &delta);
            func_00239F00(actor, pos, D_800F1E10);
            actor->velocity = vel;
            if (mover->kind == 1 && mover->def->type == 2) {
                func_002640D4(mover, actor);
            }
        } else {
            dst->flags = (dst->flags & ~0x40000) | 0x80000;
        }
    } else {
        dst->flags &= ~0xC0000;
    }
}
