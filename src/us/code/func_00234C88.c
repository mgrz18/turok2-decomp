#include "common.h"

#include "structs/swoosh.h"

s32 Random_Next(void);                /* random */

/* The frame index of an animation of `count` frames, by the definition's playback mode:
 * clamp (and remember the end), clamp, loop, ping-pong, random every time, random once. */
s32 Trail_GetAnimFrame(Swoosh *anim, SwooshInfo *def, s32 *count) {
    s32 frame;
    s32 *f;
    s32 period;
    s32 v;

    frame = anim->frame;
    f = &frame;
    switch (def->animMode) {
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
        *f = Random_Next() % *count;
        break;
    case 5:
        if (anim->randomFrame == -1) {
            anim->randomFrame = Random_Next() % *count;
        }
        *f = anim->randomFrame;
        break;
    }
    return frame;
}
