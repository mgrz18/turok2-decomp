/* One source file, 0x219710-0x219BC0 (stray words at 0x219708 and 0x219BBC
 * bound the object): CAnimationState__SwitchToQueued, CAnimationState__GetRootMotion, CAnimationState__FindKeys and the
 * small animation-player helpers after it. Its .rodata opens at 0x800A5A10
 * with the "animation" string; CAnimationState__GetRootMotion's pool follows at 0x800A5A1C,
 * 4 past an 8-byte boundary, with its 2^32 doubles 8-aligned from the file's
 * start.
 *
 * CAnimationState__GetRootYaw and CAnimationState__Load are the out-of-line copies of two inline
 * functions defined near the top and inlined into CAnimationState__GetRootMotion and
 * CAnimationState__SwitchToQueued: GCC 2.8 defers a non-static inline function's own code to
 * the end of the file, which is where both sit in .text. That is also why
 * CAnimationState__Load names the same "animation" literal as CAnimationState__SwitchToQueued.
 * CAnimationState__GetRootMotion and CAnimationState__FindKeys are Rage Wars imports. */
#include "common.h"
#include "structs/vector.h"
#ifdef TARGET_PORT
#include "structs/game_object.h"
#endif
#include "globals/engine.h"
#include "protos/iset.h"
#include "protos/cache.h"

/* Switches an animation player to its queued animation. With `blend` set and a
 * different animation queued, it loads that one and, if `previous` is given,
 * leaves the old state there (frozen on the old animation) to blend from;
 * otherwise, or if the load fails, it reloads the current animation. */

typedef struct AnimPlayer {
    /* 0x00 */ f32 time;
    /* 0x04 */ s16 current;
    /* 0x06 */ s16 next;
    /* 0x08 */ s16 frames;
    /* 0x0A */ u8 active;
    /* 0x0B */ u8 dirty; /* blending */
    /* 0x0C */ s32 keys;
    /* 0x10 */ void *data;
} AnimPlayer;

typedef struct {
    u8 pad[2];
    u16 field2;
    u8 pad4[2];
    u16 frames;
} AnimHeader;

s32 CAnimationState__IsLoaded(AnimPlayer *);
void *CAnimationState__GetData(AnimPlayer *anim);
void Anim_Decompress(void);

typedef struct AngleKeys {
    /* 0x0 */ u8 header[8];
    /* 0x8 */ s16 keys[1];
} AngleKeys;

f32 Angle_Lerp(f32 frac, f32 a, f32 b);

/* The root yaw at `time`, interpolated between the two nearest keys (s16 angles, pi/32768 per unit). */
__inline__ f32 CAnimationState__GetRootYaw(AnimPlayer *anim, f32 time) {
    s16 *keys;
    s32 first;
    s32 next;
    s32 last;

    keys = ((AngleKeys *) CISet__GetBlock(CAnimationState__GetData(anim), 0))->keys;
    last = anim->frames - 1;
    first = (s32) time;
    next = first + 1;
    if (next < last) {
        last = next;
    }
    return Angle_Lerp(time - (f32) first, (f32) keys[first] * 9.587673e-05f, (f32) keys[last] * 9.587673e-05f);
}

__inline__ s32 CAnimationState__Load(AnimPlayer *player, sptr file, s32 base, s32 index) {
    s32 size;
    void *tree;

    if (index == -1) {
        player->data = NULL;
        return 0;
    }
    player->data = CCache__Request(0, CISet__GetROMAddressAndSize(file, base, index, &size), size, 4, player, Anim_Decompress, "animation", 0);
    if (!CAnimationState__IsLoaded(player)) {
        return 0;
    }
    tree = CAnimationState__GetData(player);
#ifdef TARGET_PORT
    /* block 0 is a CUSet (s32 element size, s32 count: the frames); the u16 view reads the
     * count's low half at +6, which on a little-endian host (the archive swaps the words)
     * is at +4 */
    player->frames = ((s32 *) CISet__GetBlock(tree, 0))[1];
#else
    player->frames = ((AnimHeader *) CISet__GetBlock(tree, 0))->frames;
#endif
    player->current = index;
    player->keys = ((AnimHeader *) CISet__GetBlock(CISet__GetBlock(tree, 3), 0))->field2;
    return 1;
}

void CAnimationState__SwitchToQueued(AnimPlayer *player, sptr file, s32 base, s32 blend, s32 *changed, AnimPlayer *previous) {
    s32 current = player->current;
    s32 next = player->next;
    AnimPlayer saved;

    if (next != current && blend) {
        player->dirty = 1;
        saved = *player;
        if (CAnimationState__Load(player, file, base, next)) {
            if (previous != NULL) {
                *previous = saved;
                previous->current = current;
                previous->next = current;
                previous->dirty = 1;
            }
            player->current = next;
            player->next = next;
            player->time = 0;
            player->active = 0;
            player->dirty = 1;
            if (changed != NULL) {
                *changed = 1;
            }
        } else {
            if (changed != NULL) {
                *changed = 0;
            }
            CAnimationState__Load(player, file, base, current);
        }
    } else {
        if (changed != NULL) {
            *changed = 0;
        }
        CAnimationState__Load(player, file, base, current);
    }
}

/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_0026195C.c, func_0026195C, with Turok 2's layouts. */

/* Advances an animation channel's root motion: moves the channel's time by
 * the clip rate, and returns in `delta` the root translation and in `turn`
 * the root yaw change since the last frame, wrapping to the clip's loop
 * start when time runs past the last frame. */

typedef struct RootClip {
    /* 0x0 */ u16 count;
    /* 0x2 */ u16 loopStart; /* 0xFFFF: none */
    /* 0x4 */ u8 pad4[4];
    /* 0x8 */ f32 rate;
} RootClip;

#ifdef TARGET_PORT
typedef struct RootActor {
/* the view at the host offsets of CGameObjectInstance (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_RootActor(m) __builtin_offsetof(CGameObjectInstance, m)
    union {
        struct { u8 _hp_flags[T2_HOFF_RootActor(m_dwFlags)]; u32 flags; }; /* N64 0xD4 */
    };
} RootActor;
#else
typedef struct RootActor {
    /* 0x00 */ u8 pad0[0xD4];
    /* 0xD4 */ u32 flags;
} RootActor;
#endif


void CGameObjectInstance__FireAnimEvents(RootActor *actor, AnimPlayer *anim, f32 from, f32 to);
void CAnimationState__GetBonePosition(AnimPlayer *anim, s32 bone, f32 time, CVector3 *out);
f32 Angle_Lerp(f32 frac, f32 a, f32 b);
f32 Angle_Difference(f32 a, f32 b);
void CVector3__Add(CVector3 *d, CVector3 *a, CVector3 *b);
void CVector3__Subtract(CVector3 *d, CVector3 *a, CVector3 *b);
s32 CAnimationState__GetEventCount(AnimPlayer *anim);

void CAnimationState__GetRootMotion(AnimPlayer *anim, CVector3 *delta, f32 *turn, RootActor *actor) {
    void *header;
    register RootClip *clip asm("$18");
    CVector3 from;
    CVector3 to;
    f32 prevAngle;
    f32 t;
    f32 t1;
    f32 end;
    f32 angle;
    u32 loop;
    u32 start;
    s32 last;

    header = CAnimationState__GetData(anim);
    delta->x = 0;
    delta->y = 0;
    delta->z = 0;
    *turn = 0;
    if (actor->flags & 0x8000000) {
        anim->time = (f32) (anim->frames - 1);
        CGameObjectInstance__FireAnimEvents(actor, anim, 0.0f, anim->time);
        actor->flags = actor->flags & ~0x8000000;
    }
    clip = CISet__GetBlock(CISet__GetBlock(header, 3), 0);
    CAnimationState__GetBonePosition(anim, 0, anim->time, &from);
    t = anim->time;
    prevAngle = CAnimationState__GetRootYaw(anim, t);
    t1 = anim->time;
    if (!(actor->flags & 0x8000000)) {
        anim->time = t1 + gFrameIncrement * clip->rate * (1.0f / 15);
    }
    CGameObjectInstance__FireAnimEvents(actor, anim, t1, anim->time);
    last = anim->frames - 1;
    end = (f32) last;
    if (end < anim->time) {
        CAnimationState__GetBonePosition(anim, 0, end, &to);
        angle = CAnimationState__GetRootYaw(anim, end);
        CVector3__Add(delta, delta, &to);
        CVector3__Subtract(delta, delta, &from);
        *turn += Angle_Difference(angle, prevAngle);
        start = clip->loopStart;
        loop = 0;
        if (start != 0xFFFF) {
            loop = start;
        }
        anim->time = (f32) loop + anim->time - end;
        if (end < anim->time) {
            anim->time = (f32) loop;
        }
        if (loop >= last && anim->current == anim->next && CAnimationState__GetEventCount(anim) == 0) {
            anim->dirty = 0;
        }
        CGameObjectInstance__FireAnimEvents(actor, anim, (f32) loop, anim->time);
        CAnimationState__GetBonePosition(anim, 0, (f32) loop, &from);
        prevAngle = CAnimationState__GetRootYaw(anim, (f32) loop);
        anim->active = 1;
    }
    CAnimationState__GetBonePosition(anim, 0, anim->time, &to);
    angle = anim->time;
    angle = CAnimationState__GetRootYaw(anim, angle);
    CVector3__Add(delta, delta, &to);
    CVector3__Subtract(delta, delta, &from);
    *turn += Angle_Difference(angle, prevAngle);
}

/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_00261EB8.c, func_00261EB8, with Turok 2's layouts. */

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

s32 CMemEntry__IsLocked(s32 **resource);

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
    return CMemEntry__IsLocked(resource) != 0;
}

void CAnimationState__FindKeys(AnimChannel *anim, KeyLookup *out) {
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
    posKeys = CISet__GetBlock(file, 1);
    tracks = CISet__GetBlock(file, 5);
    CISet__GetBlock(tracks, 0);
    loop = anim->loopStart;
    time = anim->time;
    frames = anim->frames;
    out->posKeys = posKeys + 8;
    out->rotKeys = (u8 *) CISet__GetBlock(file, 2) + 8;
    out->posTracks = CISet__GetBlock(tracks, 1);
    out->rotTracks = CISet__GetBlock(tracks, 2);
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


void CAnimationState__Reset(AnimPlayer *player) {
    player->current = -1;
    player->next = 0;
    player->time = 0.0f;
    player->frames = 0;
    player->active = 0;
    player->dirty = 1;
    player->data = NULL;
}

void CAnimationState__Release(AnimPlayer *player) {
    CCache__Unlock(0, player->data);
    player->data = NULL;
}

s32 CAnimationState__IsLoaded(AnimPlayer *player) {
    void *data = player->data;

    if (data == NULL) {
        return 0;
    }
    return CMemEntry__IsLocked(data) != 0;
}

void *CAnimationState__GetKeyHeader(AnimPlayer *player) {
    return CISet__GetBlock(CISet__GetBlock(CAnimationState__GetData(player), 3), 0);
}

void *CAnimationState__GetEvents(AnimPlayer *player) {
    return (u8 *) CISet__GetBlock(CAnimationState__GetData(player), 4) + 8;
}

s32 CAnimationState__GetEventCount(AnimPlayer *player) {
    return ((s32 *) CISet__GetBlock(CAnimationState__GetData(player), 4))[1];
}
