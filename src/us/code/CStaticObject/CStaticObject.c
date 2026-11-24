/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/FX/8024F950.CPP, func_8024FD80_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/vector.h"
#include "globals/scene.h"
#include "globals/camera.h"
#include "globals/engine.h"
#include "protos/scene.h"

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
} CompactPropRecord;
typedef struct {
    /* 0x00 */ u8 state;
    /* 0x01 */ u8 pad1;
    /* 0x02 */ u16 id;
    /* 0x04 */ CVector3 pos;
    /* 0x10 */ void *segment;
    /* 0x14 */ void *model;
    /* 0x18 */ s32 owner;
    /* 0x1C */ s32 unk1C;
    s32 pad20;
    void **pad24;
    /* 0x28 */ f32 matrix[16];
    /* 0x68 */ CVector3 min;
    /* 0x74 */ CVector3 max;
    /* 0x80 */ s32 colorKey;
    /* 0x84 */ s32 value;
    /* 0x88 */ u16 flags;
    /* 0x8A */ u8 colorFrame;
} CompactProp;
#ifdef TARGET_PORT
#include "structs/level_actor.h"
#include "structs/region.h"
T2_STATIC_OBJECT_CHECK(CompactProp, matrix, flags);
#endif

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

void CStaticObject__ConstructCompact(CompactProp *prop, CompactPropRecord *record, s32 owner, u8 *segments, f32 scale) {
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
#ifdef TARGET_PORT /* the region array (section 2 of the region data) at the host CGameRegion stride: at
                    * 32 bytes a static object's region was a wrong address (pointer-wide host regions), and
                    * its first event (a particle traced from that region) crashed in Collision_SweepRegions */
        prop->segment = (void *) ((CGameRegion *) segments + record->segment);
#else
        prop->segment = segments + record->segment * 32;
#endif
    }
    prop->colorKey = nextColorKey() << 10;
    prop->colorFrame = (u8)gFrameCount - 1;
}

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
    void **field24;
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

#ifndef TARGET_PORT
/* The cache names (globals/strings.h) CStaticObject__Draw, __SpawnEvents, __RequestLitVertices and
 * __LoadGeometry read by name: in the ROM's .rodata after CStaticObject__Construct's literals, so
 * defined here (the port keeps the generated copies). */
const char sCStaticObjectCacheDesc_LitVertices_def[] __asm__("sCStaticObjectCacheDesc_LitVertices") = "lit vertices";
const char sCStaticObjectCacheDesc_StatObjectIndex_def[] __asm__("sCStaticObjectCacheDesc_StatObjectIndex") = "stat object index";
#endif

#include "globals/strings.h"
#include "structs/game.h"
#include "globals/game.h"
#include "protos/iset.h"
#include "protos/cache.h"

typedef struct {
    /* 0x00 */ u8 pad0[0xE];
    /* 0x0E */ s8 argument;
    /* 0x0F */ u8 padF[3];
    /* 0x12 */ s8 lod;
} PropDescriptor;
typedef struct {
#ifdef TARGET_PORT /* CStaticObject's host layout (structs/level_actor.h) */
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ PropDescriptor *descriptor;
    /* 0x20 */ u8 pad20[0xC];
    void **cache;
#else
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ PropDescriptor *descriptor;
    /* 0x18 */ u8 pad18[0xC];
    /* 0x24 */ s32 cache;
#endif
    /* 0x28 */ f32 matrix[16];
    /* 0x68 */ u8 pad68[0x18];
    /* 0x80 */ s32 colorKey;
    /* 0x84 */ s32 value;
    /* 0x88 */ u16 flags;
    /* 0x8A */ u8 colorFrame;
} DrawnProp;
#ifdef TARGET_PORT
#include "structs/level_actor.h"
T2_STATIC_OBJECT_CHECK(DrawnProp, matrix, flags);
_Static_assert(__builtin_offsetof(DrawnProp, cache) == __builtin_offsetof(CStaticObject, m_pOwner), "cache");
_Static_assert(__builtin_offsetof(DrawnProp, colorKey) == __builtin_offsetof(CStaticObject, m_LitKey), "colorKey");
#endif

/* A vertex (0x10 bytes). */
typedef struct {
    /* 0x0 */ s16 pos[3];
    /* 0x6 */ u16 flag;
    /* 0x8 */ s16 tc[2];
    /* 0xC */ u8 r, g, b, a;
} ColVtx;
typedef struct {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ s32 count;
    /* 0x8 */ ColVtx v[1];
} VtxChunk;
/* What this reads around gEngineApp_m_Settings_match. */


s32 Actor_GetTypeClass();
s32 *CStaticObject__LoadGeometry(void *arg0);
void CStaticObject__BuildLitVerticesCallback(void *arg0, s32 **arg1);
void Render_QueueModelInstance(void **resource, void *matrix, s32 segment, void *lights, void *textures, s32 lod, s32 pass);

/* The colour pulse (Rage Wars' func_8027892C_de): every vertex flagged 0x70
 * gets its base colour from colors plus a step of the pulse table. */
static __inline__ void pulseColors(s32 *object, u8 *colors) {
    s32 state;
    s32 i;
    s32 j;
    s32 count;
    VtxChunk *chunk;
    ColVtx *record;
    ColVtx *end;

    state = gFrameCount;
    i = 0;
    if (!(state & 1)) {
        count = *object;
        for (j = 0; j < count; j++) {
            chunk = CISet__GetBlock(object, j);
            record = chunk->v;
            end = record + chunk->count;
            if (record != end) {
                do {
                    u16 flags = record->flag;
                    s32 amount;
                    s32 value;
                    u8 *color;

                    if (flags & 0x70) {
                        amount = gColorPulseTable[(state + ((flags & 0x70) >> 2)) & 0x3F];
                        amount >>= (flags & 0xC) >> 2;
                        color = colors + i * 4;

                        value = amount + color[0];
                        if (value >= 0x100) value = 0xFF;
                        if (value < 0) value = 0;
                        record->r = value;

                        value = amount + color[1];
                        if (value >= 0x100) value = 0xFF;
                        if (value < 0) value = 0;
                        record->g = value;

                        value = amount + color[2];
                        if (value >= 0x100) value = 0xFF;
                        if (value < 0) value = 0;
                        record->b = value;
                    }
                    record++;
                    i++;
                } while (record != end);
            }
        }
    }
}

/* Queues a placed prop's model: the level of detail from its descriptor (in
 * multiplayer, from the game setup for some modes), and its vertex colours
 * pulsed once a frame. */
/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS Actor_GetTypeClass receives CStaticObject__Draw's own argument
 * registers (pass-through); the host passes them. */
void CStaticObject__Draw(DrawnProp *prop) {
    s32 argument;
    s32 mode;
    void **instance;
    s32 lod;
    s32 **resource;
    s32 *header;
    s32 *object;
    u8 *colors;
    void *data;

    { /* a block of its own, as an early-out macro would make; it decides the register allocation */
        if (prop->flags & 0x40) {
            return;
        }
    }
    argument = prop->descriptor->argument;
    if (gEngineApp_m_Settings.multiplayer) {
        mode = Actor_GetTypeClass(prop);
        switch (mode) {
            case 1:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
                lod = prop->descriptor->lod;
                break;
            default: {
                GameMatch *game = &gEngineApp_m_Settings.match;
                if (game->tag) {
                    lod = ((s8 *) game)[-0x1B0];
                } else {
                    lod = ((s8 *) game)[-0x1B5];
                }
                break;
            }
        }
    } else {
        lod = prop->descriptor->lod;
    }
    instance = (void **) CStaticObject__LoadGeometry(prop);
    if (instance == NULL) {
        return;
    }
    resource = CCache__RequestDecompression(0, prop->colorKey | gLitVerticesKey, instance, prop->cache, 8, prop, CStaticObject__BuildLitVerticesCallback, sCStaticObjectCacheDesc_LitVertices, 0);
    if (resource != NULL) {
        CCache__LockForDisplayList(0, resource);
        header = *resource;
        if (header[0] == 1) {
            data = (u8 *) header + header[1];
            object = (s32 *) data;
            colors = (u8 *) header + header[2];
            if (prop->colorFrame != (u8)gFrameCount) {
                pulseColors(object, colors);
            }
            Render_QueueModelInstance(instance, prop->matrix, 0, 0, data, argument, lod);
        } else {
            Render_QueueModelInstance(instance, prop->matrix, 0, 0, header + 2, argument, lod);
        }
        CCache__Unlock(0, resource);
    }
    CCache__Unlock(0, instance);
    prop->colorFrame = (u8)gFrameCount;
}

typedef struct {
    s32 words[2];
} AttachmentParams;
typedef struct {
    /* 0x00 */ u16 field0;
    /* 0x02 */ u16 type;
    /* 0x04 */ s32 field4;
    /* 0x08 */ CVector3 offset;
    /* 0x14 */ AttachmentParams params;
} Attachment; /* size 0x1C */
typedef struct {
    /* 0x00 */ u8 state;
    /* 0x02 */ u16 id;
    /* 0x04 */ CVector3 position;
    /* 0x10 */ void *segment;
    /* 0x14 */ void *model;
    /* 0x18 */ s32 owner;
    /* 0x1C */ s32 field1C;
    /* 0x20 */ s32 field20;
    void **resource;
    /* 0x28 */ f32 matrix[16];
    /* 0x68 */ f32 min[3];
    /* 0x74 */ f32 max[3];
    /* 0x80 */ s32 key;
    /* 0x84 */ s32 value;
    /* 0x88 */ u16 flags;
    /* 0x8A */ u8 colorFrame;
} EventProp;
#ifdef TARGET_PORT
#include "structs/level_actor.h"
T2_STATIC_OBJECT_CHECK(EventProp, matrix, flags);
#endif

void CMtxF__FromMtxAffine(f32 *, f32 *);
void CMtxF__TransformPoint(f32 *, CVector3 *, CVector3 *);
void ImpactEvent_Send(void *, void *, s32, s32, CVector3, AttachmentParams);

void CStaticObject__SpawnEvents(EventProp *prop, s32 all) {
    sptr *model;
    s32 **table;
    s32 *header;
    Attachment *attachment;
    Attachment *entries;
    f32 matrix[16];
    CVector3 position;
    s32 size;
    sptr name;
    s32 count;
    s32 i;
    s32 first;
    f32 *transform;

    model = CCache__Request(0, prop->owner, 0x18, 4, 0, 0, sCStaticObjectCacheDesc_StatObjectIndex, 0);
    if (model == NULL) {
        return;
    }
    name = CISet__GetROMAddressAndSize(*model, prop->owner, 3, &size);
    table = CCache__Request(0, name, size, 0, 0, 0, "static events", 0);
    if (table != NULL) {
        header = *table;
        count = header[1];
        entries = (Attachment *) (header + 2);
        first = 1;
        for (i = 0; i < count; i++) {
            attachment = &entries[i];
            if (all != 0 || attachment->type == 8) {
                if (prop->flags & 0x40) {
                    transform = prop->matrix;
                } else {
                    if (first) {
                        first = 0;
                        CMtxF__FromMtxAffine(matrix, prop->matrix);
                    }
                    transform = matrix;
                }
                CMtxF__TransformPoint(transform, &attachment->offset, &position);
                ImpactEvent_Send(prop, prop, attachment->type, -1, position, attachment->params);
            }
        }
        CCache__Unlock(0, table);
    }
    CCache__Unlock(0, model);
}

/* The record this spawns from: an id at 0x24, extra spawn flags at 0x80 and its own flags at 0x88 (0x40: disabled). */
#ifdef TARGET_PORT /* CStaticObject's host layout (structs/level_actor.h): id = m_pOwner */
#include "structs/level_actor.h"
typedef struct {
    u8 pad0[0x30];
    void **id;
    f32 pad28[16];
    u8 pad68[0x18];
    s32 spawnFlags;
    u8 pad84[4];
    u16 flags;
} SpawnSource;
T2_STATIC_OBJECT_CHECK(SpawnSource, pad28, flags);
_Static_assert(__builtin_offsetof(SpawnSource, id) == __builtin_offsetof(CStaticObject, m_pOwner), "id");
_Static_assert(__builtin_offsetof(SpawnSource, spawnFlags) == __builtin_offsetof(CStaticObject, m_LitKey), "spawnFlags");
#else
typedef struct {
    /* 0x00 */ u8 pad0[0x24];
    /* 0x24 */ s32 id;
    /* 0x28 */ u8 pad28[0x80 - 0x28];
    /* 0x80 */ s32 spawnFlags;
    /* 0x84 */ u8 pad84[4];
    /* 0x88 */ u16 flags;
} SpawnSource;
#endif

s32 Model_RequestTextures(sptr);                         /* extern */

/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CStaticObject__LoadGeometry receives CStaticObject__RequestLitVertices's own argument
 * registers (pass-through); the host passes them. */
void CStaticObject__RequestLitVertices(SpawnSource *arg0) {
    sptr temp_v0;
    sptr temp_v0_2;

    if (!(arg0->flags & 0x40)) {
        temp_v0 = (sptr) CStaticObject__LoadGeometry(arg0);
        if (temp_v0 != 0) {
            Model_RequestTextures(temp_v0);
            temp_v0_2 = CCache__RequestDecompression(0, arg0->spawnFlags | gLitVerticesKey, temp_v0, arg0->id, 8, arg0, &CStaticObject__BuildLitVerticesCallback, &sCStaticObjectCacheDesc_LitVertices, 0);
            if (temp_v0_2 != 0) {
                CCache__Unlock(0, temp_v0_2);
            }
            CCache__Unlock(0, temp_v0);
        }
    }
}

#include "m2c_macros.h"

void Model_DecompressCallback(void);
#include "structs/level_actor.h"

s32 *CStaticObject__LoadGeometry(void *arg0) {
    CStaticObject *o = arg0;
    s32 *temp_v0;

    if (o->m_GeometryAddr == 0) {
        temp_v0 = (s32 *) CCache__Request(0, o->m_ObjectAddr, 0x18, 4, 0, NULL, (char *) &sCStaticObjectCacheDesc_StatObjectIndex, 0);
        if (temp_v0 != NULL) {
            o->m_GeometryAddr = CISet__GetROMAddressAndSize(*(void **) temp_v0, o->m_ObjectAddr, 1, &o->m_GeometrySize);
            CCache__Unlock(0, (void *) temp_v0);
        }
        if (o->m_GeometryAddr == 0) {
            return NULL;
        }
    }
    return (s32 *) CCache__Request(0, o->m_GeometryAddr, o->m_GeometrySize, 8, 0, &Model_DecompressCallback, (char *) &sCStaticObjectCacheDesc_StatGeometry, 0);
}

/* What this reads. */
#ifdef TARGET_PORT /* CStaticObject's host layout (structs/level_actor.h) */
#include "structs/level_actor.h"
typedef struct {
    u8 pad0[__builtin_offsetof(CStaticObject, m_LitKey)];
    s32 unk80;
    u8 pad84[4];
    u16 flags;
    u8 frame;
} Drawable;
_Static_assert(__builtin_offsetof(Drawable, flags) == __builtin_offsetof(CStaticObject, m_wFlags), "flags");
_Static_assert(__builtin_offsetof(Drawable, frame) == __builtin_offsetof(CStaticObject, m_LitFrame), "frame");
#else
typedef struct {
    /* 0x00 */ u8 pad0[0x80];
    /* 0x80 */ s32 unk80;
    /* 0x84 */ u8 pad84[4];
    /* 0x88 */ u16 flags;
    /* 0x8A */ u8 frame;            /* compared with (u8)gFrameCount */
} Drawable;
#endif

void CStaticObject__TouchLitVertices(Drawable *arg0) {
    if (!(arg0->flags & 0x40) && (arg0->frame != (u8)gFrameCount)) {
        CCache__Touch(0, arg0->unk80 | gLitVerticesKey);
    }
}

s32 Instance_NextUniqueId(void) {
    s32 temp_v0;

    temp_v0 = gNextUniqueId + 1;
    gNextUniqueId = temp_v0;
    if (temp_v0 == 0x3FFFFF) {
        gNextUniqueId = 0x380000;
    }
    return gNextUniqueId;
}

#ifdef TARGET_PORT
#include "structs/level_actor.h"
#include "structs/cache.h"
#endif

sptr Lighting_BuildLitVertices(sptr, sptr, sptr, sptr, s32);

void CStaticObject__BuildLitVerticesCallback(void *arg0, s32 **arg1)
{
#ifdef TARGET_PORT /* the static object's members (structs/level_actor.h); arg1 is the CCacheEntry */
  sptr new_var = (sptr) ((CCacheEntry *) arg1)->memEntry->data;
  CStaticObject *o = arg0;
  CCache__ReleaseAndReplaceData(0, arg1, Lighting_BuildLitVertices(new_var, (sptr) *o->m_pOwner, (sptr) &o->m_Bounds, (sptr) o->m_Matrix, 0));
#else
  s32 new_var;
  new_var = *(*arg1);
  CCache__ReleaseAndReplaceData(0, arg1, Lighting_BuildLitVertices(new_var, *(*((s32 **) (((s8 *) arg0) + 0x24))), (sptr) (arg0 + 0x68), (sptr) (arg0 + 0x28), 0));
#endif
}

#include "structs/vertex_group.h"

/* Pulses the vertex colours of a model whose vertices carry a pulse phase
 * (flag bits 0x70) and amplitude shift (bits 0xC), from their saved base colours. */




void Model_PulseVertexColors(s32 *object, u8 *colors) {
    s32 state = gFrameCount;
    s32 i = 0;
    s32 g, count;

    if (!(state & 1)) {
        count = object[0];
        for (g = 0; g < count; g++) {
            VertexGroup *group = CISet__GetBlock(object, g);
            Vtx_t *v = &group->verts[0].v;
            Vtx_t *end = v + group->count;

            while (v != end) {
                u16 flags = v->flag;
                s32 amount, value;
                u8 *color;

                if (flags & 0x70) {
                    amount = gColorPulseTable[(state + ((flags & 0x70) >> 2)) & 0x3F];
                    amount >>= (flags & 0xC) >> 2;
                    color = colors + i * 4;

                    value = amount + color[0];
                    if (value >= 0x100) value = 0xFF;
                    if (value < 0) value = 0;
                    v->cn[0] = value;

                    value = amount + color[1];
                    if (value >= 0x100) value = 0xFF;
                    if (value < 0) value = 0;
                    v->cn[1] = value;

                    value = amount + color[2];
                    if (value >= 0x100) value = 0xFF;
                    if (value < 0) value = 0;
                    v->cn[2] = value;
                }
                v++;
                i++;
            }
        }
    }
}

#ifndef TARGET_PORT
/* The last bytes of the object's .rodata (CStaticObject__LoadGeometry's cache name); stale linker
 * bytes follow up to the next object (versions/ld_fill.us.txt). */
const char sCStaticObjectCacheDesc_StatGeometry_def[] __asm__("sCStaticObjectCacheDesc_StatGeometry") = "stat geometry";
#endif
