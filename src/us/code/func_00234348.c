/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/RENDER_3/CODE_802A35C0.CPP, func_802A3718_de, with Turok 2's layouts. */
#include "common.h"
#include "gfx.h"
#include "gbi_t2.h"
#include "globals/engine.h"
#include "protos/cache.h"
#include "structs/morph.h"
#include "structs/swoosh.h"

/* Draws a textured trail: spaces the texture along the tail by length, picks
 * the animation frame, sets the combiner and colours, then emits two vertex
 * rows per node and the triangle strip between them. */

typedef struct {
    /* 0x00 */ u8 kind;
    /* 0x01 */ u8 unknown1;
    /* 0x02 */ u8 shiftS;
    /* 0x03 */ u8 shiftT;
} AnimFrame;

typedef struct {
    /* 0x00 */ s32 count;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ AnimFrame *frame;
    /* 0x0C */ s32 unkC;
} AnimInfo;



extern void *Texture_Request(void *model);
extern void Texture_GetInfo(void *model, AnimInfo *info);
extern void Render_SetCombineMode(s32 mode);
extern void Render_SetRenderMode(s32 mode);
extern s32 Trail_GetAnimFrame(Swoosh *trail, SwooshInfo *track, AnimInfo *info);
extern void Texture_Load(void *slot, void *model, s32 frame, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
extern Gfx *RibbonVtxPool_Alloc(void *pool, s32 count);
extern void Morph_BlendVerticesTex(Swoosh *trail, SwooshNode *node, Gfx *vertices, f32 texS, Morph *morph);

void Trail_DrawTextured(void *pool, Swoosh *trail, s32 unused) {
    AnimInfo info;
    Morph storage;
    AnimInfo *anim;
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
    switch (info.frame->kind) {
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
        vertices = RibbonVtxPool_Alloc(pool, count);
        if (vertices == 0) break;
        Morph_BlendVerticesTex(trail, clock, vertices, clock->fraction, &storage);
        GFX_CMD(0x01000000 | (count << 12) | ((count & 0x7F) << 1), GBI_ADDR(vertices))
        GFX_CMD(0xDA380003, GBI_ADDR(&next->mtx[gEvenOdd]))
        vertices = RibbonVtxPool_Alloc(pool, count);
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
