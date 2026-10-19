/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802A25D0_de_10.c, func_802A3DD4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern Gfx *D_800D8E24;
extern s32 D_800B6D1C;
extern float D_800A6488;
Vtx *func_00235F8C(func_802A66B4_S1 *pool, s32 count);
void func_0023353C(Actor_func_802A274C_de *actor, ObjRecAt28V1 *clock, Vtx *vertices, Morph *morph);
void func_00234928(func_802A66B4_S1 *pool, Actor_func_802A274C_de *actor) {
    Morph storage;
    Morph *morph = &storage;
    ObjRecAt28V1 *clock, *next;
    Vtx *vertices;
    u32 count, i;
    s32 edges;
    morph->blends = actor->track->blends;
    morph->count = actor->track->count;
    if (actor->flags & 2) { morph->first = actor->first; morph->second = actor->second; }
    else { morph->first = actor->first; morph->second = actor->first; }
    morph->scaleS = actor->track->scaleS * D_800A6488;
    morph->scaleT = actor->track->scaleT * D_800A6488;
    clock = actor->clock;
    next = clock->next;
    count = actor->track->count;
    edges = count - 1;
    { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->words.w0 = ((((unsigned int)(0xDA) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (5)))) << (19)) | (((unsigned int)(((((unsigned int)(0x1)) << 1)) ^ (((unsigned int)(0x1)) << 0)) & (0xFFFFFFFFU >> (32 - (8)))) << (0))); _gbi->words.w1 = ((unsigned int)(&clock->matrix[D_800B6D1C])); };
    while (clock != 0 && next != 0 && 0.0f < clock->unk8 && 0.0f < next->unk8) {
        vertices = func_00235F8C(pool, count);
        if (vertices == 0) break;
        func_0023353C(actor, clock, vertices, &storage);
        { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->words.w0 = ((((unsigned int)(0x01) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(count) & (0xFFFFFFFFU >> (32 - (8)))) << (12)) | (((unsigned int)((0) + (count)) & (0xFFFFFFFFU >> (32 - (7)))) << (1))); _gbi->words.w1 = ((unsigned int)(vertices)); };
        { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->words.w0 = ((((unsigned int)(0xDA) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (5)))) << (19)) | (((unsigned int)(((((unsigned int)(0x1)) << 1)) ^ (((unsigned int)(0x1)) << 0)) & (0xFFFFFFFFU >> (32 - (8)))) << (0))); _gbi->words.w1 = ((unsigned int)(&next->matrix[D_800B6D1C])); };
        vertices = func_00235F8C(pool, count);
        if (vertices == 0) return;
        func_0023353C(actor, next, vertices, &storage);
        { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->words.w0 = ((((unsigned int)(0x01) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(count) & (0xFFFFFFFFU >> (32 - (8)))) << (12)) | (((unsigned int)((count) + (count)) & (0xFFFFFFFFU >> (32 - (7)))) << (1))); _gbi->words.w1 = ((unsigned int)(vertices)); };
        i = 0;
        while ((s32)i < edges) {
            { Gfx *cmd = D_800D8E24++;
              cmd->words.w0 = (((i * 2 & 255) << 16) | (((count + i) * 2 & 255) << 8) | ((count + i + 1) * 2 & 255)) | (0x06 << 24);
              cmd->words.w1 = ((i * 2 & 255) << 16) | (((count + i + 1) * 2 & 255) << 8) | ((i + 1) * 2 & 255);
            }
            i++;
        }
        if (actor->track->closed) {
            { Gfx *cmd = D_800D8E24++;
              cmd->words.w0 = (((i * 2 & 255) << 16) | (((count + i) * 2 & 255) << 8) | (count * 2 & 255)) | (0x06 << 24);
              cmd->words.w1 = ((i * 2 & 255) << 16) | ((count * 2 & 255) << 8);
            }
        }
        clock = next;
        next = clock->next;
    }
}
