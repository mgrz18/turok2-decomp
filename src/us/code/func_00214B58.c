/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/WORLD_2/8024E914_14.CPP, func_80250624_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/vector.h"
#include "protos/iset.h"
#include "protos/cache.h"

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
#ifdef TARGET_PORT /* m_pOwner, a pointer */
    void **resource;
#else
    /* 0x24 */ s32 resource;
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
T2_STATIC_OBJECT_CHECK(PlacedProp, matrix, flags);
#endif

void CMtxF__FromMtxAffine(f32 *, f32 *);
void CMtxF__TransformPoint(f32 *, CVector3 *, CVector3 *);
void ImpactEvent_Send(void *, void *, s32, s32, CVector3, AttachmentParams);

extern u8 D_800A5808[];

void CStaticObject__SpawnEvents(PlacedProp *prop, s32 all) {
    s32 *model;
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

    model = CCache__Request(0, prop->owner, 0x18, 4, 0, 0, D_800A5808, 0);
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
