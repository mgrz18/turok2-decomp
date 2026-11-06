#include "common.h"
#include "structs/vector.h"
#include "globals/scene.h"

typedef struct {
    /* 0x0 */ s16 x, y, z;
    /* 0x6 */ u16 bone; /* matrix index in the high byte; 0x80 = unlit */
    /* 0x8 */ u8 pad8[4];
    /* 0xC */ s8 nx, ny, nz;
    /* 0xF */ u8 padF;
} ModelVertex; /* size 0x10 */

typedef struct {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ s32 count;
    /* 0x8 */ ModelVertex verts[1];
} VertexGroup;

typedef struct {
    /* 0x00 */ u8 pad0[0x11];
    /* 0x11 */ s8 lightX, lightY, lightZ;
} LightInfo;


void *CISet__GetBlock(s32 *, s32);
void CMtxF__FromMtxAffine(f32 *mtx, void *packed);
void CVector3__Normalize(CVector3 *v);
void CMtxF__RotateVector(f32 *mtx, CVector3 *in, CVector3 *out);

/* Adds directional light to the model's vertex colors: each lit vertex's
 * rotated normal against the light direction, plus an ambient term. */
void Lighting_ApplyDirectional(s32 *model, u8 *matrices, CVector3 *colors, s32 skinned) {
    CVector3 light;
    f32 mtx[16];
    CVector3 in;
    CVector3 out;
    VertexGroup *group;
    ModelVertex *verts;
    f32 level;
    s32 n;
    s32 i;
    s32 j;
    s32 count;

    CMtxF__FromMtxAffine(mtx, matrices);
    light.x = ((LightInfo *)gScene.m_pLevelHeader)->lightX;
    n = 0;
    light.y = ((LightInfo *)gScene.m_pLevelHeader)->lightY;
    light.z = ((LightInfo *)gScene.m_pLevelHeader)->lightZ;
    CVector3__Normalize(&light);
    for (i = 0; i < *model; i++) {
        group = CISet__GetBlock(model, i);
        count = group->count;
        verts = group->verts;
        for (j = 0; j < count; j++) {
            if (verts[j].bone & 0x80) {
                continue;
            }
            if (skinned) {
                CMtxF__FromMtxAffine(mtx, matrices + (verts[j].bone >> 8) * 64);
            }
            in.x = verts[j].nx;
            in.y = verts[j].ny;
            in.z = verts[j].nz;
            CMtxF__RotateVector(mtx, &in, &out);
            level = out.x * light.x + out.y * light.y + out.z * light.z;
            /* clamp at 0, then add the ambient term */
            level = ((level < 0.0f ? 0.0f : level) + 0.21960786f) * 255.0f;
            colors[n].x += level;
            colors[n].y += level;
            colors[n].z += level;
            n++;
        }
    }
}
