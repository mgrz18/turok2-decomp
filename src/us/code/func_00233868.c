#include "common.h"

typedef struct Vec3f {
    f32 x, y, z;
} Vec3f;

typedef struct MorphVtx {
    /* 0x00 */ s16 ob[3];
    /* 0x06 */ u16 flag;
    /* 0x08 */ s16 tc[2];
    /* 0x0C */ u8 cn[4];
} MorphVtx;

typedef struct MorphPoint {         /* 0x14 bytes */
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ u8 pad0C[0x8];
} MorphPoint;

typedef struct MorphItem {          /* 0xC bytes */
    /* 0x00 */ u8 colorA[4];
    /* 0x04 */ u8 colorB[4];
    /* 0x08 */ u16 pointA;
    /* 0x0A */ u16 pointB;
} MorphItem;

typedef struct Morph {
    /* 0x00 */ s32 count;
    /* 0x04 */ MorphItem *items;
    /* 0x08 */ MorphPoint *pointsA;
    /* 0x0C */ MorphPoint *pointsB;
    /* 0x10 */ f32 scaleA;
    /* 0x14 */ f32 scaleB;
    /* 0x18 */ f32 texScaleS;
    /* 0x1C */ f32 texScaleT;
    /* 0x20 */ f32 texStepT;
} Morph;

typedef struct MorphTiming {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ f32 fadeTime;
    /* 0x08 */ f32 duration;
} MorphTiming;

typedef struct MorphOwner {
    /* 0x00 */ u8 pad0[0x8];
    /* 0x08 */ MorphTiming *timing;
    /* 0x0C */ u8 padC[0x24 - 0xC];
    /* 0x24 */ f32 remaining;
} MorphOwner;

typedef struct MorphClock {
    /* 0x00 */ u8 pad0[0x8];
    /* 0x08 */ f32 time;
} MorphClock;

extern f32 func_002974C0(f32 x);        /* cosf */
extern void func_0020EF60(Vec3f *out, Vec3f *in, f32 scale);

#define LERP_COLOR(a, b) ((f32)(a) + t * (f32)((b) - (a)))

/* func_0023353C with texture coordinates: S scaled by texS, T stepping by texStepT. */
void func_00233868(MorphOwner *owner, MorphClock *clock, MorphVtx *dst, f32 texS, Morph *morph) {
    Vec3f a;
    Vec3f b;
    f32 t;
    s32 n;
    f32 texT;
    MorphItem *item;
    MorphPoint *pb;
    f32 c;
    u8 *ca;
    u8 *cb;

    t = 0.5f - func_002974C0(clock->time / owner->timing->duration * 3.1415927f) * 0.5f;
    if (owner->remaining <= owner->timing->fadeTime) {
        t *= 0.5f - func_002974C0(owner->remaining / owner->timing->fadeTime * 3.1415927f) * 0.5f;
    }
    texT = 0.0f;
    t = 1.0f - t;
    n = morph->count;
    item = morph->items;
    while (--n != -1) {
        pb = &morph->pointsB[item->pointB];
        func_0020EF60(&a, &morph->pointsA[item->pointA].pos, morph->scaleA);
        func_0020EF60(&b, &pb->pos, morph->scaleB);
        dst->ob[0] = a.x + t * (b.x - a.x);
        dst->ob[1] = a.y + t * (b.y - a.y);
        dst->ob[2] = a.z + t * (b.z - a.z);
        dst->tc[0] = morph->texScaleS * texS;
        dst->tc[1] = morph->texScaleT * texT;
        ca = item->colorA;
        cb = item->colorB;
        c = LERP_COLOR(ca[3], cb[3]);
        dst->cn[0] = (u32)c;
        c = LERP_COLOR(ca[2], cb[2]);
        dst->cn[1] = (u32)c;
        c = LERP_COLOR(ca[1], cb[1]);
        dst->cn[2] = (u32)c;
        c = LERP_COLOR(ca[0], cb[0]);
        dst->cn[3] = (u32)c;
        item++;
        dst++;
        texT += morph->texStepT;
    }
}
