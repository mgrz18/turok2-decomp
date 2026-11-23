#include "common.h"
#include "structs/vector.h"
#include "structs/gbi.h"
#include "structs/morph.h"
#include "structs/swoosh.h"

extern f32 func_002974C0(f32 x);        /* cosf */
extern void CVector3__Scale(CVector3 *out, CVector3 *in, f32 scale);

#define LERP_COLOR(a, b) ((f32)(a) + t * (f32)((b) - (a)))

/* Morph_BlendVertices with texture coordinates: S scaled by texS, T stepping by texStepT. */
void Morph_BlendVerticesTex(Swoosh *owner, SwooshNode *clock, Vtx_t *dst, f32 texS, Morph *morph) {
    CVector3 a;
    CVector3 b;
    f32 t;
    s32 n;
    f32 texT;
    MorphItem *item;
    MorphPoint *pb;
    f32 c;
    u8 *ca;
    u8 *cb;

    t = 0.5f - func_002974C0(clock->life / owner->info->nodeLife * 3.1415927f) * 0.5f;
    if (owner->time <= owner->info->fadeTime) {
        t *= 0.5f - func_002974C0(owner->time / owner->info->fadeTime * 3.1415927f) * 0.5f;
    }
    texT = 0.0f;
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
