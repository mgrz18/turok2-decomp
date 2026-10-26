#include "common.h"

typedef struct {
    /* 0x0 */ s16 x, y, z;
    /* 0x6 */ u16 bone;
    /* 0x8 */ u8 pad8[4];
    /* 0xC */ u8 r, g, b, a;
} ModelVertex; /* size 0x10 */

typedef struct {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ s32 count;
    /* 0x8 */ ModelVertex verts[1];
} VertexGroup;

typedef struct {
    u8 r, g, b, a;
} Color4;

void *func_002017D4(s32 *, s32);

#define MIN(a, b) ((a) < (b) ? (a) : (b))

/* Copies every vertex color of the model into `colors`, clamped to 30..225. */
void func_00212F98(s32 *model, Color4 *colors) {
    VertexGroup *group;
    ModelVertex *verts;
    Color4 *c;
    s32 i;
    s32 j;
    s32 n;
    u8 x;
    s32 k;

    k = 0;
    for (i = 0; i < *model; i++) {
        group = func_002017D4(model, i);
        verts = group->verts;
        n = group->count;
        for (j = 0; j < n; j++) {
            c = &colors[k++];
            do {
                x = verts[j].r;
                c->r = MIN(verts[j].r, 225) < 31 ? 30 : MIN(x, 225);
            } while (0);
            x = verts[j].g;
            c->g = MIN(verts[j].g, 225) < 31 ? 30 : MIN(x, 225);
            x = verts[j].b;
            c->b = MIN(verts[j].b, 225) < 31 ? 30 : MIN(x, 225);
        }
    }
}
