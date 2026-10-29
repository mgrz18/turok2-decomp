#include "common.h"
#include "structs/game_object.h"

/* What moves: the start of any instance. */
typedef struct Mover {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ CVector3 pos;
    /* 0x10 */ u8 pad10[8];
    /* 0x18 */ CVector3 vel;
} Mover;

typedef struct CollisionDef {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ f32 bounce;
} CollisionDef;

/* A segment swept from an instance's position to end, and what it hit
 * (see func_00239F00). */
typedef struct Sweep {
    /* 0x00 */ Mover *obj;
    /* 0x04 */ CollisionDef *def;
    /* 0x08 */ f32 fraction;            /* of the move done before the hit */
    /* 0x0C */ s32 type;                /* 1 slide, 2 stop, 3 go on to slideTo */
    /* 0x10 */ CVector3 vel;            /* velocity after the hit */
    /* 0x1C */ CVector3 slideTo;
    /* 0x28 */ CVector3 pos;            /* where the hit is */
    /* 0x34 */ u8 pad34[0x18];
    /* 0x4C */ CVector3 end;
    /* 0x58 */ CVector3 delta;          /* end - obj->pos */
    /* 0x64 */ CVector3 lastNormal;
    /* 0x70 */ CVector3 normal;
    /* 0x7C */ u8 pad7C[8];
    /* 0x84 */ s32 lastSliding;
    /* 0x88 */ s32 sliding;
    /* 0x8C */ s32 more;                /* another pass follows */
    /* 0x90 */ s32 physics;
    /* 0x94 */ s32 onGround;
} Sweep;

typedef struct CollisionOut {
    /* 0x00 */ u8 pad0[0xE4];
    /* 0xE4 */ CVector3 normal;
} CollisionOut;

extern CollisionOut *D_800F1ADC;

void func_0020EF2C(CVector3 *out, CVector3 *a, CVector3 *b);           /* out = a - b */
void func_0020EEF8(CVector3 *out, CVector3 *a, CVector3 *b);           /* out = a + b */
void func_0020EF60(CVector3 *out, CVector3 *v, f32 k);                 /* out = v * k */
void func_0020EF8C(CVector3 *out, f32 t, CVector3 *a, CVector3 *b);    /* out = a + (b - a) * t */
void func_0020F144(CVector3 *out, CVector3 *v, CVector3 *n, f32 bounce); /* bounce off n */
void func_0020F1D8(CVector3 *out, CVector3 *v, CVector3 *n);           /* slide along n */
void func_0020F3C0(CVector3 *out, CVector3 *v, CVector3 *n);
f32 func_00298470(f32);                                                /* sqrtf */

#define DOT(a, b) ((a)->x * (b)->x + (a)->y * (b)->y + (a)->z * (b)->z)

/* Tests the sweep against the plane through point with this normal; on a
 * nearer hit records it and answers it the way kind says (1 stop, 2 slide,
 * 3 bounce, 4 and 5 other slides, 6 slide or bounce, 7 pass through).
 * Returns 1 when the hit was taken. */
s32 func_0023AB18(Sweep *sweep, CVector3 *normal, CVector3 *point, s32 frontOnly, s32 kind, f32 friction,
                  s32 allowBehind) {
    CVector3 toPoint;
    CVector3 dir;
    CVector3 rest;
    CVector3 bounced;
    CVector3 move;
    CVector3 a;
    CVector3 b;
    CVector3 push;
    CVector3 vel;
    f32 denom;
    f32 t;
    f32 len;
    f32 xx;
    f32 zz;

    if (kind == 0) {
        return 0;
    }
    if (sweep->lastSliding == 1 && sweep->lastNormal.x == normal->x && sweep->lastNormal.y == normal->y
        && sweep->lastNormal.z == normal->z) {
        return 0;
    }
    denom = DOT(normal, &sweep->delta);
    if (denom == 0.0f) {
        return 0;
    }
    if (frontOnly && denom > 0.0f) {
        return 0;
    }
    func_0020EF2C(&toPoint, point, &sweep->obj->pos);
    t = DOT(normal, &toPoint) / denom;
    if (sweep->fraction <= t) {
        return 0;
    }
    if (t < 0.0f && !allowBehind) {
        return 0;
    }
    if (t < -0.2f) {
        t = -0.2f;
    }
    D_800F1ADC->normal = *normal;
    sweep->fraction = t;
    func_0020EF8C(&sweep->pos, t, &sweep->obj->pos, &sweep->end);
    if (kind == 6) {
        kind = 3;
        if (sweep->onGround) {
            kind = 2;
        }
    }
    if (kind == 3 && sweep->def->bounce < 1.0f) {
        xx = sweep->delta.x * sweep->delta.x;
        zz = sweep->delta.z * sweep->delta.z;
        if (xx + sweep->delta.y * sweep->delta.y + zz < 16.777218f) {
            kind = 2;
            if (xx + zz < 1.0485761f) {
                kind = 1;
            }
        }
    }
    switch (kind) {
        case 7:
            sweep->sliding = 0;
            sweep->type = 1;
            if (sweep->physics) {
                sweep->vel = sweep->obj->vel;
            }
            sweep->slideTo = sweep->end;
            return 1;
        case 1:
            sweep->sliding = 0;
            sweep->type = 2;
            if (sweep->physics) {
                sweep->vel.x = sweep->vel.y = sweep->vel.z = 0.0f;
            }
            break;
        case 3:
            sweep->sliding = 0;
            if (sweep->physics || sweep->more) {
                len = func_00298470(normal->x * normal->x + normal->y * normal->y + normal->z * normal->z);
                if (len == 0.0f) {
                    return 0;
                }
                func_0020EF60(&dir, normal, 1.0f / len);
            }
            if (sweep->physics) {
                func_0020F144(&vel, &sweep->obj->vel, &dir, sweep->def->bounce);
                sweep->vel = vel;
                func_0020EF60(&sweep->vel, &sweep->vel, 1.0f - friction);
            }
            if (sweep->more) {
                sweep->type = 3;
                func_0020EF60(&rest, &sweep->delta, 1.0f - t);
                func_0020F144(&bounced, &rest, &dir, sweep->def->bounce);
                func_0020EF60(&bounced, &bounced, 1.0f - friction);
                func_0020EEF8(&sweep->slideTo, &sweep->pos, &bounced);
            } else {
                sweep->type = 2;
            }
            break;
        case 2:
            sweep->sliding = 1;
            sweep->normal = *normal;
            if (sweep->physics || sweep->more) {
                len = func_00298470(normal->x * normal->x + normal->y * normal->y + normal->z * normal->z);
                if (len == 0.0f) {
                    return 0;
                }
                func_0020EF60(&dir, normal, 1.0f / len);
            }
            if (sweep->physics) {
                func_0020F1D8(&vel, &sweep->obj->vel, &dir);
                sweep->vel = vel;
                func_0020EF60(&sweep->vel, &sweep->vel, 1.0f - friction);
            }
            if (sweep->more) {
                sweep->type = 3;
                func_0020EF60(&rest, &sweep->delta, 1.0f - t);
                func_0020F1D8(&move, &rest, &dir);
                func_0020EF60(&move, &move, 1.0f - friction);
                func_0020EEF8(&sweep->slideTo, &sweep->pos, &move);
                func_0020EF60(&push, &dir, 0.2048f);
                func_0020EEF8(&sweep->slideTo, &sweep->slideTo, &push);
            } else {
                sweep->type = 2;
            }
            break;
        case 4:
            sweep->sliding = 1;
            sweep->normal = *normal;
            if (sweep->physics || sweep->more) {
                len = func_00298470(normal->x * normal->x + normal->y * normal->y + normal->z * normal->z);
                if (len == 0.0f) {
                    return 0;
                }
                func_0020EF60(&dir, normal, 1.0f / len);
            }
            if (sweep->physics) {
                func_0020F3C0(&vel, &sweep->obj->vel, &dir);
                sweep->vel = vel;
            }
            if (sweep->more) {
                sweep->type = 3;
                func_0020EF60(&rest, &sweep->delta, 1.0f - t);
                func_0020F3C0(&move, &rest, &dir);
                func_0020EEF8(&sweep->slideTo, &sweep->pos, &move);
                func_0020EF60(&push, &dir, 0.2048f);
                func_0020EEF8(&sweep->slideTo, &sweep->slideTo, &push);
            } else {
                sweep->type = 2;
            }
            break;
        case 5:
            sweep->sliding = 1;
            sweep->normal = *normal;
            if (sweep->physics || sweep->more) {
                len = func_00298470(normal->x * normal->x + normal->y * normal->y + normal->z * normal->z);
                if (len == 0.0f) {
                    return 0;
                }
                func_0020EF60(&dir, normal, 1.0f / len);
            }
            if (sweep->physics) {
                func_0020F1D8(&a, &sweep->obj->vel, &dir);
                func_0020F3C0(&b, &sweep->obj->vel, &dir);
                func_0020EF8C(&sweep->vel, 0.25f, &a, &b);
                func_0020EF60(&sweep->vel, &sweep->vel, 1.0f - friction);
            }
            if (sweep->more) {
                sweep->type = 3;
                func_0020EF60(&rest, &sweep->delta, 1.0f - t);
                func_0020F1D8(&a, &rest, &dir);
                func_0020F3C0(&b, &rest, &dir);
                func_0020EF8C(&move, 0.25f, &a, &b);
                func_0020EF60(&move, &move, 1.0f - friction);
                func_0020EEF8(&sweep->slideTo, &sweep->pos, &move);
                func_0020EF60(&push, &dir, 0.2048f);
                func_0020EEF8(&sweep->slideTo, &sweep->slideTo, &push);
            } else {
                sweep->type = 2;
            }
            break;
        case 6:
            break;
    }
    return 1;
}
