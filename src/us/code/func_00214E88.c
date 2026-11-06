#include "common.h"

/* What this reads. */
typedef struct {
    /* 0x00 */ u8 pad0[0x80];
    /* 0x80 */ s32 unk80;
    /* 0x84 */ u8 pad84[4];
    /* 0x88 */ u16 flags;
    /* 0x8A */ u8 frame;            /* compared with D_800B6D1B */
} Drawable;

void CCache__Touch(s32, s32);                       /* extern */
extern s32 gLitVerticesKey;
extern u8 D_800B6D1B;

void CStaticObject__TouchLitVertices(Drawable *arg0) {
    if (!(arg0->flags & 0x40) && (arg0->frame != D_800B6D1B)) {
        CCache__Touch(0, arg0->unk80 | gLitVerticesKey);
    }
}
