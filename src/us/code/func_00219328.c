/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_00261EB8.c, func_00261EB8, with Turok 2's layouts. */
#include "common.h"

/* Finds the two keyframes an animation channel's time falls between: the
 * key tables of the channel's animation, the byte offsets of the keys
 * before and after `time` (wrapping to the loop start past the last frame)
 * and the fraction between them. */

typedef struct AnimChannel {
    /* 0x00 */ f32 time;
    /* 0x04 */ s16 current;
    /* 0x06 */ s16 next;
    /* 0x08 */ s16 frames;
    /* 0x0A */ u8 active;
    /* 0x0B */ u8 blending;
    /* 0x0C */ u32 loopStart; /* 0xFFFF: none */
    /* 0x10 */ s32 **resource;
} AnimChannel;

typedef struct KeyLookup {
    /* 0x00 */ void *posKeys;
    /* 0x04 */ void *rotKeys;
    /* 0x08 */ void *posTracks;
    /* 0x0C */ void *rotTracks;
    /* 0x10 */ s32 offset0;
    /* 0x14 */ s32 offset1;
    /* 0x18 */ s32 rootOffset0;
    /* 0x1C */ s32 rootOffset1;
    /* 0x20 */ f32 frac;
} KeyLookup;

void *func_002017D4(void *file, s32 index);
s32 func_00204D90(s32 **resource);

#define FLOOR(x) ((x) >= 0.0f ? (s32) (x) : (s32) (x) - ((f32) (s32) (x) == (x) ? 0 : 1))
#define CEIL(r, x)                              \
    if ((x) <= 0.0f) {                          \
        r = (s32) (x);                          \
    } else {                                    \
        r = (f32) (s32) (x) == (x) ? 0 : 1;     \
        r += (s32) (x);                         \
    }

static __inline__ s32 isLoaded(s32 **resource) {
    if (resource == NULL) {
        return 0;
    }
    return func_00204D90(resource) != 0;
}

void func_00219328(AnimChannel *anim, KeyLookup *out) {
    s32 *file;
    u8 *posKeys;
    void *tracks;
    s32 loop;
    f32 time;
    f32 frames;
    f32 scale;
    s32 k0;
    s32 k1;

    if (!isLoaded(anim->resource)) {
        return;
    }
    scale = 1.0f;
    file = *anim->resource;
    posKeys = func_002017D4(file, 1);
    tracks = func_002017D4(file, 5);
    func_002017D4(tracks, 0);
    loop = anim->loopStart;
    time = anim->time;
    frames = anim->frames;
    out->posKeys = posKeys + 8;
    out->rotKeys = (u8 *) func_002017D4(file, 2) + 8;
    out->posTracks = func_002017D4(tracks, 1);
    out->rotTracks = func_002017D4(tracks, 2);
    out->frac = time - FLOOR(time);
    if (loop == 0xFFFF) {
        loop = 0;
        k0 = FLOOR(time);
        if (frames <= k0) {
            k0 = 0;
        }
        k1 = k0 + 1;
        if (frames <= k1) {
            k1 = 0;
        }
    } else {
        k0 = FLOOR(time);
        if (frames - 1.0f <= k0) {
            CEIL(k0, anim->loopStart * scale)
        }
        k1 = k0 + 1;
        if (frames - 1.0f <= k1) {
            CEIL(k1, anim->loopStart * scale)
        }
    }
    out->offset0 = k0 * 4;
    out->offset1 = k1 * 4;
    k0 = FLOOR(time);
    if (frames <= k0) {
        CEIL(k0, loop * scale)
    }
    k1 = k0 + 1;
    if (frames <= k1) {
        CEIL(k1, loop * scale)
    }
    out->rootOffset0 = k0 * 4;
    out->rootOffset1 = k1 * 4;
}
