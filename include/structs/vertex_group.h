#ifndef STRUCTS_VERTEX_GROUP_H
#define STRUCTS_VERTEX_GROUP_H

/* A model's vertex block, CISet__GetBlock(model, i) for i < *model: a count at 4, then the
 * N64 vertices, read with their colours (Vtx_t: Lighting_CopyVertexColors, Model_PulseVertexColors)
 * or their normals (Vtx_tn: Lighting_ApplyDirectional). flag: the matrix index in the high byte,
 * 0x80 = unlit, 0x70 the pulse phase, 0xC the pulse shift. */

#include "common.h"
#include "structs/gbi.h"

typedef union ModelVtx {
    Vtx_t v;
    Vtx_tn n;
} ModelVtx; /* size 0x10 */

typedef struct VertexGroup {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ s32 count;
    /* 0x8 */ ModelVtx verts[1];
} VertexGroup;

#endif
