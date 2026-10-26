/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_0027ADBC.c, func_0027ADBC, with Turok 2's layouts. */
/* A projectile's per-frame steering: by type, then homing, thrust and movement. */

#include "common.h"

typedef struct { f32 x, y, z; } Vec3;
typedef struct { f32 x, y, z, w; } Quat;
typedef struct { f32 m[4][4]; } Mtx44;

typedef struct ProjNode {
    /* 0x0000 */ u8 pad0[4];
    /* 0x0004 */ Vec3 pos;
    /* 0x0010 */ u8 pad10[0x54 - 0x10];
    /* 0x0054 */ f32 height;
    /* 0x0058 */ u8 pad58[0x1320 - 0x58];
    /* 0x1320 */ struct ProjNode *next;
} ProjNode;

typedef struct ProjLimits {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u16 thrust;                 /* high half of an f32 */
    /* 0x12 */ u16 maxSpeed;               /* high half of an f32 */
} ProjLimits;

typedef struct ProjDef {
    /* 0x00 */ u32 flags;
    /* 0x04 */ u8 pad4[0xA];
    /* 0x0E */ s16 turnRate;               /* degrees */
    /* 0x10 */ u8 pad10[0x20];
    /* 0x30 */ ProjLimits *limits;
} ProjDef;

typedef struct ProjSound {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ s32 state;
    /* 0x0C */ s32 id;
} ProjSound;

typedef struct Projectile {
    /* 0x000 */ u8 pad0[2];
    /* 0x002 */ u16 type;
    /* 0x004 */ Vec3 pos;
    /* 0x010 */ u8 pad10[8];
    /* 0x018 */ Vec3 velocity;
    /* 0x024 */ u8 pad24[0x40 - 0x24];
    /* 0x040 */ u32 flags;
    /* 0x044 */ u8 pad44[0xF8 - 0x44];
    /* 0x0F8 */ ProjDef *def;
    /* 0x0FC */ u8 padFC[0x114 - 0xFC];
    /* 0x114 */ ProjNode *target;
    /* 0x118 */ u8 pad118[0x150 - 0x118];
    /* 0x150 */ Vec3 heading;
    /* 0x15C */ u8 pad15C[0x1B8 - 0x15C];
    /* 0x1B8 */ ProjSound *sound;
} Projectile;

extern ProjNode *D_80119890;               /* current player */

extern f32 func_0026D210(s32 halfFloat);
extern f32 func_00298470(f32);             /* sqrtf */
extern f32 func_002671B4(ProjNode *node);
extern f32 func_0026841C(ProjNode *node);
extern f32 func_002119FC(f32 lo, f32 hi);   /* random in range */
extern f32 func_0021153C(f32);             /* acosf */
extern void func_0020EF2C(Vec3 *out, Vec3 *a, Vec3 *b);
extern void func_0020EEF8(Vec3 *out, Vec3 *a, Vec3 *b);
extern void func_0020EF60(Vec3 *out, Vec3 *in, f32 scale);
extern void func_0020F70C(Vec3 *v, f32 max);
extern void func_0020F040(Vec3 *v);
extern void func_0020E810(Quat *out, Vec3 *dir);
extern void func_0020DCC8(Quat *out, f32 t, Quat *a, Quat *b);
extern void func_00211114(Quat *q, Mtx44 *out);
extern void func_0020F85C(Mtx44 *m, Vec3 *in, Vec3 *out);
extern void *func_00275F7C(void);
extern ProjSound *func_00275C6C(void *pool, s32 id, Vec3 *pos, Vec3 *pos2, s32 arg4);
extern void func_00232B40(Projectile *p);
extern void func_00229618(Projectile *p, f32 scale);
extern void func_002298C4(Projectile *p);
extern void func_00229D10(Projectile *p);
extern void func_0022A1A4(Projectile *p);

/* func_00232AC4, inlined: (re)start the projectile's looping sound unless it is already playing. */
static __inline__ void PlayLoopingSound(Projectile *o, s32 id) {
    if (o->sound != NULL) {
        if (o->sound->id == id && o->sound->state != -1) {
            return;
        }
        func_00232B40(o);
    }
    o->sound = func_00275C6C(func_00275F7C(), id, &o->pos, &o->pos, -1);
}

static __inline__ void Steer(Projectile *o, f32 a) {
    Vec3 push;
    Vec3 vel;
    f32 len, lim;

    if (a != 0.0f) {
        vel = o->heading;
        func_0020EF60(&push, &vel, a);
        func_0020EEF8(&o->velocity, &o->velocity, &push);
        len = func_00298470(o->velocity.x * o->velocity.x + o->velocity.y * o->velocity.y
                            + o->velocity.z * o->velocity.z);
        lim = func_0026D210(o->def->limits->maxSpeed);
        if (a > 0.0f) {
            if (lim < len) func_0020EF60(&o->velocity, &vel, lim);
        } else {
            if (len < lim) func_0020EF60(&o->velocity, &vel, lim);
        }
    }
}

void func_0022A4C8(Projectile *o) {
    Vec3 d;
    Vec3 tp;
    Vec3 vv;
    Vec3 dir;
    Vec3 ax;
    Quat q0, q1, q2;
    Mtx44 mt;
    ProjNode *p;
    ProjNode *p2;
    Vec3 *tpp;
    Vec3 *velp;
    ProjNode *t;
    ProjDef *def;
    f32 h, fa, fb, fc, fd, k, speed, ang, w, tt;

    switch (o->type) {
        case 0x2E:
            p2 = D_80119890;
            if (p2 != NULL) {
                tpp = &tp;
                velp = &o->velocity;
                fa = 0.5f;
                fb = 2.0f;
                __asm__ __volatile__("");
                p = p2;
                do {
                    h = p->height;
                    tp = p->pos;
                    tp.y += h;
                    tp.y += func_002671B4(p2) * fa;
                    func_0020EF2C(&d, tpp, &o->pos);
                    k = func_00298470(d.x * d.x + d.y * d.y + d.z * d.z);
                    if (k < 512.0f) {
                        k = 1.0f - k * (1.0f / 512);
                        d.x *= k * func_002119FC(fa, fb);
                        d.y *= k * func_002119FC(fa, fb);
                        d.z *= k * func_002119FC(fa, fb);
                        func_0020EF2C(velp, velp, &d);
                    }
                    p2 = p->next;
                    p = p2;
                } while (p2 != NULL);
            }
            t = o->target;
            tt = func_0026841C(t);
            tp = t->pos;
            tp.y += tt;
            func_0020EF2C(&d, &tp, &o->pos);
            fc = 0.3f;
            fd = 0.7f;
            d.x *= func_002119FC(fc, fd);
            d.y *= func_002119FC(fc, fd);
            d.z *= func_002119FC(fc, fd);
            func_0020EEF8(&o->velocity, &o->velocity, &d);
            func_0020F70C(&o->velocity, func_0026D210(o->def->limits->maxSpeed));
            return;
        case 0xE:
            func_0022A1A4(o);
            return;
        case 0xB:
        case 0x129:
            func_002298C4(o);
            return;
        case 0x68:
            func_00229D10(o);
            return;
        case 0x5F:
        case 0x60:
            PlayLoopingSound(o, 0x228);
            break;
    }

    t = o->target;
    if (t != NULL) {
        def = o->def;
        if (def->flags & 0x40000) {
            if (def->turnRate != 0) {
                tt = func_0026841C(t);
                tp = t->pos;
                tp.y += tt;
                tp.y += func_002671B4(t) * 0.5f;
                func_0020EF2C(&dir, &tp, &o->pos);
                func_0020F040(&dir);
                func_0020E810(&q1, &dir);
                func_0020E810(&q0, &o->heading);
                vv = o->heading;
                func_0020F040(&vv);
                speed = func_00298470(o->velocity.x * o->velocity.x + o->velocity.y * o->velocity.y
                                      + o->velocity.z * o->velocity.z);
                ang = func_0021153C(vv.x * dir.x + vv.y * dir.y + vv.z * dir.z);
                tt = (f32)o->def->turnRate * 0.017453294f;
                w = 1.0f;
                if (tt < ang) {
                    w = tt / ang;
                }
                func_0020DCC8(&q2, w, &q0, &q1);
                func_00211114(&q2, &mt);
                *(s32 *)&ax.x = 0;
                *(s32 *)&ax.y = 0;
                ax.z = 1.0f;
                func_0020F85C(&mt, &ax, &o->heading);
                func_0020EF60(&o->velocity, &o->heading, speed);
            }
        }
    }
    if (!(o->flags & 0x70000)) {
        Steer(o, func_0026D210(o->def->limits->thrust));
    }
    if (!(o->flags & 0x30000)) {
        func_00229618(o, 1.0f);
    }
}
