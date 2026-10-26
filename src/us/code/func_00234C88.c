#include "common.h"

typedef struct FrameAnim {
    /* 0x00 */ u8 pad0[0x2C];
    /* 0x2C */ f32 endFrame;
    /* 0x30 */ f32 frame;
    /* 0x34 */ u8 pad34[0x4];
    /* 0x38 */ s32 randomFrame;
} FrameAnim;

typedef struct FrameAnimDef {
    /* 0x00 */ u8 pad0[0x1E];
    /* 0x1E */ u8 mode;
} FrameAnimDef;

s32 func_002113A4(void);                /* random */

/* The frame index of an animation of `count` frames, by the definition's playback mode:
 * clamp (and remember the end), clamp, loop, ping-pong, random every time, random once. */
s32 func_00234C88(FrameAnim *anim, FrameAnimDef *def, s32 *count) {
    s32 frame;
    s32 *f;
    s32 period;
    s32 v;

    frame = anim->frame;
    f = &frame;
    switch (def->mode) {
    case 0:
        if (*f >= *count - 1) {
            *f = *count - 1;
            anim->endFrame = *count;
        }
        break;
    case 1:
        if (*f >= *count) {
            *f = *count - 1;
        }
        break;
    case 2:
        *f = *f % *count;
        break;
    case 3:
        period = *count * 2 - 2;
        v = period > 0 ? *f % period : 0;
        *f = v;
        if (v >= *count) {
            *f = *count * 2 - 2 - v;
        }
        break;
    case 4:
        *f = func_002113A4() % *count;
        break;
    case 5:
        if (anim->randomFrame == -1) {
            anim->randomFrame = func_002113A4() % *count;
        }
        *f = anim->randomFrame;
        break;
    }
    return frame;
}
