#include "common.h"
#include "structs/vector.h"
#include "structs/gbi.h"
#include "structs/morph.h"






#ifdef TARGET_PORT
/* Round 20 i3: the N64 pad put `remaining` at host offset 0x30 (the owner pointer), not at
 * Swoosh.time (0x3C on the host): the trails faded by the bits of a pointer. */
#include "structs/swoosh.h"
_Static_assert(__builtin_offsetof(MorphOwner, timing) == __builtin_offsetof(Swoosh, info), "MorphOwner.timing");
_Static_assert(__builtin_offsetof(MorphOwner, remaining) == __builtin_offsetof(Swoosh, time), "MorphOwner.remaining");
#endif


extern f32 func_002974C0(f32 x);        /* cosf */
extern void CVector3__Scale(CVector3 *out, CVector3 *in, f32 scale);

#define LERP_COLOR(a, b) ((f32)(a) + t * (f32)((b) - (a)))

/* Blends a vertex buffer between two point sets with a cosine ease: in over the
 * duration, out over the last fadeTime. */
void Morph_BlendVertices(MorphOwner *owner, MorphClock *clock, Vtx_t *dst, Morph *morph) {
    CVector3 a;
    CVector3 b;
    f32 t;
    s32 n;
    MorphItem *item;
    MorphPoint *pb;
    f32 c;
    u8 *ca;
    u8 *cb;

    t = 0.5f - func_002974C0(clock->time / owner->timing->duration * 3.1415927f) * 0.5f;
    if (owner->remaining <= owner->timing->fadeTime) {
        t *= 0.5f - func_002974C0(owner->remaining / owner->timing->fadeTime * 3.1415927f) * 0.5f;
    }
    t = 1.0f - t;
    n = morph->count;
    item = morph->items;
    while (--n != -1) {
        pb = &morph->pointsB[item->pointB];
        CVector3__Scale(&a, &morph->pointsA[item->pointA].pos, morph->scaleA);
        CVector3__Scale(&b, &pb->pos, morph->scaleB);
        dst->ob[0] = a.x + t * (b.x - a.x);
        dst->ob[1] = a.y + t * (b.y - a.y);
        dst->ob[2] = a.z + t * (b.z - a.z);
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
    }
}
