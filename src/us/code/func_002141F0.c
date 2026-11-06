/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/FX/8024F950.CPP, func_8024FD80_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/vector.h"
#include "globals/scene.h"
#include "globals/camera.h"
#include "globals/engine.h"

/* A placed prop's record in the level data. */
typedef struct {
    /* 0x00 */ s32 value;
    /* 0x04 */ f32 scale;
    /* 0x08 */ u16 id;
    /* 0x0A */ u16 segment; /* 0xFFFF = none */
    /* 0x0C */ u16 flags;
    /* 0x0E */ u16 model;
    /* 0x10 */ s16 y;
    /* 0x12 */ s8 extents[6]; /* min x, max x, min y, max y, min z, max z */
    /* 0x18 */ u8 bits; /* 0..1 quarter turns, 0x10 negate x, 0x20 negate z */
    /* 0x19 */ u8 x;
    /* 0x1A */ u8 z;
} PlacedPropRecord;

typedef struct {
    /* 0x00 */ u8 state;
    /* 0x01 */ u8 pad1;
    /* 0x02 */ u16 id;
    /* 0x04 */ CVector3 pos;
    /* 0x10 */ void *segment;
    /* 0x14 */ void *model;
    /* 0x18 */ s32 owner;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ u8 pad20[8];
    /* 0x28 */ f32 matrix[16];
    /* 0x68 */ CVector3 min;
    /* 0x74 */ CVector3 max;
    /* 0x80 */ s32 colorKey;
    /* 0x84 */ s32 value;
    /* 0x88 */ u16 flags;
    /* 0x8A */ u8 colorFrame;
} PlacedProp;

extern s32 gNextUniqueId;

void *CScene__GetIntelligence(void *, s32);
void CMtxF__SetRotateY(f32 *mtx, f32 angle);
void CMtxF__ScaleAxes(f32 *mtx, f32 x, f32 y, f32 z);
void CMtxF__Translate(f32 *mtx, f32 x, f32 y, f32 z);
void CMtxF__SwapYZRows(f32 *mtx);
void CMtxF__Copy(f32 *dst, f32 *src);
void CMtxF__ToMtxAffine(f32 *src, f32 *dst);

#define PI 3.141593f

static __inline__ s32 nextColorKey(void) {
    s32 next;

    next = gNextUniqueId + 1;
    gNextUniqueId = next;
    if (next == 0x3FFFFF) {
        gNextUniqueId = 0x380000;
    }
    return gNextUniqueId;
}

void CStaticObject__ConstructCompact(PlacedProp *prop, PlacedPropRecord *record, s32 owner, u8 *segments, f32 scale) {
    f32 matrix[16];
    CVector3 pos;
    f32 angle;

    angle = 0.0f;
    prop->state = 0;
    prop->id = record->id;
    prop->owner = owner;
    prop->unk1C = 0;
    prop->model = CScene__GetIntelligence(&gScene, record->model);
    pos.x = record->x * scale;
    if (record->bits & 0x10) {
        pos.x = -pos.x;
    }
    pos.z = record->z * scale;
    if (record->bits & 0x20) {
        pos.z = -pos.z;
    }
    pos.y = record->y * 5.12f;
    switch (record->bits & 3) {
        case 0:
            break;
        case 1:
            angle = PI * 0.5f;
            break;
        case 2:
            angle = PI;
            break;
        case 3:
            angle = PI * 1.5f;
            break;
    }
    prop->flags = record->flags;
    CMtxF__SetRotateY(matrix, angle);
    CMtxF__ScaleAxes(matrix, record->scale, record->scale, record->scale);
    CMtxF__Translate(matrix, pos.x, pos.y, pos.z);
    CMtxF__SwapYZRows(matrix);
    if (prop->flags & 0x40) {
        prop->min.x = pos.x - 102.399994f;
        prop->min.y = pos.y - 102.399994f;
        prop->min.z = pos.z - 102.399994f;
        prop->max.x = pos.x + 102.399994f;
        prop->max.y = pos.y + 102.399994f;
        prop->max.z = pos.z + 102.399994f;
        CMtxF__Copy(prop->matrix, matrix);
        prop->value = gDefaultVisBits;
    } else {
        prop->min.x = pos.x + record->extents[0] * 10.24f - 0.5f;
        prop->min.y = pos.y + record->extents[2] * 10.24f - 0.5f;
        prop->min.z = pos.z + record->extents[4] * 10.24f - 0.5f;
        prop->max.x = pos.x + record->extents[1] * 10.24f + 0.5f;
        prop->max.y = pos.y + record->extents[3] * 10.24f + 0.5f;
        prop->max.z = pos.z + record->extents[5] * 10.24f + 0.5f;
        CMtxF__ToMtxAffine(matrix, prop->matrix);
        prop->value = record->value;
    }
    prop->pos = pos;
    if (record->segment == 0xFFFF) {
        prop->segment = NULL;
    } else {
        prop->segment = segments + record->segment * 32;
    }
    prop->colorKey = nextColorKey() << 10;
    prop->colorFrame = (u8)gFrameCount - 1;
}
