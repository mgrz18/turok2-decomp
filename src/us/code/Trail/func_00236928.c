/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802A5AD0_de_2.c, func_802A6068_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"

/* A texture image as Sprite_FromImage reads it (Rage Wars' Image, rw/abu_types.h). */
typedef struct ImageHeader {
    /* 0x00 */ char pad0[2];
    /* 0x02 */ u8 widthShift;
    /* 0x03 */ u8 heightShift;
    /* 0x04 */ char pad4[0x1D - 4];
    /* 0x1D */ u8 scale;
    /* 0x1E */ char pad1E[0x21 - 0x1E];
    /* 0x21 */ u8 width;
    /* 0x22 */ u8 height;
    /* 0x23 */ u8 format;
    /* 0x24 */ char data[1];
} ImageHeader;
typedef struct Image {
    /* 0x00 */ char pad0[8];
    /* 0x08 */ ImageHeader *header;
    /* 0x0C */ s32 fieldC;
    /* 0x10 */ s32 field10;
    /* 0x14 */ char pad14[0x3C - 0x14];
    /* 0x3C */ s32 flags;
} Image;
typedef struct Sprite {
    /* 0x00 */ s32 format;
    /* 0x04 */ char *data;
    /* 0x08 */ s32 field8;
    /* 0x0C */ s32 fieldC;
    /* 0x10 */ f32 width;
    /* 0x14 */ f32 height;
    /* 0x18 */ f32 extentX;
    /* 0x1C */ f32 extentY;
    /* 0x20 */ f32 step;
} Sprite;

extern float D_800A652C;
void Sprite_FromImage(Image *image, Image *texture, Sprite *sprite) {
    f32 scale;
    s32 height;
    f32 *unit;
    sprite->data = image->header->data;
    sprite->format = image->header->format;
    if (image->flags & 2) {
        sprite->field8 = image->fieldC;
        sprite->fieldC = image->field10;
    } else {
        sprite->field8 = image->fieldC;
        sprite->fieldC = image->fieldC;
    }
    scale = 3.125f;
    sprite->width = image->header->width * scale;
    sprite->height = image->header->height * scale;
    if (texture != 0) {
        sprite->extentX = 1 << (texture->header->widthShift + 5);
        sprite->extentX = sprite->extentX * image->header->scale - D_800A652C;
        height = 1 << (texture->header->heightShift + 5);
        unit = &D_800A652C + 1;
        sprite->step = *unit / ((f32)sprite->format - *unit);
        sprite->extentY = height;
    }
}
