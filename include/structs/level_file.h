#ifndef STRUCTS_LEVEL_FILE_H
#define STRUCTS_LEVEL_FILE_H

/* Small records of the level file shared by the loader (SceneLoad), the collision
 * code and the Rage Wars imports (rw/abu_types.h includes this). */

#include "common.h"

/* A resident data chunk of the level file (CScene__LoadResidentData: the vertices,
 * faces and edge data): the element width, the count, then the data. */
typedef struct ResidentDataChunk {
    s32 width;
    s32 count;
    u8 data[1];
} ResidentDataChunk;

/* A vertex of the collision mesh as the level loader builds it (CGameRegion__Unpack,
 * UnpackScaledVec4): the floor point and the ceiling height above it
 * (CGameRegion__GetCeilingNormal reads the ceiling as its y). */
typedef struct CollisionBuildVertex {
    /* 0x0 */ f32 x;
    /* 0x4 */ f32 y;
    /* 0x8 */ f32 z;
    /* 0xC */ f32 ceiling;
} CollisionBuildVertex; /* size 0x10 */

#endif
