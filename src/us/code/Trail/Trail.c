/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802A25D0_de_3.c, func_802A25D0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py.
 * On structs/swoosh.h's Swoosh and SwooshNode (Rage Wars' ObjectLinks54 / ObjectLinksB4). */

#include "common.h"
#include "structs/swoosh.h"

SwooshNode *Swoosh_TakeNode(CSwooshSystem *arg0, Swoosh *arg1);
void CMtxF__GetTranslation(void *mf, void *out);
void CMtxF__GetScale(void *mf, void *out);
void CMtxF__ToMtxAffine(void *mf, void *mtx);
void CVector3__Subtract(void *out, void *a, void *b);
extern f32 func_00298470(f32);

/* Adds a point to swoosh arg1 at matrix arg2 and spreads the points along its length. */
SwooshNode *Swoosh_NewNode(CSwooshSystem *arg0, Swoosh *arg1, void *arg2) {
    CVector3 sp10;
    f32 temp_f0;
    f32 temp_f0_2;
    f32 var_f1;
    SwooshNode *temp_s1;
    SwooshNode *var_s0;
    SwooshNode *temp_s2;
    temp_s2 = Swoosh_TakeNode(arg0, arg1);
    var_s0 = temp_s2;
    if (temp_s2 != 0) {
        arg1->unk50 = 0;
        var_s0->life = arg1->info->nodeLife;
        var_s0->owner = 0;
        CMtxF__GetTranslation(arg2, &var_s0->pos.x);
        if (arg1->flags & 4) {
            CMtxF__GetScale(arg2, &var_s0->scale.x);
        } else {
            CMtxF__ToMtxAffine(arg2, &var_s0->mtx[0]);
            CMtxF__ToMtxAffine(arg2, &var_s0->mtx[1]);
        }
        temp_s1 = var_s0->prev;
        *(s32 *) &var_s0->length = 0;
        var_s0->fraction = 0.0f;
        if (temp_s1 != 0) {
            CVector3__Subtract(&sp10, &var_s0->pos, &temp_s1->pos);
            temp_f0 = func_00298470((sp10.x * sp10.x) + (sp10.y * sp10.y) + (sp10.z * sp10.z));
            temp_s1->length = temp_f0;
            arg1->length += temp_f0;
        }
        if (arg1->info->texMode == 0) {
            if (arg1->length > 0.0f) {
                var_f1 = 0.0f;
                var_s0 = arg1->nodes.head;
                if (var_s0 != 0) {
                    do {
                        var_s0->fraction = var_f1 / arg1->length;
                        temp_f0_2 = var_s0->length;
                        var_s0 = var_s0->next;
                        var_f1 += temp_f0_2;
                    } while (var_s0 != 0);
                }
            }
        } else {
            if (temp_s1 != 0) {
                arg1->unk28 += temp_s1->length * 0.005f;
            }
            var_s0->fraction = arg1->unk28;
        }
    }
    return temp_s2;
}

#include "structs/vector.h"
#include "structs/gbi.h"
#include "structs/morph.h"

extern f32 func_002974C0(f32 x);        /* cosf */
void CVector3__Scale(void *out, void *in, f32 scale);
#define LERP_COLOR(a, b) ((f32)(a) + t * (f32)((b) - (a)))

/* Blends a vertex buffer between two point sets with a cosine ease: in over the
 * duration, out over the last fadeTime. */
void Morph_BlendVertices(Swoosh *owner, SwooshNode *clock, Vtx_t *dst, Morph *morph) {
    CVector3 a;
    CVector3 b;
    f32 t;
    s32 n;
    MorphItem *item;
    MorphPoint *pb;
    f32 c;
    u8 *ca;
    u8 *cb;

    t = 0.5f - func_002974C0(clock->life / owner->info->nodeLife * 3.1415927f) * 0.5f;
    if (owner->time <= owner->info->fadeTime) {
        t *= 0.5f - func_002974C0(owner->time / owner->info->fadeTime * 3.1415927f) * 0.5f;
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

#include "globals/engine.h"
#include "structs/game_object.h"

void SwooshGroup_Remove(CSwooshSystem *arg0, Swoosh *arg1, SwooshNode *arg2);
void Swoosh_ReleaseOwner(Swoosh *arg0);

s32 Swoosh_Update(sptr a0, Swoosh *s) {
    f32 saved, k1, k2;
    SwooshNode *e, *next;
    CGameObjectInstance *ow;
    s32 r;
    saved = gFrameIncrement;
    if (s->flags & 2) {
        gFrameIncrement = saved * gParticleTimeScale;
    }
    if (s->flags & 1) {
        gFrameIncrement = gFrameIncrement * gEnemyTimeScale;
    }
    if (s->flags & 8) {
        s->unk50 = s->unk50 + 1;
        if (s->unk50 >= 3) {
            s->owner = 0;
        }
    }
    if (s->endFrame >= 0.0f) {
        s->frame = s->frame + gFrameIncrement;
    }
    s->time = s->time - gFrameIncrement;
    if (s->time < 0.0f) {
        s->time = 0.0f;
    }
    e = s->nodes.head;
    if (e != 0) {
        k1 = 1.0f;
        k2 = -3.0f;
        do {
            next = e->next;
            if (s->time <= 0.0f && e->life > 0.0f) {
                e->life = 0.0f;
            }
            if (e->life <= 0.0f) {
                e->life = e->life - k1;
                if (e->life < k2) {
                    SwooshGroup_Remove(a0, s, e);
                }
            } else {
                e->life = e->life - gFrameIncrement;
                if (e->life < 0.0f) {
                    e->life = 0.0f;
                }
            }
            e = next;
        } while (e != 0);
    }
    ow = (CGameObjectInstance *)s->owner;
    if (ow != 0 && (s->flags & 1) && !(ow->m_dwFlags & 0x200)) {
        Swoosh_ReleaseOwner(s);
    }
    gFrameIncrement = saved;
    if (s->nodes.count != 0 || (s->time != 0.0f && s->owner != 0)) {
        return 1;
    }
    return 0;
}

void CVector3__Cross(void *out, void *a, void *b);
void CVector3__Normalize(void *v);
void CMtxF__Identity(void *mf);
void CMtxF__RotateVector(u8 *p, void *a, void *b);

void Trail_ComputeNodeFrame(SwooshNode *e0, SwooshNode *e1, u8 *arg2, SwooshFrame *out) {
    SwooshFrameRow v10, v20, v30, v40, v50;
    f32 ty, tz;
    CVector3__Subtract(&v20, &e0->pos, &e1->pos);
    CVector3__Normalize(&v20);
    *(s32 *)&v10.x = 0;
    *(s32 *)&v10.y = 0;
    v10.z = 1.0f;
    CMtxF__RotateVector(arg2 + 0x18C, &v10, &v30);
    ty = -v30.y;
    tz = -v30.z;
    v30.y = ty;
    v30.z = tz;
    CVector3__Scale(&v40, &v30, v30.x * v20.x + ty * v20.y + tz * v20.z);
    CVector3__Subtract(&v40, &v20, &v40);
    CVector3__Normalize(&v40);
    CVector3__Cross(&v50, &v30, &v40);
    CVector3__Normalize(&v50);
    CVector3__Scale(&v50, &v50, e0->scale.x);
    CVector3__Scale(&v40, &v40, e0->scale.y);
    CVector3__Scale(&v30, &v30, e0->scale.z);
    CMtxF__Identity(out);
    out->a.x = v50.x;
    out->a.y = v50.y;
    out->a.z = v50.z;
    out->b.x = v40.x;
    out->b.y = v40.y;
    out->b.z = v40.z;
    out->c.x = v30.x;
    out->c.y = v30.y;
    out->c.z = v30.z;
    out->d.x = e0->pos.x;
    out->d.y = e0->pos.y;
    out->d.z = e0->pos.z;
}

void CMtxF__SetScale(void *mf, f32 x, f32 y, f32 z);

/* The swoosh's points (structs/swoosh.h): each one's frame (Trail_ComputeNodeFrame) toward the next
 * point at another position, as an Mtx in this frame buffer's slot. */
void Trail_ComputeFrames(void *arg0, sptr arg1) {
    f32 matrix[16];
    SwooshNode *first;
    SwooshNode *next;
    Swoosh *state = arg0;
    if (state->nodes.count < 2) {
        return;
    }
    first = state->nodes.head;
    next = first->next;
    if (first != 0) {
        CMtxF__SetScale(matrix, first->scale.x, first->scale.y, first->scale.z);
    } else {
        CMtxF__Identity(matrix);
    }
    while (first != 0) {
        SwooshNode *a;
        SwooshNode *b;
        if (next == 0) {
            break;
        }
        a = first;
        b = next;
        if (first == 0) {
            goto no_pair;
        }
        if (first->pos.x != next->pos.x || first->pos.y != next->pos.y) {
            goto boundary;
        }
z_test:
        if (a->pos.z != b->pos.z) {
            goto boundary;
        }
        a = b;
        b = b->next;
        if (a == 0) {
            goto no_pair;
        }
        if (b == 0 || a->pos.x != b->pos.x || a->pos.y != b->pos.y) {
            goto boundary;
        }
        goto z_test;
boundary:
        if (a != 0 && b != 0) {
            Trail_ComputeNodeFrame(a, b, (u8 *) arg1, (SwooshFrame *) matrix);
            goto apply;
        }
no_pair:
        {
            matrix[12] = first->pos.x;
            matrix[13] = first->pos.y;
            matrix[14] = first->pos.z;
        }
apply:
        CMtxF__ToMtxAffine(matrix, &first->mtx[gEvenOdd]);
        first = next;
        next = first->next;
    }
    matrix[12] = first->pos.x;
    matrix[13] = first->pos.y;
    matrix[14] = first->pos.z;
    CMtxF__ToMtxAffine(matrix, &first->mtx[gEvenOdd]);
}

#include "structs/particle.h"

void CMtxF__FromMtxAffine(void *mf, void *mtx);
void CMtxF__ClampEntries(void *mf);
void CMtxF__ScaleAxes(void *mf, f32 x, f32 y, f32 z);
f32 func_00298470(f32);                 /* sqrtf */

/* Rage Wars' func_802A6174: places one node on its owner, or freezes it when the owner went away (-1). */
static __inline__ void placeNode(Swoosh *e, SwooshNode *n, CParticle *o) {
    u8 m[0x40];
    f32 k;

    if (o == (CParticle *)-1) {
        n->mtx[gEvenOdd] = n->mtx[gEvenOdd ^ 1];
        n->owner = NULL;
    } else {
        CMtxF__FromMtxAffine(m, o->m_mtxDraw[gEvenOdd]);
        k = 0.01f;
        if (o->m_pEffect->m_pModel != NULL) {
            k = 0.29257143f;
        }
        CMtxF__ScaleAxes(m, k, k, k);
        CMtxF__ClampEntries(m);
        CMtxF__GetTranslation(m, &n->pos);
        if (e->flags & 4) {
            CMtxF__GetScale(m, &n->scale);
        } else {
            CMtxF__ToMtxAffine(m, &n->mtx[gEvenOdd]);
        }
    }
}

/* Places every node of the swoosh on its owner and sums the distances between consecutive nodes. */
void Trail_PlaceNodes(Swoosh *e) {
    CVector3 d;
    SwooshNode *n;
    CParticle *o;
    SwooshNode *prev;
    f32 len;
    s32 moved;

    n = e->nodes.head;
    moved = 0;
    e->length = 0.0f;
    if (n != NULL) {
        do {
            o = n->owner;
            if (o != NULL) {
                placeNode(e, n, o);
                moved = 1;
                n->life = e->info->nodeLife;
            }
            prev = n->prev;
            if (prev != NULL) {
                CVector3__Subtract(&d, &n->pos, &prev->pos);
                len = func_00298470(d.x * d.x + d.y * d.y + d.z * d.z);
                prev->length = len;
                e->length += len;
            }
            n = n->next;
        } while (n != NULL);
    }
    if (moved) {
        e->time = e->info->unk0;
    }
}

#include "gfx.h"
#include "gbi_t2.h"
#include "protos/cache.h"

/* Draws a textured trail: spaces the texture along the tail by length, picks
 * the animation frame, sets the combiner and colours, then emits two vertex
 * rows per node and the triangle strip between them. */

#include "structs/texture.h"
extern void *Texture_Request(void *model);
extern void Texture_GetInfo(void *model, TextureInfo *info);
extern void Render_SetCombineMode(s32 mode);
extern void Render_SetRenderMode(s32 mode);
s32 Trail_GetAnimFrame(Swoosh *anim, SwooshInfo *def, s32 *count);
extern void Texture_Load(void *slot, void *model, s32 frame, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
sptr RibbonVtxPool_Alloc(void *arg0, s32 arg1);

void Trail_DrawTextured(void *pool, Swoosh *trail, s32 unused) {
    TextureInfo info;
    Morph storage;
    TextureInfo *anim;
    void *model;
    SwooshInfo *track;
    SwooshNode *clock, *next, *node;
    Gfx *vertices;
    f32 offset;
    f32 period;
    f32 scale;
    s32 count, edges, i, frame;

    track = trail->info;
    if (track->texMode == 2 && 0.0f < trail->length) {
        offset = 0.0f;
        for (node = trail->nodes.head; node != 0; node = node->next) {
            node->fraction = offset / trail->length;
            offset += node->length;
        }
    }
    model = Texture_Request(trail->texture);
    if (model == 0) return;
    Texture_GetInfo(model, &info);
    anim = &info;
    storage.items = trail->info->blends;
    storage.count = trail->info->count;
    if (trail->flags & 2) {
        storage.pointsA = trail->tex0;
        storage.pointsB = trail->tex1;
    } else {
        storage.pointsA = trail->tex0;
        storage.pointsB = trail->tex0;
    }
    storage.scaleA = trail->info->scaleS * 3.125f;
    storage.scaleB = trail->info->scaleT * 3.125f;
    if (anim != 0) {
        storage.texScaleS = 1 << (anim->frame->shiftS + 5);
        storage.texScaleS = storage.texScaleS * trail->info->textureScale - 32.0f;
        storage.texScaleT = 1 << (anim->frame->shiftT + 5);
        storage.texStepT = 1.0f / (storage.count - 1.0f);
    }
    period = 1.0f;
    while (trail->unk28 >= period) trail->unk28 -= period;
    GFX_CMD(0xE7000000, 0)
    switch (info.frame->format) {
    case 3:
        Render_SetCombineMode(0x29);
        GFX_CMD(0xFB000000, (track->env[0] << 24) | (track->env[1] << 16) | (track->env[2] << 8) | 0xFF)
        GFX_CMD(0xFA000000, (track->prim[0] << 24) | (track->prim[1] << 16) | (track->prim[2] << 8) | 0xFF)
        break;
    case 0:
    case 1:
        Render_SetCombineMode(0x28);
        break;
    }
    Render_SetRenderMode(0xD);
    clock = trail->nodes.tail;
    count = track->count;
    next = clock->prev;
    frame = Trail_GetAnimFrame(trail, track, &info);
    Texture_Load(trail->texture, model, frame, 0, 0xFFFF, 0xFFFF, 0, 0, 0);
    edges = count - 1;
    GFX_CMD(0xDA380003, GBI_ADDR(&clock->mtx[gEvenOdd]))
    while (clock != 0 && next != 0 && 0.0f < clock->life && 0.0f < next->life) {
        vertices = (Gfx *) RibbonVtxPool_Alloc(pool, count);
        if (vertices == 0) break;
        Morph_BlendVerticesTex(trail, clock, vertices, clock->fraction, &storage);
        GFX_CMD(0x01000000 | (count << 12) | ((count & 0x7F) << 1), GBI_ADDR(vertices))
        GFX_CMD(0xDA380003, GBI_ADDR(&next->mtx[gEvenOdd]))
        vertices = (Gfx *) RibbonVtxPool_Alloc(pool, count);
        if (vertices == 0) break;
        scale = next->fraction;
        while (clock->fraction < scale) scale -= period;
        Morph_BlendVerticesTex(trail, next, vertices, scale, &storage);
        GFX_CMD(0x01000000 | (count << 12) | (((count * 2) & 0x7F) << 1), GBI_ADDR(vertices))
        for (i = 0; i < edges; i++) {
            GFX_CMD((((i * 2 & 255) << 16) | (((count + i) * 2 & 255) << 8) | ((count + i + 1) * 2 & 255)) | (6 << 24), ((i * 2 & 255) << 16) | (((count + i + 1) * 2 & 255) << 8) | ((i + 1) * 2 & 255))
        }
        if (trail->info->closed) {
            GFX_CMD((((i * 2 & 255) << 16) | (((count + i) * 2 & 255) << 8) | (count * 2 & 255)) | (6 << 24), ((i * 2 & 255) << 16) | ((count * 2 & 255) << 8))
        }
        clock = next;
        next = clock->prev;
    }
    CCache__Unlock(0, model);
}

/* An untextured swoosh: its points from the tail, each pair of them a band of the record's
 * cross-section (Morph_BlendVertices) drawn as triangles. */
void Trail_DrawUntextured(CSwooshSystem *pool, Swoosh *actor, s32 unused) {
    Morph storage;
    Morph *morph = &storage;
    SwooshNode *clock, *next;
    Vtx_t *vertices;
    u32 count, i;
    s32 edges;
    morph->items = actor->info->blends;
    morph->count = actor->info->count;
    if (actor->flags & 2) { morph->pointsA = actor->tex0; morph->pointsB = actor->tex1; }
    else { morph->pointsA = actor->tex0; morph->pointsB = actor->tex0; }
    morph->scaleA = actor->info->scaleS * 3.125f;
    morph->scaleB = actor->info->scaleT * 3.125f;
    clock = actor->nodes.tail;
    next = clock->prev;
    count = actor->info->count;
    edges = count - 1;
    { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xDA) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (5)))) << (19)) | (((unsigned int)(((((unsigned int)(0x1)) << 1)) ^ (((unsigned int)(0x1)) << 0)) & (0xFFFFFFFFU >> (32 - (8)))) << (0))); _gbi->w1 = (GBI_ADDR(&clock->mtx[gEvenOdd])); };
    while (clock != 0 && next != 0 && 0.0f < clock->life && 0.0f < next->life) {
        vertices = (Vtx_t *) RibbonVtxPool_Alloc(pool, count);
        if (vertices == 0) break;
        Morph_BlendVertices(actor, clock, vertices, &storage);
        { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0x01) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(count) & (0xFFFFFFFFU >> (32 - (8)))) << (12)) | (((unsigned int)((0) + (count)) & (0xFFFFFFFFU >> (32 - (7)))) << (1))); _gbi->w1 = (GBI_ADDR(vertices)); };
        { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xDA) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (5)))) << (19)) | (((unsigned int)(((((unsigned int)(0x1)) << 1)) ^ (((unsigned int)(0x1)) << 0)) & (0xFFFFFFFFU >> (32 - (8)))) << (0))); _gbi->w1 = (GBI_ADDR(&next->mtx[gEvenOdd])); };
        vertices = (Vtx_t *) RibbonVtxPool_Alloc(pool, count);
        if (vertices == 0) return;
        Morph_BlendVertices(actor, next, vertices, &storage);
        { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0x01) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(count) & (0xFFFFFFFFU >> (32 - (8)))) << (12)) | (((unsigned int)((count) + (count)) & (0xFFFFFFFFU >> (32 - (7)))) << (1))); _gbi->w1 = (GBI_ADDR(vertices)); };
        i = 0;
        while ((s32)i < edges) {
            { Gfx *cmd = gDList++;
              cmd->w0 = (((i * 2 & 255) << 16) | (((count + i) * 2 & 255) << 8) | ((count + i + 1) * 2 & 255)) | (0x06 << 24);
              cmd->w1 = ((i * 2 & 255) << 16) | (((count + i + 1) * 2 & 255) << 8) | ((i + 1) * 2 & 255);
            }
            i++;
        }
        if (actor->info->closed) {
            { Gfx *cmd = gDList++;
              cmd->w0 = (((i * 2 & 255) << 16) | (((count + i) * 2 & 255) << 8) | (count * 2 & 255)) | (0x06 << 24);
              cmd->w1 = ((i * 2 & 255) << 16) | ((count * 2 & 255) << 8);
            }
        }
        clock = next;
        next = clock->prev;
    }
}

s32 Random_Next(void);                /* random */

/* The frame index of an animation of `count` frames, by the definition's playback mode:
 * clamp (and remember the end), clamp, loop, ping-pong, random every time, random once. */
s32 Trail_GetAnimFrame(Swoosh *anim, SwooshInfo *def, s32 *count) {
    s32 frame;
    s32 *f;
    s32 period;
    s32 v;

    frame = anim->frame;
    f = &frame;
    switch (def->animMode) {
    case 0:
        if (*f >= *count - 1) {
            *f = *count - 1;
            anim->endFrame = *count;
        }
        break;
    case 1:
        if (*f >= *count) {
            *f = *count - 1;
        }
        break;
    case 2:
        *f = *f % *count;
        break;
    case 3:
        period = *count * 2 - 2;
        v = period > 0 ? *f % period : 0;
        *f = v;
        if (v >= *count) {
            *f = *count * 2 - 2 - v;
        }
        break;
    case 4:
        *f = Random_Next() % *count;
        break;
    case 5:
        if (anim->randomFrame == -1) {
            anim->randomFrame = Random_Next() % *count;
        }
        *f = anim->randomFrame;
        break;
    }
    return frame;
}

#include "globals/swoosh.h"

typedef struct R8Mtx { s32 w[16]; } R8Mtx;
typedef struct R8Info {
               u8 pad0[4];
               f32 unk4;
               f32 unk8;
} R8Info;
typedef struct R8Part R8Part;
/* structs/swoosh.h SwooshNode, Swoosh: the same layouts. */
struct R8Part {
               R8Part *prev;
               R8Part *next;
               f32 unk8;
               u8 padC[4];
               u8 unk10[0x18];
               R8Mtx mtx[2];
               u8 padA8[8];
               sptr owner;
};
typedef struct R8Emit R8Emit;
struct R8Emit {
               R8Emit *prev;
               R8Emit *next;
               R8Info *info;
               void *tex0;
               void *tex1;
               s32 texture[2];
               void *owner;     /* flags & 1: a CGameObjectInstance, & 2: a CParticle */
               s32 id;
               f32 unk24;
               f32 unk28;
               f32 unk2C;
               f32 unk30;
               s32 layer;
               s32 unk38;
               u32 flags;
               R8Part *parts;
};
/* The particle a swoosh follows: m_mtxDraw at 0x48, m_pEffect at 0xF8 (Rage Wars 0x60 / 0x118). */
typedef CParticle R8Actor;
typedef CSwooshSystem R8Mgr;

void Trail_DetachOwner(R8Mgr *mgr, R8Actor *who) {
    u8 sp10[0x40];
    R8Emit *e;
    R8Part *p;
    f32 k;
    s32 i;
    u8 *dst, *src;
    e = mgr->m_SwooshPool.used.head;
    if (e == ((void *)0)) {
        return;
    }
    do {
        if (e->flags & 8) {
            p = e->parts;
            if (p != ((void *)0)) {
                do {
                    if (p->owner == (sptr)who) {
                        if (who != ((void *)0)) {
                            if ((sptr)who == -1) {
                                p->mtx[gEvenOdd] = p->mtx[gEvenOdd ^ 1];
                                p->owner = 0;
                            } else {
                                CMtxF__FromMtxAffine(sp10, who->m_mtxDraw[gEvenOdd]);
                                k = 0.01f;
                                if (who->m_pEffect->m_pModel != 0) {
                                    k = 0.29257143f;
                                }
                                CMtxF__ScaleAxes(sp10, k, k, k);
                                CMtxF__ClampEntries(sp10);
                                CMtxF__GetTranslation(sp10, p->unk10);
                                if (e->flags & 4) {
                                    CMtxF__GetScale(sp10, p->unk10 + 0xC);
                                } else {
                                    CMtxF__ToMtxAffine(sp10, &p->mtx[gEvenOdd]);
                                }
                            }
                            p->unk8 = e->info->unk8;
                        }
                        p->owner = -1;
                    }
                    p = p->next;
                } while (p != ((void *)0));
            }
        } else if ((R8Actor *)e->owner == who) {
            if (e->flags & 2) {
#ifdef TARGET_PORT
              u8 *wm = who->m_mtxDraw[gEvenOdd];
              if (e->unk24 > 0.0f) {
#else
              register s32 t asm("$2") = gEvenOdd;
              register u8 *wm asm("$5") = (u8 *)((t << 6) + (sptr)who);
              __asm__("" : "+r"(wm));
              if (e->unk24 > 0.0f) {
                wm += 0x48;
#endif
                CMtxF__FromMtxAffine(sp10, wm);
                k = 0.01f;
                if (who->m_pEffect->m_pModel != 0) {
                    k = 0.29257143f;
                }
                CMtxF__ScaleAxes(sp10, k, k, k);
                CMtxF__ClampEntries(sp10);
                Swoosh_NewNode(&gEngineApp_m_SwooshSystem, e, sp10);
              }
            }
            if (e->owner != ((void *)0)) {
                if (e->flags & 1) {
                    if (((CGameObjectInstance *)e->owner)->m_ActiveSwooshes != 0) {
                        ((CGameObjectInstance *)e->owner)->m_ActiveSwooshes = ((CGameObjectInstance *)e->owner)->m_ActiveSwooshes - 1;
                    }
                }
                if (e->flags & 2) {
                    if (((CParticle *)e->owner)->m_ActiveSwooshes != 0) {
                        ((CParticle *)e->owner)->m_ActiveSwooshes = ((CParticle *)e->owner)->m_ActiveSwooshes - 1;
                    }
                }
            }
            e->owner = ((void *)0);
            if (e->unk24 < e->info->unk4) {
                e->unk24 = e->info->unk4;
            }
        }
        e = e->next;
    } while (e != ((void *)0));
}

#include "globals/scene.h"
#include "globals/particle.h"

/* The particle a trail follows (CParticle): its effect's flags (bit 31: ..., 0x10000: a trail
 * keyed by the particle's owner and type), its type, owner and trail count. */
typedef CParticle TrailActor;
typedef Swoosh Trail;
typedef SwooshNode TrailNode;
typedef CSwooshSystem TrailMgr;
void DList_Init(void *p);
void *NodePool_Alloc(void *pool);
void CParticle__AddDrawModeBits(void *particle, void *texture);
SwooshInfo *CScene__GetSwoosh(void *tbl, s32 arg);
#ifdef TARGET_PORT
/* Round 20 i3: gendata types a data symbol by the declarations of the C that uses it. The trail
 * point records (Morph points: x, y, z and two more f32, 0x14 bytes each) are reached only
 * through sTrailTextures, so they came out as N64 bytes: every point read as a denormal, about 0,
 * and every textured trail was drawn with no width. This reference makes them host f32s. */
f32 *const gTrailPoints_port[4] = { sTrailTexture0Points, sTrailTexture1Points, sTrailTexture2Points, sTrailTexture3Points };
#endif

void Material_GetTextureSet(void *texture, s32 id);

/* Adds a node to the actor's trail, starting the trail first when the actor has none. */
void Trail_AddNode(TrailMgr *mgr, TrailActor *a, s32 type) {
    u8 m[0x40];
    Trail *s;
    register Trail *q asm("$5");
    s32 qid;
    f32 start;
    SwooshInfo *t;
    TrailNode *node;
    void *tex0;
    void *tex1;
    void *key;
    s32 id;

    if (a->m_pEffect->m_dwFlags & 0x10000) {
        key = a->m_pOwner;
        id = a->header.m_nObjType;
        s = NULL;
        q = mgr->m_SwooshPool.used.head;
        while (q != NULL) {
            if (q->owner == key) {
                qid = q->id;
                s = q;
                if (qid == id) {
                    break;
                }
            }
            q = q->next;
            s = NULL;
        }
    } else {
        s = NULL;
    }
    if (s == NULL) {
        s = NodePool_Alloc(&mgr->m_SwooshPool);
        if (s == NULL) {
            return;
        }
        t = CScene__GetSwoosh(&gScene, type);
        tex0 = sTrailTextures[t->tex0];
        tex1 = sTrailTextures[t->tex1];
        DList_Init(&s->nodes);
        s->flags = 2;
        if ((s32) a->m_pEffect->m_dwFlags < 0) {
            s->flags = 6;
        }
        if (a->m_pEffect->m_dwFlags & 0x10000) {
            s->flags |= 8;
            s->owner = a->m_pOwner;
            s->id = a->header.m_nObjType;
        } else {
            s->owner = a;
            s->id = -1;
        }
        s->info = t;
        s->tex0 = tex0;
        s->tex1 = tex1;
        start = t->unk0;
        *(s32 *) &s->unk28 = 0;
        s->time = start;
        s->layer = t->sound;
        *(s32 *) &s->length = 0;
        s->unk50 = 0;
        *(s32 *) &s->endFrame = 0;
        *(s32 *) &s->frame = 0;
        s->randomFrame = -1;
        if (s->layer != -1) {
            Material_GetTextureSet(s->texture, s->layer);
            if (a != NULL) {
                CParticle__AddDrawModeBits(a, s->texture);
            }
        }
        if (s == NULL) {
            return;
        }
    }
    a->m_ActiveSwooshes++;
    if (a->m_pEffect->m_dwFlags & 0x10000) {
        CMtxF__Identity(m);
        node = Swoosh_NewNode(mgr, s, m);
        if (node != NULL) {
            node->owner = a;
        }
    }
}

void Render_InvalidateModeCache(void);
void Render_SetRenderMode(s32);
void Render_SetCombineMode(s32);
void Texture_InvalidateCache(void);

/* Draws the trails: the negative layers first, then the positive ones. */
void Trail_DrawAll(TrailMgr *mgr, sptr arg) {
    Trail *e;

    GFX_CMD(0xE7000000, 0)
    Render_InvalidateModeCache();
    Render_SetRenderMode(0xE);
    Render_SetCombineMode(0x1C);
    GFX_CMD(0xD9F8FB7F, 0)
    GFX_CMD(0xD9FFFFFF, 0x200004)
    GFX_CMD(0xE3000A01, 0)
    GFX_CMD(0xE3000C00, 0x80000)
    GFX_CMD(0xD7000000, 0)
    Texture_InvalidateCache();
    for (e = mgr->m_SwooshPool.used.head; e != NULL; e = e->next) {
        if (e->layer < 0) {
            if (e->flags & 8) {
                Trail_PlaceNodes(e);
            }
            if (e->flags & 4) {
                Trail_ComputeFrames(e, arg);
            }
            if (e->nodes.count >= 2) {
                if (e->layer >= 0) {
                    Trail_DrawTextured(mgr, e, arg);
                } else {
                    Trail_DrawUntextured(mgr, e, arg);
                }
            }
        }
    }
    for (e = mgr->m_SwooshPool.used.head; e != NULL; e = e->next) {
        if (e->layer > 0) {
            if (e->flags & 8) {
                Trail_PlaceNodes(e);
            }
            if (e->flags & 4) {
                Trail_ComputeFrames(e, arg);
            }
            if (e->nodes.count >= 2) {
                if (e->layer >= 0) {
                    Trail_DrawTextured(mgr, e, arg);
                } else {
                    Trail_DrawUntextured(mgr, e, arg);
                }
            }
        }
    }
}

#include "structs/boss.h"

extern f32 Random_Blend(f32 arg0, f32 arg1);

/* Steps the animator: the current op (wait, fade the value, fade the colour), then the next ops
 * of its script until one takes time. */
void Animator_Update(ScriptAnimator *a) {
    s32 next;
    f32 op0;
    f32 op1;
    f32 op2;
    f32 op3;
    f32 op4;
    f32 op5;
    next = 0;
    switch (a->mode) {
    case 0:
        if (a->loop != 0) {
            next = 1;
        }
        break;
    case 1:
        a->time -= gFrameIncrement;
        if (a->time < 0.0f) {
            next = 1;
        }
        break;
    case 4:
        a->time += gFrameIncrement;
        if (a->valueDuration <= a->time) {
            a->time = a->valueDuration;
            next = 1;
        }
        op1 = a->time / a->valueDuration;
        op0 = 1.0f - op1;
        a->value = op0 * a->valueFrom + op1 * a->valueTo;
        break;
    case 6:
        a->time += gFrameIncrement;
        if (a->colourDuration <= a->time) {
            a->time = a->colourDuration;
            next = 1;
        }
        op1 = a->time / a->colourDuration;
        op0 = 1.0f - op1;
        a->rgb[0] = op0 * a->rgbFrom[0] + op1 * a->rgbTo[0];
        a->rgb[1] = op0 * a->rgbFrom[1] + op1 * a->rgbTo[1];
        a->rgb[2] = op0 * a->rgbFrom[2] + op1 * a->rgbTo[2];
        a->alpha = op0 * a->alphaFrom + op1 * a->alphaTo;
        break;
    }
    while (next) {
        a->mode = *a->script++;
        next = 0;
        switch (a->mode) {
        case 0:
            a->script = 0;
            a->loop = 0;
            break;
        case 1:
            op5 = *a->script++;
            a->time = op5;
            break;
        case 2:
            op0 = *a->script++;
            op1 = *a->script++;
            a->mode = 1;
            a->time = Random_Blend(op0, op1);
            break;
        case 3:
            next = 1;
            op4 = *a->script++;
            a->value = op4;
            break;
        case 4:
            op4 = *a->script++;
            op5 = *a->script++;
            a->valueFrom = a->value;
            a->time = 0.0f;
            a->valueTo = op4;
            a->valueDuration = op5;
            break;
        case 5:
            next = 1;
            op2 = *a->script++;
            op3 = *a->script++;
            op1 = *a->script++;
            op4 = *a->script++;
            a->rgb[0] = op2;
            a->rgb[1] = op3;
            a->rgb[2] = op1;
            a->alpha = op4;
            break;
        case 6:
            op2 = *a->script++;
            op3 = *a->script++;
            op1 = *a->script++;
            op4 = *a->script++;
            op5 = *a->script++;
            a->rgbFrom[0] = a->rgb[0];
            a->rgbFrom[1] = a->rgb[1];
            a->rgbFrom[2] = a->rgb[2];
            a->alphaFrom = a->alpha;
            a->time = 0.0f;
            a->rgbTo[0] = op2;
            a->rgbTo[1] = op3;
            a->rgbTo[2] = op1;
            a->alphaTo = op4;
            a->colourDuration = op5;
            break;
        case 7:
            next = 1;
            a->script++;
            break;
        case 8:
            next = 1;
            a->script = a->loop;
            break;
        }
    }
}

#include "structs/light.h"

static __inline__ void clamp255(f32 *v) {
    if (*v < 0.0f) {
        *v = 0.0f;
    } else if (*v > 255.0f) {
        *v = 255.0f;
    }
}

void ScreenTint_UpdateMode(void) {
    f32 alpha;
    f32 fade;

    clamp255(&gBlendState_m_Color[0]);
    clamp255(&gBlendState_m_Color[1]);
    clamp255(&gBlendState_m_Color[2]);
    clamp255(&gBlendState_m_Color[3]);
    clamp255(&gBlendState_m_Color[4]);

    fade = gBlendState_m_Color[3];
    alpha = gBlendState_m_Color[4];
    if (alpha == 255.0f && fade == 0.0f) {
        gBlendState = 0;
    } else if (alpha != 255.0f && fade == 0.0f) {
        gBlendState = 1;
        gBlendState_m_Color[3] = gBlendState_m_Color[4];
    } else if (alpha == 255.0f && fade != 0.0f) {
        gBlendState = 3;
    } else if (alpha != 255.0f && fade != 0.0f) {
        gBlendState = 6;
    }
}

#include "globals/math.h"

extern f32 Math_AcosLookup(f32 arg0);
extern float func_002974C0(float x);
extern float func_002982F0(float x);
CQuaternion *Quat_AlignYTo(CQuaternion *out, u32 bx, u32 by, u32 bz);

/* The rotation that turns +y onto the unit vector (bx, by, bz), passed as three words (the floats' bits). */
CQuaternion *Quat_AlignYTo(CQuaternion *out, u32 bx, u32 by, u32 bz) {
    CVector3 axis;
    CVector3 cross;
    CQuaternion result;
    f32 one;
    f32 magnitude;
    f32 angle;
    f32 trig;
    one = 1.0f;
    axis.x = 0.0f;
    axis.y = one;
    axis.z = 0.0f;
    CVector3__Cross(&cross, &axis, (CVector3 *)&bx);
    magnitude = func_00298470((cross.x * cross.x) +
                              (cross.y * cross.y) +
                              (cross.z * cross.z));
    if (magnitude == 0.0f) {
        result.z = 0.0f;
        result.y = 0.0f;
        result.x = 0.0f;
        result.w = one;
    } else {
        CVector3__Scale(&cross, &cross, one / magnitude);
        angle = Math_AcosLookup((axis.x * *(f32 *)&bx) +
                              (axis.y * *(f32 *)&by) +
                              (axis.z * *(f32 *)&bz));
        angle *= 0.5f;
        trig = func_002982F0(angle);
        result.x = cross.x * trig;
        result.y = cross.y * trig;
        result.z = cross.z * trig;
        gQuaternionSin = trig;
        result.w = func_002974C0(angle);
    }
    *out = result;
    return out;
}

#include "globals/render.h"
#include "protos/list.h"

/* A decal's matrix: the integer part of its translation at 0x18 (structs/swoosh.h SwooshMtx). */
typedef struct VInfo {
    u8 pad0[0x18];
    s16 x;
    s16 y;
    s16 z;
} VInfo;
typedef struct Vec3fv { f32 x, y, z; } Vec3fv;
extern f32 gCameraVisibleBox[6]; /* local type: the CBox read as six floats by index */

/* Draws the ground decals inside the camera's box; frees those outside it. */
void SwooshSystem_DrawDecals(CSwooshSystem *c) {
    s32 i;
    CList *l;
    SwooshDecal *n, *next;
    Vec3fv v;
    VInfo *p;
    if (c->m_FreeDecals.m_nCount == 100) {
        return;
    }
    { Gfx *_g = gDList++; _g->w0 = (0xE7000000); _g->w1 = (0); }
    { Gfx *_g = gDList++; _g->w0 = (0xE3000A01); _g->w1 = (0x100000); }
    { Gfx *_g = gDList++; _g->w0 = (0xFD900000); _g->w1 = (GBI_ADDR(gShadowTexture)); }
    { Gfx *_g = gDList++; _g->w0 = (0xF5900000); _g->w1 = (0x07080200); }
    { Gfx *_g = gDList++; _g->w0 = (0xE6000000); _g->w1 = (0); }
    { Gfx *_g = gDList++; _g->w0 = (0xF3000000); _g->w1 = (0x0707F400); }
    { Gfx *_g = gDList++; _g->w0 = (0xE7000000); _g->w1 = (0); }
    { Gfx *_g = gDList++; _g->w0 = (0xF5880400); _g->w1 = (0x00080200); }
    { Gfx *_g = gDList++; _g->w0 = (0xF2000000); _g->w1 = (0x0003C03C); }
    { Gfx *_g = gDList++; _g->w0 = (0xFA000000); _g->w1 = (-0x38); }
    { Gfx *_g = gDList++; _g->w0 = (0xDE000000); _g->w1 = (GBI_ADDR(sDecalSetupDList)); }
    for (i = 0; i < 10; i++) {
        l = &c->m_DecalLists[i];
        for (n = l->m_pHead; n != ((void *)0); n = next) {
            next = n->next;
            p = (VInfo *) &n->mtx;
#ifdef TARGET_PORT
            /* the matrix holds host words (hi << 16 | lo), not big-endian halves */
            v.x = (s16) (n->mtx.w[6] >> 16);
            v.y = (s16) n->mtx.w[6];
            v.z = (s16) (n->mtx.w[7] >> 16);
#else
            v.x = p->x;
            v.y = p->y;
            v.z = p->z;
#endif
            if (!(v.x < gCameraVisibleBox[3] && v.y < gCameraVisibleBox[4] && v.z < gCameraVisibleBox[5] && gCameraVisibleBox[0] <= v.x && gCameraVisibleBox[1] <= v.y && gCameraVisibleBox[2] <= v.z)) {
                CList__Remove(l, n);
                CList__AddTail(&c->m_FreeDecals, n);
            } else {
                { Gfx *_g = gDList++; _g->w0 = (0xDA380003); _g->w1 = (GBI_ADDR(p)); }
                { Gfx *_g = gDList++; _g->w0 = (0x01004008); _g->w1 = (GBI_ADDR(gUnitQuadVtx)); }
                { Gfx *_g = gDList++; _g->w0 = (0x06000204); _g->w1 = (0x00040600); }
            }
        }
    }
    Render_InvalidateModeCache();
    Texture_InvalidateCache();
}

#include "globals/game.h"

void SwooshSystem_UpdateSwooshes(CSwooshSystem *arg0);
void SwooshSystem_UpdateTimers(CSwooshSystem *arg0);

void SwooshSystem_Update(CSwooshSystem *arg0) {
    sSwooshSpawned = 0;
    if (gEngineApp_m_Settings.paused == 0) {
        SwooshSystem_UpdateSwooshes(arg0);  /* the N64 code leaves arg0 in $a0 for it */
        SwooshSystem_UpdateTimers(arg0);
    }
}

/* count vertices from this frame's ribbon buffer (CSwooshSystem.m_RibbonVtx), or 0 when fewer are left. */
sptr RibbonVtxPool_Alloc(void *arg0, s32 arg1) {
    CSwooshSystem *pool = arg0;
    s32 avail;
    sptr head;
    avail = pool->m_RibbonVtxLeft;
    if (avail < arg1) {
        return 0;
    }
    head = (sptr)pool->m_pRibbonVtx;
    pool->m_RibbonVtxLeft = avail - arg1;
    pool->m_pRibbonVtx += arg1;
    return head;
}

/* The ribbon vertex pool: 300 vertices for this frame's buffer. */
void RibbonVtxPool_Reset(CSwooshSystem *arg0) {
    s32 i = gEvenOdd;
    arg0->m_RibbonVtxLeft = 0x12C;
    arg0->m_pRibbonVtx = arg0->m_RibbonVtx[i];
}

s32 DList_AddTail(void *list, void *node);
void DList_Remove(void *list, void *node);

/* Moves a free swoosh point to the end of swoosh arg1's points. */
SwooshNode *Swoosh_TakeNode(CSwooshSystem *arg0, Swoosh *arg1) {
    SwooshNode *temp_s0;

    temp_s0 = arg0->m_FreeNodes.head;
    if (temp_s0 != NULL) {
        DList_Remove(&arg0->m_FreeNodes, temp_s0);
        DList_AddTail(&arg1->nodes, temp_s0);
    }
    return temp_s0;
}

/* Takes point arg2 out of swoosh arg1 (and its length out of the swoosh's), back to the free points. */
void SwooshGroup_Remove(CSwooshSystem *arg0, Swoosh *arg1, SwooshNode *arg2) {
    arg1->length = (f32) (arg1->length - arg2->length);
    DList_Remove(&arg1->nodes, arg2);
    DList_AddTail(&arg0->m_FreeNodes, arg2);
}

#include "m2c_macros.h"

void func_00236098(void) {

}

s32 CMtxF__Copy(s32 *, sptr);              /* extern */

/* Adds a point to every live swoosh of object arg1, at its record bone of matrices arg2. */
void SwooshSystem_AddBonePoints(CSwooshSystem *arg0, sptr arg1, sptr arg2) {
    u8 sp10[0x40];
    Swoosh *var_s0;

    var_s0 = arg0->m_SwooshPool.used.head;
    if (var_s0 != NULL) {
        do {
            if (((sptr) var_s0->owner == arg1) && (var_s0->time > 0.0f)) {
                CMtxF__Copy(sp10, arg2 + (var_s0->info->bone << 6));
                CMtxF__ScaleAxes(sp10, 0.00000015258789f, 0.00000015258789f, 0.00000015258789f);
                CMtxF__ClampEntries(sp10);
                Swoosh_NewNode((sptr) arg0, (sptr) var_s0, (sptr) sp10);
            }
            var_s0 = var_s0->next;
        } while (var_s0 != NULL);
    }
}

void Swoosh_AddParticleNode(CSwooshSystem *arg0, CParticle *arg1, sptr arg2) {
    u8 sp10[0x40];
    f32 temp_f0;
    f32 var_f0;
    Swoosh *var_s0;

    var_s0 = arg0->m_SwooshPool.used.head;
    if (var_s0 != NULL) {
        do {
            if (var_s0->owner == arg1) {
                temp_f0 = var_s0->time;
                if (temp_f0 > 0.0f) {
                    if (temp_f0 > 0.0f) {
                        CMtxF__FromMtxAffine(sp10, arg2);
                        var_f0 = 0.01f;
                        if (arg1->m_pEffect->m_pModel != 0) {
                            var_f0 = 0.29257143f;
                        }
                        CMtxF__ScaleAxes(sp10, var_f0, var_f0, var_f0);
                        CMtxF__ClampEntries(sp10);
                        Swoosh_NewNode((sptr) arg0, (sptr) var_s0, (sptr) sp10);
                    }
                }
            }
            var_s0 = var_s0->next;
        } while (var_s0 != NULL);
    }
}

/* Detaches every swoosh of owner arg1 (flags & 1: counted in a game object's m_ActiveSwooshes,
 * & 2: in a particle's), letting each fade out. */
void Swoosh_DetachOwner(CSwooshSystem *arg0, void *arg1) {
    f32 temp_f1;
    u8 temp_v0;
    u8 temp_v0_2;
    void *temp_a0;
    void *temp_a0_2;
    Swoosh *var_v1;

    var_v1 = arg0->m_SwooshPool.used.head;
    if (var_v1 != NULL) {
        do {
            temp_a0 = var_v1->owner;
            if (temp_a0 == arg1) {
                if (temp_a0 != NULL) {
                    if (var_v1->flags & 1) {
                        temp_v0 = ((CGameObjectInstance *) temp_a0)->m_ActiveSwooshes;
                        if (temp_v0 != 0) {
                            ((CGameObjectInstance *) temp_a0)->m_ActiveSwooshes = (u8) (temp_v0 - 1);
                        }
                    }
                    if (var_v1->flags & 2) {
                        temp_a0_2 = var_v1->owner;
                        temp_v0_2 = ((CParticle *) temp_a0_2)->m_ActiveSwooshes;
                        if (temp_v0_2 != 0) {
                            ((CParticle *) temp_a0_2)->m_ActiveSwooshes = (u8) (temp_v0_2 - 1);
                        }
                    }
                }
                var_v1->owner = NULL;
                temp_f1 = var_v1->info->fadeTime;
                if (var_v1->time < temp_f1) {
                    var_v1->time = temp_f1;
                }
            }
            var_v1 = var_v1->next;
        } while (var_v1 != NULL);
    }
}

void ScriptAnimator_Start(ScriptAnimator *arg0, void *owner) {
    arg0->owner = owner;
    arg0->mode = 0;
    arg0->loop = 0;
    arg0->script = 0;
    arg0->alpha = 0;
    arg0->rgb[0] = 0;
    arg0->rgb[1] = 0;
    arg0->rgb[2] = 0;
    arg0->value = 255.0f;
}

void ScriptAnimator_SetScript(ScriptAnimator *arg0, f32 *script) {
    arg0->mode = 0;
    arg0->loop = script;
    arg0->script = script;
}

void ScriptAnimator_Apply(ScriptAnimator *arg0) {
    CVector3 *temp_v0;
    CVector3 *temp_v0_2;

    if (arg0->alpha != 0.0f) {
        temp_v0 = (CVector3 *) gBlendState_m_Color;
        gBlendState_m_Color[0] = (f32) arg0->rgb[0];
        temp_v0->y = (f32) arg0->rgb[1];
        temp_v0->z = (f32) arg0->rgb[2];
    } else {
        temp_v0_2 = (CVector3 *) gBlendState_m_Color;
        gBlendState_m_Color[0] = 0.0f;
        temp_v0_2->y = 0.0f;
        temp_v0_2->z = 0.0f;
    }
    gBlendState_m_Color[3] = (f32) arg0->alpha;
    gBlendState_m_Color[4] = (f32) arg0->value;
}

void ScriptAnimator_AddToBlend(ScriptAnimator *arg0) {
    f32 *temp_v0;

    if (arg0->alpha != 0.0f) {
        temp_v0 = &gBlendState_m_Color[0]; /* [3] runs into gBlendState_m_Color[3] */
        gBlendState_m_Color[0] = (f32) (gBlendState_m_Color[0] + arg0->rgb[0]);
        temp_v0[1] = (f32) (temp_v0[1] + arg0->rgb[1]);
        temp_v0[2] = (f32) (temp_v0[2] + arg0->rgb[2]);
        temp_v0[3] = (f32) (temp_v0[3] + arg0->alpha);
    }
    gBlendState_m_Color[4] = (f32) (gBlendState_m_Color[4] + arg0->value);
}

/* De-faked in round 21 (j4): the draft loaded the value in two identical arms under an uninitialised
 * test; the plain clamp matches. */

/* Clamps *value to 0..255. */
void Color_ClampByte(f32 *value) {
    f32 v = *value;

    if (v < 0.0f) {
        *value = 0.0f;
    } else if (v > 255.0f) {
        *value = 255.0f;
    }
}

/* Starts a timer: callback arg1 (with arg2) every arg4, arg3 times; the first at once. */
void SwooshSystem_AddTimer(CSwooshSystem *arg0, s32 (*arg1)(SwooshTimer *), sptr arg2, s32 arg3, f32 arg4) {
    SwooshTimer *temp_v0;

    temp_v0 = NodePool_Alloc(&arg0->m_TimerPool);
    if (temp_v0 != 0) {
        temp_v0->callback = arg1;
        temp_v0->arg = arg2;
        *(s32 *) &temp_v0->time = 0;
        temp_v0->period = arg4;
        temp_v0->repeats = arg3;
    }
}

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002A6BD4.c, func_002A6BD4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

void CQuaternion__FromDirection(void *a, CVector3 *b);
void CQuaternion__ToMatrix(void *a, void *b);
void CMtxF__PreRotateX(void *a, f32 f);
void CVector3__Add(CVector3 *a, CVector3 *b, CVector3 *c);
void CMtxF__Translate(void *m, f32 x, f32 y, f32 z);

/* Places a ground decal of kind idx at pos facing dir (a free one, else the oldest of its kind). */
SwooshDecal *SwooshSystem_PlaceDecal(CSwooshSystem *o, CVector3 pos, CVector3 dir, s32 idx, sptr val) {
    u8 m10[0x40];
    u8 m50[0x10];
    SwooshDecal *n;
    CList *list;
    CQuaternion__FromDirection(m50, &dir);
    if (sSwooshSpawned != 0) {
        return 0;
    }
    {
        sSwooshSpawned = 1;
        n = o->m_FreeDecals.m_pHead;
        list = &o->m_DecalLists[idx];
        if (n != 0) {
            CList__Remove(&o->m_FreeDecals, n);
            goto have;
        }
        n = list->m_pHead;
        if (n != 0) {
            CList__Remove(list, n);
have:
            CList__AddTail(list, n);
            if (n != 0) {
                n->owner = val;
                CQuaternion__ToMatrix(m50, m10);
                CMtxF__PreRotateX(m10, 1.5707965f);
                CVector3__Normalize(&dir);
                CVector3__Scale(&dir, &dir, 2.048f);
                CVector3__Add(&pos, &pos, &dir);
                CMtxF__Translate(m10, pos.x, pos.y, pos.z);
                CMtxF__ScaleAxes(m10, 6.144f, 1.0f, 6.144f);
                CMtxF__ToMtxAffine(m10, &n->mtx);
            }
        }
    }
    return n;
}

#include "structs/list.h"

/* Frees every ground decal (gEngineApp_m_SwooshSystem, not the particle system) that belongs to arg1. */
void SwooshSet_FreeOwner(CSwooshSystem *arg0, sptr arg1)
{
  s32 var_s4;
  CList *temp_s2;
  SwooshDecal *temp_s1;
  SwooshDecal *var_s0;
  if (arg0->m_FreeDecals.m_nCount != 0x64)
  {
    for (var_s4 = 0; var_s4 < 0xA; var_s4++)
    {
      temp_s2 = &arg0->m_DecalLists[var_s4];
      var_s0 = temp_s2->m_pHead;
      while (var_s0 != 0)
      {
        temp_s1 = var_s0->next;
        if (var_s0->owner == arg1)
        {
          CList__Remove(temp_s2, var_s0);
          CList__AddTail(&arg0->m_FreeDecals, var_s0);
        }
        var_s0 = temp_s1;
      }
    }
  }
}

s32 NodePool_Free(void *, void *);              /* extern */

/* Updates every live swoosh; frees those that ended (detached from their owner first:
 * flags & 1 counted in a game object's m_ActiveSwooshes, & 2 in a particle's). */
void SwooshSystem_UpdateSwooshes(CSwooshSystem *arg0) {
    f32 temp_f1;
    u8 temp_v0;
    u8 temp_v0_2;
    Swoosh *temp_s1;
    void *temp_v1;
    void *temp_v1_2;
    Swoosh *var_s0;

    var_s0 = arg0->m_SwooshPool.used.head;
    if (var_s0 != NULL) {
        do {
            temp_s1 = var_s0->next;
            if (Swoosh_Update(arg0, var_s0) == 0) {
                temp_v1 = var_s0->owner;
                if (temp_v1 != NULL) {
                    if (var_s0->flags & 1) {
                        temp_v0 = ((CGameObjectInstance *) temp_v1)->m_ActiveSwooshes;
                        if (temp_v0 != 0) {
                            ((CGameObjectInstance *) temp_v1)->m_ActiveSwooshes = temp_v0 - 1;
                        }
                    }
                    if (var_s0->flags & 2) {
                        temp_v1_2 = var_s0->owner;
                        temp_v0_2 = ((CParticle *) temp_v1_2)->m_ActiveSwooshes;
                        if (temp_v0_2 != 0) {
                            ((CParticle *) temp_v1_2)->m_ActiveSwooshes = temp_v0_2 - 1;
                        }
                    }
                }
                var_s0->owner = NULL;
                temp_f1 = var_s0->info->fadeTime;
                if (var_s0->time < temp_f1) {
                    var_s0->time = temp_f1;
                }
                NodePool_Free(&arg0->m_SwooshPool, var_s0);
            }
            var_s0 = temp_s1;
        } while (var_s0 != NULL);
    }
}

/* Runs the repeating timers: each counts down by the frame time, fires its callback,
 * and is freed after its last repeat. */
void SwooshSystem_UpdateTimers(CSwooshSystem *arg0) {
    f32 temp_f0;
    f32 temp_f1;
    s32 temp_v0;
    SwooshTimer *temp_s1;
    SwooshTimer *var_s0;

    var_s0 = arg0->m_TimerPool.used.head;
    if (var_s0 != NULL) {
        do {
            temp_f1 = var_s0->time - gFrameIncrement;
            temp_s1 = var_s0->next;
            var_s0->time = temp_f1;
            if (temp_f1 <= 0.0f) {
                temp_f0 = temp_f1 + var_s0->period;
                var_s0->time = temp_f0;
                if (temp_f0 < 0.0f) {
                    var_s0->time = 0.0f;
                }
                var_s0->callback(var_s0);
                temp_v0 = var_s0->repeats - 1;
                var_s0->repeats = temp_v0;
                if (temp_v0 <= 0) {
                    NodePool_Free(&arg0->m_TimerPool, var_s0);
                }
            }
            var_s0 = temp_s1;
        } while (var_s0 != NULL);
    }
}

/* Loads swoosh arg1's texture set (its record's id, -1: untextured) and adds its draw mode to particle arg2. */
void Trail_LoadTexture(s32 arg0, Swoosh *arg1, s32 arg2) {
    s32 temp_a1;
    void *temp_s0;

    temp_a1 = arg1->layer;
    if (temp_a1 != -1) {
        temp_s0 = arg1->texture;
        Material_GetTextureSet(temp_s0, temp_a1);
        if (arg2 != 0) {
            CParticle__AddDrawModeBits((void *) (sptr) arg2, temp_s0);
        }
    }
}

/* Detaches swoosh arg0 from its owner (flags & 1: counted in a game object's m_ActiveSwooshes,
 * & 2: in a particle's) and lets it fade out. */
void Swoosh_ReleaseOwner(Swoosh *arg0) {
    f32 temp_f1;
    u8 temp_v0;
    u8 temp_v0_2;
    void *temp_v1;
    void *temp_v1_2;

    temp_v1 = arg0->owner;
    if (temp_v1 != NULL) {
        if (arg0->flags & 1) {
            temp_v0 = ((CGameObjectInstance *) temp_v1)->m_ActiveSwooshes;
            if (temp_v0 != 0) {
                ((CGameObjectInstance *) temp_v1)->m_ActiveSwooshes = temp_v0 - 1;
            }
        }
        if (arg0->flags & 2) {
            temp_v1_2 = arg0->owner;
            temp_v0_2 = ((CParticle *) temp_v1_2)->m_ActiveSwooshes;
            if (temp_v0_2 != 0) {
                ((CParticle *) temp_v1_2)->m_ActiveSwooshes = temp_v0_2 - 1;
            }
        }
    }
    arg0->owner = NULL;
    temp_f1 = arg0->info->fadeTime;
    if (arg0->time < temp_f1) {
        arg0->time = temp_f1;
    }
}

/* A texture image as Sprite_FromImage reads it (Rage Wars' Image, rw/abu_types.h). */
typedef struct ImageHeader {
    /* 0x00 */ char pad0[2];
    /* 0x02 */ u8 widthShift;
    /* 0x03 */ u8 heightShift;
    /* 0x04 */ char pad4[0x1D - 4];
    /* 0x1D */ u8 scale;
    /* 0x1E */ char pad1E[0x21 - 0x1E];
    /* 0x21 */ u8 width;
    /* 0x22 */ u8 height;
    /* 0x23 */ u8 format;
    /* 0x24 */ char data[1];
} ImageHeader;
typedef struct Image {
    /* 0x00 */ char pad0[8];
    /* 0x08 */ ImageHeader *header;
    /* 0x0C */ s32 fieldC;
    /* 0x10 */ s32 field10;
    /* 0x14 */ char pad14[0x3C - 0x14];
    /* 0x3C */ s32 flags;
} Image;
typedef struct Sprite {
    /* 0x00 */ s32 format;
    /* 0x04 */ char *data;
    /* 0x08 */ s32 field8;
    /* 0x0C */ s32 fieldC;
    /* 0x10 */ f32 width;
    /* 0x14 */ f32 height;
    /* 0x18 */ f32 extentX;
    /* 0x1C */ f32 extentY;
    /* 0x20 */ f32 step;
} Sprite;

void Sprite_FromImage(Image *image, Image *texture, Sprite *sprite) {
    f32 scale;
    s32 height;
    f32 *unit;
    sprite->data = image->header->data;
    sprite->format = image->header->format;
    if (image->flags & 2) {
        sprite->field8 = image->fieldC;
        sprite->fieldC = image->field10;
    } else {
        sprite->field8 = image->fieldC;
        sprite->fieldC = image->fieldC;
    }
    scale = 3.125f;
    sprite->width = image->header->width * scale;
    sprite->height = image->header->height * scale;
    if (texture != 0) {
        sprite->extentX = 1 << (texture->header->widthShift + 5);
        sprite->extentX = sprite->extentX * image->header->scale - sSpriteTexelScale;
        height = 1 << (texture->header->heightShift + 5);
        unit = &sSpriteTexelScale + 1;
        sprite->step = *unit / ((f32)sprite->format - *unit);
        sprite->extentY = height;
    }
}

#ifndef TARGET_PORT
/* Sprite_FromImage's other two literals, which it reads as sSpriteTexelScale and the word after it:
 * defined here, after its own (the port keeps the generated copies). */
const f32 sSpriteTexelScale_def __asm__("sSpriteTexelScale") = 32.0f;
const f32 sSpriteOne_def __asm__("sSpriteOne") = 1.0f;
#endif

/* Moves point n to its owner particle's draw matrix (-1: the owner is gone, the point keeps the
 * other frame buffer's matrix), scaled to world units (0.2926 for a model particle). */
void Swoosh_UpdateNodeFromOwner(Swoosh *e, SwooshNode *n) {
    u8 m[0x40];
    CParticle *o;
    f32 s;
    o = n->owner;
    if (o != ((void *)0)) {
        if (o == (CParticle *)-1) {
            n->mtx[gEvenOdd] = n->mtx[gEvenOdd ^ 1];
            n->owner = ((void *)0);
        } else {
            CMtxF__FromMtxAffine(m, o->m_mtxDraw[gEvenOdd]);
            s = 0.01f;
            if (o->m_pEffect->m_pModel != 0) {
                s = 0.29257143f;
            }
            CMtxF__ScaleAxes(m, s, s, s);
            CMtxF__ClampEntries(m);
            CMtxF__GetTranslation(m, &n->pos);
            if (e->flags & 4) {
                CMtxF__GetScale(m, &n->scale);
            } else {
                CMtxF__ToMtxAffine(m, &n->mtx[gEvenOdd]);
            }
        }
        n->life = e->info->nodeLife;
    }
}

typedef struct Segment {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ struct Segment *next;
    /* 0x08 */ u8 pad8[0xA8 - 0x8];
    /* 0xA8 */ f32 start;
    /* 0xAC */ f32 length;
} Segment;
typedef struct {
    /* 0x00 */ u8 pad0[0x40];
    /* 0x40 */ Segment *segments;
    /* 0x44 */ u8 pad44[0x4C - 0x44];
    /* 0x4C */ f32 totalLength;
} Path;

/* Stores where each segment starts, as a fraction of the whole path. */
void Path_StoreSegmentFractions(Path *arg0) {
    f32 temp_f0;
    f32 var_f1;
    Segment *var_v0;

    if (arg0->totalLength > 0.0f) {
        var_f1 = 0.0f;
        var_v0 = arg0->segments;
        if (var_v0 != NULL) {
            do {
                var_v0->start = (f32) (var_f1 / arg0->totalLength);
                temp_f0 = var_v0->length;
                var_v0 = var_v0->next;
                var_f1 += temp_f0;
            } while (var_v0 != NULL);
        }
    }
}

void Trail_Draw(sptr arg0, Trail *arg1, s32 arg2) {
    if (arg1->flags & 8) {
        Trail_PlaceNodes(arg1);
    }
    if (arg1->flags & 4) {
        Trail_ComputeFrames(arg1, arg2);
    }
    if (arg1->nodes.count >= 2) {
        if (arg1->layer >= 0) {
            Trail_DrawTextured((void *) arg0, arg1, arg2);
            return;
        }
        Trail_DrawUntextured((CSwooshSystem *) arg0, arg1, arg2);
    }
}

/* Adds a point at matrix arg3 to swoosh arg1 when arg2 is its owner particle and it is still live. */
void Swoosh_AddNodeFromMtx(s32 arg0, Swoosh *arg1, CParticle *arg2, s32 arg3) {
    u8 sp10[0x40];
    f32 var_f0;
    CParticle *temp_s0;

    temp_s0 = arg1->owner;
    if ((temp_s0 == arg2) && (arg1->time > 0.0f)) {
        CMtxF__FromMtxAffine(sp10, arg3);
        var_f0 = 0.01f;
        if (temp_s0->m_pEffect->m_pModel != NULL) {
            var_f0 = 0.29257143f;
        }
        CMtxF__ScaleAxes(sp10, var_f0, var_f0, var_f0);
        CMtxF__ClampEntries(sp10);
        Swoosh_NewNode(arg0, (sptr) arg1, (sptr) sp10);
    }
}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/SYSTEM_3/802A6AC0_2.CPP, func_802A6450_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

/* The live swoosh of owner arg1 with id arg2, or 0. */
void *SwooshSystem_FindSwoosh(CSwooshSystem *arg0, sptr arg1, s32 arg2) {
    Swoosh *node;
    node = arg0->m_SwooshPool.used.head;
    if (node != 0) {
        do {
            if ((sptr)node->owner == arg1) {
                if (node->id == arg2) {
                    return node;
                }
            }
            node = node->next;
        } while (node != 0);
    }
    return 0;
}

/* Static inline in the Rage Wars source file (dropped by the importer). */
static inline void upd(CSwooshSystem *m, Swoosh *e, s32 c) {
    if (e->flags & 8) {
        Trail_PlaceNodes(e);
    }
    if (e->flags & 4) {
        Trail_ComputeFrames(e, c);
    }
    if (e->nodes.count >= 2) {
        if (e->layer >= 0) {
            Trail_DrawTextured(m, e, c);
        } else {
            Trail_DrawUntextured(m, e, c);
        }
    }
}

/* Draws the live swooshes: the untextured layers (< 0) first, then the textured ones (> 0). */
void SwooshSystem_DrawTrails(CSwooshSystem *m, s32 c) {
    Swoosh *e;
    for (e = m->m_SwooshPool.used.head; e != ((void *)0); e = e->next) {
        if (e->layer < 0) {
            upd(m, e, c);
        }
    }
    for (e = m->m_SwooshPool.used.head; e != ((void *)0); e = e->next) {
        if (e->layer > 0) {
            upd(m, e, c);
        }
    }
}
