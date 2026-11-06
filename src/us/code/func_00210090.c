/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/GAME/CODE_8027302C.CPP, func_80273198_de, with Turok 2's layouts. */

#include "common.h"
#include "structs/vector.h"

typedef struct {
    /* 0x00 */ CVector3 right;
    /* 0x0C */ f32 m03;
    /* 0x10 */ CVector3 up;
    /* 0x1C */ f32 m13;
    /* 0x20 */ CVector3 forward;
    /* 0x2C */ f32 m23;
    /* 0x30 */ f32 m30, m31, m32, m33;
} DirMtx;

extern CVector3 D_800A5404; /* {0, 1, 0} */

void CVector3__Cross(CVector3 *out, CVector3 *a, CVector3 *b); /* cross product */

/* Builds an orientation matrix whose forward axis is direction. */
void CMtxF__SetLookDirection(DirMtx *m, CVector3 *direction) {
    CVector3 up;

    up = D_800A5404;
    if (direction->y < 0.0f ? 0.99f <= -direction->y : 0.99f <= direction->y) {
        m->right.y = 0;
        m->right.z = 0;
        m->right.x = 1.0f;
    } else {
        CVector3__Cross(&m->right, &up, direction);
    }
    CVector3__Cross(&m->up, &m->right, direction);
    m->forward = *direction;
    m->m03 = m->m13 = m->m23 = m->m30 = m->m31 = m->m32 = 0.0f;
    m->m33 = 1.0f;
}
