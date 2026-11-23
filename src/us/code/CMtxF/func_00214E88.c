#include "common.h"
#include "globals/engine.h"
#include "globals/scene.h"
#include "protos/cache.h"

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
