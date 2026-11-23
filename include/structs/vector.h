#ifndef STRUCTS_VECTOR_H
#define STRUCTS_VECTOR_H

/* The engine math types (Acclaim naming, as in Rage Wars): a 3D vector, a
 * rotation quaternion and a 4x4 float matrix. Vectors are passed by value
 * in integer registers like any small struct, so a view with s32 fields
 * compiles the same as CVector3 when only copied. */

#include "common.h"

typedef struct {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 y;
    /* 0x08 */ f32 z;
} CVector3; /* size 0xC */

typedef struct {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 y;
    /* 0x08 */ f32 z;
    /* 0x0C */ f32 w;
} CQuaternion; /* size 0x10 */

typedef f32 CMtxF[4][4];

/* An axis-aligned box. gCameraVisibleBox is the box of what the active cameras can
 * see (widened for animation), gCameraViewBox their plain view boxes
 * (CCameraPool__Advance builds both); an object is drawn or tested when its box
 * (CGameObjectInstance m_vMin / m_vMax) meets it. */
typedef struct CBox {
    /* 0x00 */ CVector3 m_vMin;
    /* 0x0C */ CVector3 m_vMax;
} CBox; /* size 0x18 */

#endif
