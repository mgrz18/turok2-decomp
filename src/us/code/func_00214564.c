/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/FX/8024F950.CPP, func_8024FD80_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/vector.h"
#include "globals/scene.h"
#include "globals/camera.h"
#include "globals/engine.h"
#include "protos/scene.h"

/* A placed prop as the level file stores it. */
typedef struct {
    /* 0x00 */ s32 value;
    /* 0x04 */ CVector3 position;
    /* 0x10 */ f32 scale[3];
    /* 0x1C */ u16 extents[6];
    /* 0x28 */ u16 id;
    /* 0x2A */ u16 segment;
    /* 0x2C */ u16 model;
    /* 0x2E */ u8 flags;
    /* 0x2F */ s8 rotation[4];
} PlacedPropRecord;

typedef struct {
    /* 0x00 */ u8 state;
    /* 0x02 */ u16 id;
    /* 0x04 */ CVector3 position;
    /* 0x10 */ void *segment;
    /* 0x14 */ void *model;
    /* 0x18 */ s32 owner;
    /* 0x1C */ s32 field1C;
    /* 0x20 */ s32 field20;
#ifdef TARGET_PORT /* m_pOwner, a pointer */
    void **field24;
#else
    /* 0x24 */ s32 field24;
#endif
    /* 0x28 */ f32 matrix[16];
    /* 0x68 */ f32 min[3];
    /* 0x74 */ f32 max[3];
    /* 0x80 */ s32 key;
    /* 0x84 */ s32 value;
    /* 0x88 */ u16 flags;
    /* 0x8A */ u8 colorFrame;
} PlacedProp;
#ifdef TARGET_PORT
#include "structs/level_actor.h"
#include "structs/region.h"
T2_STATIC_OBJECT_CHECK(PlacedProp, matrix, flags);
#endif

void CQuaternion__Normalize(f32 *);
void CQuaternion__ToMatrix(f32 *, void *);
void CMtxF__ScaleAxes(void *, f32, f32, f32);
void CMtxF__Translate(void *, f32, f32, f32);
void CMtxF__SwapYZRows(void *);
void CMtxF__Copy(f32 *, void *);
void CMtxF__ToMtxAffine(void *, f32 *);
f32 HalfToFloat(s32);

static __inline__ s32 next_key(void) {
    s32 next;

    next = gNextUniqueId + 1;
    gNextUniqueId = next;
    if (next == 0x3FFFFF) {
        gNextUniqueId = 0x380000;
    }
    return gNextUniqueId;
}

void CStaticObject__Construct(PlacedProp *prop, PlacedPropRecord *record, s32 owner, u8 *segments) {
    f32 rotation[4];
    u8 matrix[0x40];

    prop->state = 0;
    prop->id = record->id;
    prop->owner = owner;
    prop->field1C = 0;
    prop->model = CScene__GetIntelligence(&gScene, record->model);
    prop->flags = record->flags;
    rotation[0] = record->rotation[0] * 0.007874016f;
    rotation[1] = record->rotation[1] * 0.007874016f;
    rotation[2] = record->rotation[2] * 0.007874016f;
    rotation[3] = record->rotation[3] * 0.007874016f;
    CQuaternion__Normalize(rotation);
    CQuaternion__ToMatrix(rotation, matrix);
    CMtxF__ScaleAxes(matrix, record->scale[0], record->scale[1], record->scale[2]);
    CMtxF__Translate(matrix, record->position.x, record->position.y, record->position.z);
    CMtxF__SwapYZRows(matrix);
    if (prop->flags & 0x40) {
        prop->min[0] = record->position.x - 102.399994f;
        prop->min[1] = record->position.y - 102.399994f;
        prop->min[2] = record->position.z - 102.399994f;
        prop->max[0] = record->position.x + 102.399994f;
        prop->max[1] = record->position.y + 102.399994f;
        prop->max[2] = record->position.z + 102.399994f;
        CMtxF__Copy(prop->matrix, matrix);
        prop->value = gDefaultVisBits;
    } else {
        prop->min[0] = HalfToFloat(record->extents[0]) + record->position.x - 0.5f;
        prop->min[1] = HalfToFloat(record->extents[1]) + record->position.y - 0.5f;
        prop->min[2] = HalfToFloat(record->extents[2]) + record->position.z - 0.5f;
        prop->max[0] = HalfToFloat(record->extents[3]) + record->position.x + 0.5f;
        prop->max[1] = HalfToFloat(record->extents[4]) + record->position.y + 0.5f;
        prop->max[2] = HalfToFloat(record->extents[5]) + record->position.z + 0.5f;
        CMtxF__ToMtxAffine(matrix, prop->matrix);
        prop->value = record->value;
    }
    prop->position = record->position;
    if (record->segment == 0xFFFF) {
        prop->segment = 0;
    } else {
#ifdef TARGET_PORT /* the region array (section 2 of the region data) at the host CGameRegion stride: at
                    * 32 bytes a static object's region was a wrong address (pointer-wide host regions), and
                    * its first event (a particle traced from that region) crashed in Collision_SweepRegions */
        prop->segment = (void *) ((CGameRegion *) segments + record->segment);
#else
        prop->segment = segments + record->segment * 32;
#endif
    }
    prop->key = next_key() << 10;
    prop->colorFrame = (u8)gFrameCount - 1;
}
