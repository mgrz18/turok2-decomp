/* Boundary fixed in versions/cuts.us.txt: splat starts this function at 0x26E758 with two stray words
 * (`sw $v0,0x10($s0); j .L0023587C`); the real function is func_0026E760
 * (func_002851C0 calls it there). */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/RENDER/8028469C_17.CPP, func_802856E0_de, with Turok 2's layouts. */
#include "common.h"

typedef struct {
    f32 x, y, z;
} Vec3f;

/* A sound/effect emitter attenuated by distance. */
typedef struct Emitter {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ struct Emitter *next;
    /* 0x08 */ u8 pad8[4];
    /* 0x0C */ Vec3f pos;
    /* 0x18 */ f32 radius;
    /* 0x1C */ f32 gain;
    /* 0x20 */ u8 curveA[0xC];
    /* 0x2C */ u8 curveB[0xC];
} Emitter;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ Emitter *emitters;
    /* 0x18 */ u8 pad18[0x10];
    /* 0x28 */ f32 base;
} EmitterList;

extern s32 func_0029DFF0(void);         /* disable interrupts */
extern void func_0029E010(s32 mask);    /* restore interrupts */
extern s32 func_0027BC34(void);
extern s32 Cinema_PathPlaying(void);
extern f32 func_00298470(f32);          /* sqrtf */
extern f32 func_0026EF30(void *curve);

#define MIN(a, b) ((a) > (b) ? (b) : (a))
#define MAX(a, b) ((a) < (b) ? (b) : (a))

static __inline__ f32 distance(Emitter *e, Vec3f a) {
    return func_00298470((a.x - e->pos.x) * (a.x - e->pos.x) + (a.y - e->pos.y) * (a.y - e->pos.y)
                         + (a.z - e->pos.z) * (a.z - e->pos.z));
}

/* Summed gain of a list's emitters heard from a point, clamped to [0, 1]. */
f32 func_0026E760(EmitterList *list, Vec3f pos) {
    Emitter *e;
    s32 mask;
    f32 total;
    f32 dist;
    f32 falloff;
    f32 a;

    total = list->base;
    mask = func_0029DFF0();
    if (func_0027BC34() || Cinema_PathPlaying()) {
        total = 0.0f;
    } else {
        for (e = list->emitters; e != NULL; e = e->next) {
            dist = distance(e, pos);
            if (dist == 0.0f || e->radius == 0.0f) {
                falloff = 1.0f;
            } else {
                falloff = 0.0f;
                if (dist < e->radius) {
                    falloff = 1.0f - dist / e->radius;
                }
            }
            falloff *= e->gain;
            a = func_0026EF30(e->curveA);
            total += falloff * a * func_0026EF30(e->curveB);
        }
    }
    func_0029E010(mask);
    return MAX(MIN(total, 1.0f), 0.0f);
}
