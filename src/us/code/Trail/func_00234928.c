/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802A25D0_de_10.c, func_802A3DD4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/gbi.h"
#include "structs/morph.h"
#include "structs/swoosh.h"
#include "gfx.h"
#include "gbi_t2.h"
#include "globals/engine.h"

sptr RibbonVtxPool_Alloc(void *pool, s32 count);
void Morph_BlendVertices(Swoosh *owner, SwooshNode *clock, Vtx_t *dst, Morph *morph);

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
