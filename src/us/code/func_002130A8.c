#include "common.h"

typedef struct {
    f32 x, y, z;
} Vec3;

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

extern LightInfo *D_8010FEC8;

void *func_002017D4(s32 *, s32);
void func_0020D908(f32 *mtx, void *packed);
void func_0020F040(Vec3 *v);
void func_0020FAFC(f32 *mtx, Vec3 *in, Vec3 *out);

/* Adds directional light to the model's vertex colors: each lit vertex's
 * rotated normal against the light direction, plus an ambient term. */
void func_002130A8(s32 *model, u8 *matrices, Vec3 *colors, s32 skinned) {
    Vec3 light;
    f32 mtx[16];
    Vec3 in;
    Vec3 out;
    VertexGroup *group;
    ModelVertex *verts;
    f32 level;
    s32 n;
    s32 i;
    s32 j;
    s32 count;

    func_0020D908(mtx, matrices);
    light.x = D_8010FEC8->lightX;
    n = 0;
    light.y = D_8010FEC8->lightY;
    light.z = D_8010FEC8->lightZ;
    func_0020F040(&light);
    for (i = 0; i < *model; i++) {
        group = func_002017D4(model, i);
        count = group->count;
        verts = group->verts;
        for (j = 0; j < count; j++) {
            if (verts[j].bone & 0x80) {
                continue;
            }
            if (skinned) {
                func_0020D908(mtx, matrices + (verts[j].bone >> 8) * 64);
            }
            in.x = verts[j].nx;
            in.y = verts[j].ny;
            in.z = verts[j].nz;
            func_0020FAFC(mtx, &in, &out);
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
