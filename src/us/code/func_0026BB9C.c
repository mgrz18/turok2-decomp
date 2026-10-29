/* WRONG CUT: the loop body continues as .L0026BBC0 (us/asm/6C7C0.s) up to func_0026BC54;
 * the "return 0" exit at 0x8026BBB8 has no symbol. */
#include "common.h"

typedef struct {
    f32 x, y, z;
} Vtx3;

typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ Vtx3 *verts[3];
} Tri;

/* Is (x, z) inside the XZ projection of the triangle? */
s32 func_0026BB9C(Tri *tri, f32 x, f32 z) {
    s32 i, next;
    Vtx3 *a, *b;

    if (tri == NULL) {
        return 1;
    }
    for (i = 0; i < 3; i = next) {
        next = i + 1;
        a = tri->verts[i];
        b = tri->verts[next % 3];
        if ((b->z - a->z) * (x - a->x) + (a->x - b->x) * (z - a->z) < 0.0f) {
            return 0;
        }
    }
    return 1;
}
