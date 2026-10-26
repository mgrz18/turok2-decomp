#include "common.h"

typedef struct Vec3f { f32 x, y, z; } Vec3f;

typedef struct BlastSrc {
    /* 0x00 */ u8 pad0[0x68];
    /* 0x68 */ f32 armor;
} BlastSrc;

typedef struct BlastActor {
    /* 0x000 */ u8 kind;            /* 1: player, 2: object */
    /* 0x001 */ u8 pad1[0x3];
    /* 0x004 */ Vec3f pos;
    /* 0x010 */ u8 pad10[0x54 - 0x10];
    /* 0x054 */ f32 footY;
} BlastActor;

/* The blast being scripted: its source actor and, when set, the armour record to use. */
typedef struct BlastOverride {
    /* 0x00 */ BlastActor *source;
    /* 0x04 */ u8 pad4[0x10];
    /* 0x14 */ s32 id;
    /* 0x18 */ BlastSrc src;
} BlastOverride;

extern BlastOverride D_800F1CE0;
extern BlastSrc D_800F1970;
extern u8 D_8011AAF1;

f32 func_0020F6BC(Vec3f *a, Vec3f *b);
s32 func_0021E374(BlastActor *a, Vec3f pos, BlastSrc *out);
f32 func_002671B4(BlastActor *a);
f32 func_002672C8(BlastActor *a);

/* How much of a blast at `p` reaches `a`: its armour, then (with falloff) a linear drop over
 * the distance to the nearest point of its body, out to `reach` plus its own size. */
f32 func_00238334(BlastActor *a, BlastActor *t, Vec3f p, f32 reach, s32 falloff, u32 flags, BlastSrc **psrc) {
    Vec3f c;
    BlastOverride *o;
    BlastSrc *sv;
    s32 armored;
    s32 ok;
    f32 m;
    f32 lo;
    f32 hi;
    f32 lim;
    f32 dist;

    m = 1.0f;
    armored = 1;
    ok = 1;
    if (D_8011AAF1 != 0 && (flags & 0x400000)) {
        armored = 0;
    }
    if (t->kind == 2 && (o = &D_800F1CE0)->source == a && o->id != -1) {
        sv = &o->src;
        falloff = 0;
    } else if (func_0021E374(a, p, &D_800F1970) != 0) {
        sv = &D_800F1970;
    } else {
        sv = NULL;
    }
    *psrc = sv;
    if (armored && sv != NULL) {
        m *= sv->armor;
    }
    if (!falloff) {
        return m;
    }
    c = a->pos;
    lo = a->pos.y + a->footY;
    hi = lo + func_002671B4(a);
    if (hi < p.y) {
        c.y = hi;
    } else if (p.y < lo) {
        c.y = lo;
    } else {
        c.y = p.y;
    }
    lim = reach + func_002672C8(a);
    dist = func_0020F6BC(&p, &c);
    if (lim < dist) {
        return 0.0f;
    }
    if (ok) {
        m *= 1.0f - dist / lim;
    }
    return m;
}
