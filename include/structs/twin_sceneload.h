/* Local types that vm1/SceneLoad and its vm2 copy both define (one layout): one definition
 * for both copies (tools/struct_census.py). */
#ifndef STRUCTS_TWIN_SCENELOAD_H
#define STRUCTS_TWIN_SCENELOAD_H

#include "common.h"
#include "structs/world.h"
#include "structs/collision.h"
#include "structs/game.h"

typedef struct CollisionBuildTriangle {
    /* 0x00 */ u16 kind;
    /* 0x02 */ u16 flags;
    /* 0x04 */ CollisionBuildVertex *v[3];
    /* 0x10 */ struct CollisionBuildTriangle *adj[3];
    /* 0x1C */ s32 mask;
} CollisionBuildTriangle;
typedef struct CollisionBuildFace {
    u8 data[0x14];
} CollisionBuildFace;
typedef struct CollisionBuildHeader {
    s32 count;
    s32 offsets[5];
} CollisionBuildHeader;
typedef struct CollisionBuildList {
    s32 kind;
    s32 count;
    CollisionBuildFace items[1];
} CollisionBuildList;
typedef struct CollisionBuildVertexList {
    s32 kind, count;
    CollisionBuildVertex items[1];
} CollisionBuildVertexList;
typedef struct CollisionBuildTriangleList {
    s32 kind, count;
    CollisionBuildTriangle items[1];
} CollisionBuildTriangleList;
typedef struct CollisionBuildMeshHandle {
    void *data;
} CollisionBuildMeshHandle;
typedef struct CollisionBuildWorld {
    /* 0x00000 */ char pad0[0x70];
    /* 0x00070 */ void *tree;
    /* 0x00074 */ char pad74[0x18FA8 - 0x74];
    /* 0x18FA8 */ s32 group;
} CollisionBuildWorld;
typedef struct SearchPoint {
    /* 0x00 */ f32 position[3];
    /* 0x0C */ u32 fieldC;
    /* 0x10 */ s16 type;
    /* 0x12 */ s16 field12;
} SearchPoint; /* 0x14 */
typedef struct SearchValues {
    s32 field0;
    s32 count;
    s32 values[1];
} SearchValues;
typedef struct SearchPoints {
    s32 field0;
    s32 field4;
    SearchPoint points[1];
} SearchPoints;
typedef struct ListRec {   /* a CList (structs/list.h) whose m_nCount is read as count */
    /* 0x00 */ void *pad0[2];
    /* 0x08 */ s32 pad8[2];
    /* 0x10 */ s32 count;
    /* 0x14 */ u8 pad14[4];
} ListRec;
typedef struct ListItem {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 ref; /* an index, made a pointer into the level's table */
    /* 0x10 */ u8 data[0xC];
} ListItem; /* 0x1C */
typedef struct ItemList {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 count;
    /* 0x08 */ ListItem items[1];
} ItemList;
typedef struct Counted {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 count;
} Counted;
typedef struct StreamCell {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ s32 occupied;
} StreamCell; /* 0x14 */
typedef struct LevelState {
    /* 0x00 */ u8 pad0[0x98];
    /* 0x98 */ void *handle98;
    /* 0x9C */ void *handle9C;
    /* 0xA0 */ void *handleA0;
    /* 0xA4 */ void *handleA4;
    /* 0xA8 */ void *handleA8;
    /* 0xAC */ void *handleAC;
    /* 0xB0 */ void *handleB0;
    /* 0xB4 */ void *handleB4;
    /* 0xB8 */ u8 padB8[0x10];
    /* 0xC8 */ void *handleC8;
    /* 0xCC */ void *handleCC;
} LevelState;
typedef struct Handle {
    void *data;
} Handle;
typedef struct Placed {
    /* 0x00 */ s32 data[4];
    /* 0x10 */ s16 kind;
    /* 0x12 */ s16 unk12;
} Placed; /* 0x14 */
#ifdef TARGET_PORT
/* The port: the grid sections hold their static objects and level actors at the host sizes
 * (CScene__DecompressGridSection), so these views are host twins of CStaticObject (block =
 * m_pOwner) and CLevelActor. With the N64 stride and offset the third attract demo's level load
 * (level 109) wrote the handle into the wrong objects and passed a garbage m_pOwner to
 * CCache__RequestDecompression. */
#include "structs/level_actor.h"
typedef struct ObjA {
    u8 pad0[__builtin_offsetof(CStaticObject, m_pOwner)];
    void **block;
    u8 pad28[sizeof(CStaticObject) - __builtin_offsetof(CStaticObject, m_pOwner) - sizeof(void **)];
} ObjA;
_Static_assert(sizeof(ObjA) == sizeof(CStaticObject), "ObjA size");

typedef struct ObjB {
    u8 pad0[sizeof(CLevelActor)];
} ObjB;
#else
typedef struct ObjA {
    /* 0x00 */ u8 pad0[0x24];
    /* 0x24 */ void **block;
    /* 0x28 */ u8 pad28[0x68];
} ObjA; /* 0x90 */

typedef struct ObjB {
    /* 0x000 */ u8 pad0[0x198];
} ObjB; /* 0x198 */
#endif
typedef struct Node {
    /* 0x00 */ s32 index;
    /* 0x04 */ void *prev;
    /* 0x08 */ void *next;
} Node;
typedef struct {
    /* 0x00 */ s32 type;
    /* 0x04 */ u8 pad4[0x24];
    /* 0x28 */ f32 x;
    /* 0x2C */ f32 y;
    /* 0x30 */ f32 z30;
    /* 0x34 */ f32 z34;
    /* 0x38 */ u8 pad38[0x14];
    /* 0x4C */ f32 x2;
    /* 0x50 */ f32 y2;
    /* 0x54 */ f32 z2;
} Element;
typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s32 done;
    /* 0x08 */ u8 pad8[0x28 - 0x8];
    /* 0x28 */ s32 unk28;
    /* 0x2C */ u8 pad2C[4];
    /* 0x30 */ s32 unk30;
} Arg0_0042C418;
typedef struct Entry {
    u8 data[0x20];
} Entry;

#endif
