#include "common.h"
#include "protos/iset.h"
#include "structs/vertex_group.h"

typedef struct {
    u8 r, g, b, a;
} Color4;


#define MIN(a, b) ((a) < (b) ? (a) : (b))

/* Copies every vertex color of the model into `colors`, clamped to 30..225. */
void Lighting_CopyVertexColors(s32 *model, Color4 *colors) {
    VertexGroup *group;
    ModelVtx *verts;
    Color4 *c;
    s32 i;
    s32 j;
    s32 n;
    u8 x;
    s32 k;

    k = 0;
    for (i = 0; i < *model; i++) {
        group = CISet__GetBlock(model, i);
        verts = group->verts;
        n = group->count;
        for (j = 0; j < n; j++) {
            c = &colors[k++];
            do {
                x = verts[j].v.cn[0];
                c->r = MIN(verts[j].v.cn[0], 225) < 31 ? 30 : MIN(x, 225);
            } while (0);
            x = verts[j].v.cn[1];
            c->g = MIN(verts[j].v.cn[1], 225) < 31 ? 30 : MIN(x, 225);
            x = verts[j].v.cn[2];
            c->b = MIN(verts[j].v.cn[2], 225) < 31 ? 30 : MIN(x, 225);
        }
    }
}
