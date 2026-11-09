#include "common.h"
#include "globals/engine.h"
#include "globals/scene.h"
#include "protos/cache.h"

/* What this reads. */
typedef struct {
    /* 0x00 */ u8 pad0[0x80];
    /* 0x80 */ s32 unk80;
    /* 0x84 */ u8 pad84[4];
    /* 0x88 */ u16 flags;
    /* 0x8A */ u8 frame;            /* compared with (u8)gFrameCount */
} Drawable;


void CStaticObject__TouchLitVertices(Drawable *arg0) {
    if (!(arg0->flags & 0x40) && (arg0->frame != (u8)gFrameCount)) {
        CCache__Touch(0, arg0->unk80 | gLitVerticesKey);
    }
}
