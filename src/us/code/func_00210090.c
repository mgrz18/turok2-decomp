/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/GAME/CODE_8027302C.CPP, func_80273198_de, with Turok 2's layouts. */

#include "common.h"

typedef struct {
    f32 x, y, z;
} Vec3f;

typedef struct {
    /* 0x00 */ Vec3f right;
    /* 0x0C */ f32 m03;
    /* 0x10 */ Vec3f up;
    /* 0x1C */ f32 m13;
    /* 0x20 */ Vec3f forward;
    /* 0x2C */ f32 m23;
    /* 0x30 */ f32 m30, m31, m32, m33;
} DirMtx;

extern Vec3f D_800A5404; /* {0, 1, 0} */

void func_0020EFDC(Vec3f *out, Vec3f *a, Vec3f *b); /* cross product */

/* Builds an orientation matrix whose forward axis is direction. */
void func_00210090(DirMtx *m, Vec3f *direction) {
    Vec3f up;

    up = D_800A5404;
    if (direction->y < 0.0f ? 0.99f <= -direction->y : 0.99f <= direction->y) {
        m->right.y = 0;
        m->right.z = 0;
        m->right.x = 1.0f;
    } else {
        func_0020EFDC(&m->right, &up, direction);
    }
    func_0020EFDC(&m->up, &m->right, direction);
    m->forward = *direction;
    m->m03 = m->m13 = m->m23 = m->m30 = m->m31 = m->m32 = 0.0f;
    m->m33 = 1.0f;
}
