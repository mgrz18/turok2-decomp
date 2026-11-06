#include "common.h"
#include "structs/vector.h"

typedef struct {
    f32 r, g, b;
} ColorF;

typedef struct {
    /* 0x0 */ s16 x, y, z;
    /* 0x6 */ u16 bone; /* matrix index in the high byte */
    /* 0x8 */ u8 pad8[8];
} ModelVertex; /* size 0x10 */

typedef struct {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ s32 count;
    /* 0x8 */ ModelVertex verts[1];
} VertexGroup;

void *CISet__GetBlock(s32 *, s32);
void CMtxF__FromMtxAffine(f32 *mtx, void *packed);
void CVector3__Normalize(CVector3 *v);
void CMtxF__TransformPoint(f32 *mtx, CVector3 *in, CVector3 *out);
ColorF Color_HSVToRGB(f32 h, f32 s, f32 v);

/* Debug coloring: each vertex gets a hue from its transformed position along
 * the (1,1,1) diagonal. */
void Lighting_DebugHueColors(s32 *model, u8 *matrices, CVector3 *colors, s32 skinned) {
    f32 mtx[16];
    CVector3 axis;
    ColorF c;
    CVector3 in;
    CVector3 out;
    VertexGroup *group;
    ModelVertex *verts;
    s32 n;
    f32 hue;
    s32 i;
    s32 j;

    n = 0;
    CMtxF__FromMtxAffine(mtx, matrices);
    axis.x = 1.0f;
    axis.y = 1.0f;
    axis.z = 1.0f;
    CVector3__Normalize(&axis);
    for (i = 0; i < *model; i++) {
        group = CISet__GetBlock(model, i);
        verts = group->verts;
        for (j = 0; j < group->count; j++) {
            if (skinned) {
                CMtxF__FromMtxAffine(mtx, matrices + (verts[j].bone >> 8) * 64);
            }
            in.x = verts[j].x;
            in.y = verts[j].y;
            in.z = verts[j].z;
            CMtxF__TransformPoint(mtx, &in, &out);
            hue = out.x * axis.x + out.y * axis.y + out.z * axis.z;
            hue /= 1024.0f;
            c = Color_HSVToRGB(hue, 1.0f, 1.0f);
            colors[n].x = c.r * 255.0f;
            colors[n].y = c.g * 255.0f;
            colors[n].z = c.b * 255.0f;
            n++;
        }
    }
}
