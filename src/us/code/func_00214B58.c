/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/WORLD_2/8024E914_14.CPP, func_80250624_de, with Turok 2's layouts. */
#include "common.h"

typedef struct {
    f32 x, y, z;
} Vec3;

typedef struct {
    s32 words[2];
} AttachmentParams;

typedef struct {
    /* 0x00 */ u16 field0;
    /* 0x02 */ u16 type;
    /* 0x04 */ s32 field4;
    /* 0x08 */ Vec3 offset;
    /* 0x14 */ AttachmentParams params;
} Attachment; /* size 0x1C */

typedef struct {
    /* 0x00 */ u8 state;
    /* 0x02 */ u16 id;
    /* 0x04 */ Vec3 position;
    /* 0x10 */ void *segment;
    /* 0x14 */ void *model;
    /* 0x18 */ s32 owner;
    /* 0x1C */ s32 field1C;
    /* 0x20 */ s32 field20;
    /* 0x24 */ s32 resource;
    /* 0x28 */ f32 matrix[16];
    /* 0x68 */ f32 min[3];
    /* 0x74 */ f32 max[3];
    /* 0x80 */ s32 key;
    /* 0x84 */ s32 value;
    /* 0x88 */ u16 flags;
    /* 0x8A */ u8 colorFrame;
} PlacedProp;

void *func_0020367C(s32, s32, s32, s32, s32, void *, void *, s32);
s32 func_0020185C(s32, s32, s32, s32 *);
void func_002051F4(s32, void *);
void func_0020D908(f32 *, f32 *);
void func_0020F85C(f32 *, Vec3 *, Vec3 *);
void func_00236EA0(void *, void *, s32, s32, Vec3, AttachmentParams);

extern u8 D_800A5808[];
extern u8 D_800A581C[];

void func_00214B58(PlacedProp *prop, s32 all) {
    s32 *model;
    s32 **table;
    s32 *header;
    Attachment *attachment;
    Attachment *entries;
    f32 matrix[16];
    Vec3 position;
    s32 size;
    s32 name;
    s32 count;
    s32 i;
    s32 first;
    f32 *transform;

    model = func_0020367C(0, prop->owner, 0x18, 4, 0, 0, D_800A5808, 0);
    if (model == NULL) {
        return;
    }
    name = func_0020185C(*model, prop->owner, 3, &size);
    table = func_0020367C(0, name, size, 0, 0, 0, D_800A581C, 0);
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
                        func_0020D908(matrix, prop->matrix);
                    }
                    transform = matrix;
                }
                func_0020F85C(transform, &attachment->offset, &position);
                func_00236EA0(prop, prop, attachment->type, -1, position, attachment->params);
            }
        }
        func_002051F4(0, table);
    }
    func_002051F4(0, model);
}
