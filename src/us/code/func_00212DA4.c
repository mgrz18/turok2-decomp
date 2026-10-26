#include "common.h"

typedef struct {
    f32 x, y, z;
} Vec3;

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

void *func_002017D4(s32 *, s32);
void func_0020D908(f32 *mtx, void *packed);
void func_0020F040(Vec3 *v);
void func_0020F85C(f32 *mtx, Vec3 *in, Vec3 *out);
ColorF func_00211DB0(f32 h, f32 s, f32 v);

/* Debug coloring: each vertex gets a hue from its transformed position along
 * the (1,1,1) diagonal. */
void func_00212DA4(s32 *model, u8 *matrices, Vec3 *colors, s32 skinned) {
    f32 mtx[16];
    Vec3 axis;
    ColorF c;
    Vec3 in;
    Vec3 out;
    VertexGroup *group;
    ModelVertex *verts;
    s32 n;
    f32 hue;
    s32 i;
    s32 j;

    n = 0;
    func_0020D908(mtx, matrices);
    axis.x = 1.0f;
    axis.y = 1.0f;
    axis.z = 1.0f;
    func_0020F040(&axis);
    for (i = 0; i < *model; i++) {
        group = func_002017D4(model, i);
        verts = group->verts;
        for (j = 0; j < group->count; j++) {
            if (skinned) {
                func_0020D908(mtx, matrices + (verts[j].bone >> 8) * 64);
            }
            in.x = verts[j].x;
            in.y = verts[j].y;
            in.z = verts[j].z;
            func_0020F85C(mtx, &in, &out);
            hue = out.x * axis.x + out.y * axis.y + out.z * axis.z;
            hue /= 1024.0f;
            c = func_00211DB0(hue, 1.0f, 1.0f);
            colors[n].x = c.r * 255.0f;
            colors[n].y = c.g * 255.0f;
            colors[n].z = c.b * 255.0f;
            n++;
        }
    }
}
