#include "common.h"

/* Projectile wobble: swings the velocity sideways (and then up/down) along
 * sine waves whose amplitude scales with how close the projectile is to its
 * top speed. */

typedef struct { f32 x, y, z; } Vec3f;

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s8 maxSpeed;
} WobbleParams;

typedef struct {
    /* 0x00 */ u8 pad0[0x2C];
    /* 0x2C */ WobbleParams *wobble;
} ProjectileDef;

typedef struct {
    /* 0x000 */ u8 pad0[0x18];
    /* 0x018 */ Vec3f vel;
    /* 0x024 */ u8 pad24[0xF8 - 0x24];
    /* 0x0F8 */ ProjectileDef *def;
    /* 0x0FC */ u8 padFC[0x11C - 0xFC];
    /* 0x11C */ f32 speed;
    /* 0x120 */ u8 pad120[0x150 - 0x120];
    /* 0x150 */ Vec3f dir;
    /* 0x15C */ u8 pad15C[0x178 - 0x15C];
    /* 0x178 */ f32 phaseX;
    /* 0x17C */ f32 phaseY;
    /* 0x180 */ f32 freqX;
    /* 0x184 */ f32 freqY;
    /* 0x188 */ f32 ampX;
    /* 0x18C */ f32 ampY;
} Projectile;

extern f32 func_002982F0(f32);                              /* sinf */
extern void func_0020EFDC(Vec3f *out, Vec3f *a, Vec3f *b);  /* out = a x b */
extern void func_0020EF60(Vec3f *out, Vec3f *in, f32 s);    /* out = in * s */
extern void func_0020EF2C(Vec3f *out, Vec3f *a, Vec3f *b);  /* out = a - b */
extern void func_0020EEF8(Vec3f *out, Vec3f *a, Vec3f *b);  /* out = a + b */
extern f32 D_800B6D28;                                      /* frame time scale */

void func_00229618(Projectile *p, f32 scale) {
    Vec3f up;
    Vec3f axis;
    Vec3f dir;
    Vec3f before;
    Vec3f after;
    Vec3f side;
    f32 maxSpeed;
    f32 k;

    k = 1.0f;
    maxSpeed = p->def->wobble->maxSpeed;
    if (p->speed < maxSpeed) {
        k = p->speed / maxSpeed;
    }
    k *= scale;
    dir = p->dir;
    if (p->ampX > 0.0f) {
        up.x = 0.0f;
        up.y = 1.0f;
        up.z = 0.0f;
        func_0020EFDC(&axis, &up, &dir);
        func_0020EF60(&before, &axis, func_002982F0(p->phaseX) * p->ampX * k);
        p->phaseX += p->freqX * k * D_800B6D28;
        func_0020EF60(&after, &axis, func_002982F0(p->phaseX) * p->ampX * k);
        func_0020EF2C(&p->vel, &p->vel, &before);
        func_0020EEF8(&p->vel, &p->vel, &after);
    } else {
        p->phaseX += p->freqX * k * D_800B6D28;
    }
    if (p->ampY > 0.0f) {
        up.x = 0.0f;
        up.y = 1.0f;
        up.z = 0.0f;
        func_0020EFDC(&side, &up, &dir);
        func_0020EFDC(&axis, &side, &dir);
        func_0020EF60(&before, &axis, func_002982F0(p->phaseY) * p->ampY * k);
        p->phaseY += p->freqY * k * D_800B6D28;
        func_0020EF60(&after, &axis, func_002982F0(p->phaseY) * p->ampY * k);
        func_0020EF2C(&p->vel, &p->vel, &before);
        func_0020EEF8(&p->vel, &p->vel, &after);
    } else {
        p->phaseY += p->freqY * k * D_800B6D28;
    }
}
