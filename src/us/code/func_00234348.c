/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/RENDER_3/CODE_802A35C0.CPP, func_802A3718_de, with Turok 2's layouts. */
#include "common.h"
#include "gfx.h"
#include "gbi_t2.h"
#include "globals/engine.h"

/* Draws a textured trail: spaces the texture along the tail by length, picks
 * the animation frame, sets the combiner and colours, then emits two vertex
 * rows per node and the triangle strip between them. */

typedef struct TrailNode TrailNode;
struct TrailNode {
    /* 0x00 */ TrailNode *next;
    /* 0x04 */ TrailNode *prev;
    /* 0x08 */ f32 time;
    /* 0x0C */ u8 padC[0x28 - 0xC];
    /* 0x28 */ u8 matrix[2][0x40];
    /* 0xA8 */ f32 offset;
    /* 0xAC */ f32 length;
};

typedef struct {
    /* 0x00 */ u8 pad0[0x12];
    /* 0x12 */ u8 closed;
    /* 0x13 */ u8 pad13[0x16 - 0x13];
    /* 0x16 */ u8 prim[3];
    /* 0x19 */ u8 env[3];
    /* 0x1C */ u8 kind;
    /* 0x1D */ u8 textureScale;
    /* 0x1E */ u8 pad1E[0x21 - 0x1E];
    /* 0x21 */ u8 scaleS;
    /* 0x22 */ u8 scaleT;
    /* 0x23 */ u8 count;
    /* 0x24 */ u8 blends[1];
} TrailTrack;

typedef struct {
    /* 0x00 */ u8 pad0[0x8];
    /* 0x08 */ TrailTrack *track;
    /* 0x0C */ void *first;
    /* 0x10 */ void *second;
    /* 0x14 */ u8 model[0x28 - 0x14];
    /* 0x28 */ f32 phase;
    /* 0x2C */ u8 pad2C[0x3C - 0x2C];
    /* 0x3C */ u32 flags;
    /* 0x40 */ TrailNode *tail;
    /* 0x44 */ TrailNode *clock;
    /* 0x48 */ u8 pad48[0x4C - 0x48];
    /* 0x4C */ f32 length;
} Trail;

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

typedef struct {
    /* 0x00 */ s32 count;
    /* 0x04 */ void *items;
    /* 0x08 */ void *pointsA;
    /* 0x0C */ void *pointsB;
    /* 0x10 */ f32 scaleA;
    /* 0x14 */ f32 scaleB;
    /* 0x18 */ f32 texScaleS;
    /* 0x1C */ f32 texScaleT;
    /* 0x20 */ f32 texStepT;
} Morph;


extern void *Texture_Request(void *model);
extern void Texture_GetInfo(void *model, AnimInfo *info);
extern void Render_SetCombineMode(s32 mode);
extern void Render_SetRenderMode(s32 mode);
extern s32 Trail_GetAnimFrame(Trail *trail, TrailTrack *track, AnimInfo *info);
extern void Texture_Load(void *slot, void *model, s32 frame, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
extern Gfx *RibbonVtxPool_Alloc(void *pool, s32 count);
extern void Morph_BlendVerticesTex(Trail *trail, TrailNode *node, Gfx *vertices, f32 texS, Morph *morph);
extern void CCache__Unlock(s32 a, void *model);

void Trail_DrawTextured(void *pool, Trail *trail, s32 unused) {
    AnimInfo info;
    Morph storage;
    AnimInfo *anim;
    void *model;
    TrailTrack *track;
    TrailNode *clock, *next, *node;
    Gfx *vertices;
    f32 offset;
    f32 period;
    f32 scale;
    s32 count, edges, i, frame;

    track = trail->track;
    if (track->kind == 2 && 0.0f < trail->length) {
        offset = 0.0f;
        for (node = trail->tail; node != 0; node = node->prev) {
            node->offset = offset / trail->length;
            offset += node->length;
        }
    }
    model = Texture_Request(trail->model);
    if (model == 0) return;
    Texture_GetInfo(model, &info);
    anim = &info;
    storage.items = trail->track->blends;
    storage.count = trail->track->count;
    if (trail->flags & 2) {
        storage.pointsA = trail->first;
        storage.pointsB = trail->second;
    } else {
        storage.pointsA = trail->first;
        storage.pointsB = trail->first;
    }
    storage.scaleA = trail->track->scaleS * 3.125f;
    storage.scaleB = trail->track->scaleT * 3.125f;
    if (anim != 0) {
        storage.texScaleS = 1 << (anim->frame->shiftS + 5);
        storage.texScaleS = storage.texScaleS * trail->track->textureScale - 32.0f;
        storage.texScaleT = 1 << (anim->frame->shiftT + 5);
        storage.texStepT = 1.0f / (storage.count - 1.0f);
    }
    period = 1.0f;
    while (trail->phase >= period) trail->phase -= period;
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
    clock = trail->clock;
    count = track->count;
    next = clock->next;
    frame = Trail_GetAnimFrame(trail, track, &info);
    Texture_Load(trail->model, model, frame, 0, 0xFFFF, 0xFFFF, 0, 0, 0);
    edges = count - 1;
    GFX_CMD(0xDA380003, GBI_ADDR(clock->matrix[gEvenOdd]))
    while (clock != 0 && next != 0 && 0.0f < clock->time && 0.0f < next->time) {
        vertices = RibbonVtxPool_Alloc(pool, count);
        if (vertices == 0) break;
        Morph_BlendVerticesTex(trail, clock, vertices, clock->offset, &storage);
        GFX_CMD(0x01000000 | (count << 12) | ((count & 0x7F) << 1), GBI_ADDR(vertices))
        GFX_CMD(0xDA380003, GBI_ADDR(next->matrix[gEvenOdd]))
        vertices = RibbonVtxPool_Alloc(pool, count);
        if (vertices == 0) break;
        scale = next->offset;
        while (clock->offset < scale) scale -= period;
        Morph_BlendVerticesTex(trail, next, vertices, scale, &storage);
        GFX_CMD(0x01000000 | (count << 12) | (((count * 2) & 0x7F) << 1), GBI_ADDR(vertices))
        for (i = 0; i < edges; i++) {
            GFX_CMD((((i * 2 & 255) << 16) | (((count + i) * 2 & 255) << 8) | ((count + i + 1) * 2 & 255)) | (6 << 24), ((i * 2 & 255) << 16) | (((count + i + 1) * 2 & 255) << 8) | ((i + 1) * 2 & 255))
        }
        if (trail->track->closed) {
            GFX_CMD((((i * 2 & 255) << 16) | (((count + i) * 2 & 255) << 8) | (count * 2 & 255)) | (6 << 24), ((i * 2 & 255) << 16) | ((count * 2 & 255) << 8))
        }
        clock = next;
        next = clock->next;
    }
    CCache__Unlock(0, model);
}
