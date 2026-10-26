/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/FX/8024F950.CPP, func_8024FD80_de, with Turok 2's layouts. */
#include "common.h"

typedef struct {
    f32 x, y, z;
} Vec3;

/* A placed prop as the level file stores it. */
typedef struct {
    /* 0x00 */ s32 value;
    /* 0x04 */ Vec3 position;
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
    /* 0x04 */ Vec3 position;
    /* 0x10 */ void *segment;
    /* 0x14 */ void *model;
    /* 0x18 */ s32 owner;
    /* 0x1C */ s32 field1C;
    /* 0x20 */ s32 field20;
    /* 0x24 */ s32 field24;
    /* 0x28 */ f32 matrix[16];
    /* 0x68 */ f32 min[3];
    /* 0x74 */ f32 max[3];
    /* 0x80 */ s32 key;
    /* 0x84 */ s32 value;
    /* 0x88 */ u16 flags;
    /* 0x8A */ u8 colorFrame;
} PlacedProp;

void *func_00225E88(void *, s32);
void func_00211074(f32 *);
void func_00211114(f32 *, void *);
void func_0021034C(void *, f32, f32, f32);
void func_00210318(void *, f32, f32, f32);
void func_00210C3C(void *);
void func_0020FF80(f32 *, void *);
void func_0020D274(void *, f32 *);
f32 func_0026D210(s32);

extern u8 D_800F7078[];
extern s32 D_800ACAF0;
extern s32 D_800B6D00;
extern u8 D_800B6D1B;

static __inline__ s32 next_key(void) {
    s32 next;

    next = D_800ACAF0 + 1;
    D_800ACAF0 = next;
    if (next == 0x3FFFFF) {
        D_800ACAF0 = 0x380000;
    }
    return D_800ACAF0;
}

void func_00214564(PlacedProp *prop, PlacedPropRecord *record, s32 owner, u8 *segments) {
    f32 rotation[4];
    u8 matrix[0x40];

    prop->state = 0;
    prop->id = record->id;
    prop->owner = owner;
    prop->field1C = 0;
    prop->model = func_00225E88(D_800F7078, record->model);
    prop->flags = record->flags;
    rotation[0] = record->rotation[0] * 0.007874016f;
    rotation[1] = record->rotation[1] * 0.007874016f;
    rotation[2] = record->rotation[2] * 0.007874016f;
    rotation[3] = record->rotation[3] * 0.007874016f;
    func_00211074(rotation);
    func_00211114(rotation, matrix);
    func_0021034C(matrix, record->scale[0], record->scale[1], record->scale[2]);
    func_00210318(matrix, record->position.x, record->position.y, record->position.z);
    func_00210C3C(matrix);
    if (prop->flags & 0x40) {
        prop->min[0] = record->position.x - 102.399994f;
        prop->min[1] = record->position.y - 102.399994f;
        prop->min[2] = record->position.z - 102.399994f;
        prop->max[0] = record->position.x + 102.399994f;
        prop->max[1] = record->position.y + 102.399994f;
        prop->max[2] = record->position.z + 102.399994f;
        func_0020FF80(prop->matrix, matrix);
        prop->value = D_800B6D00;
    } else {
        prop->min[0] = func_0026D210(record->extents[0]) + record->position.x - 0.5f;
        prop->min[1] = func_0026D210(record->extents[1]) + record->position.y - 0.5f;
        prop->min[2] = func_0026D210(record->extents[2]) + record->position.z - 0.5f;
        prop->max[0] = func_0026D210(record->extents[3]) + record->position.x + 0.5f;
        prop->max[1] = func_0026D210(record->extents[4]) + record->position.y + 0.5f;
        prop->max[2] = func_0026D210(record->extents[5]) + record->position.z + 0.5f;
        func_0020D274(matrix, prop->matrix);
        prop->value = record->value;
    }
    prop->position = record->position;
    if (record->segment == 0xFFFF) {
        prop->segment = 0;
    } else {
        prop->segment = segments + record->segment * 32;
    }
    prop->key = next_key() << 10;
    prop->colorFrame = D_800B6D1B - 1;
}
