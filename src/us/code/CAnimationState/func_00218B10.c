/* FAKEMATCH (round 21 audit): 1 register pin (`register ... asm("$n")`); without them:
 * CAnimationState__GetRootMotion 31 of 343 words. */
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
#include "structs/game_object.h"
#include "globals/engine.h"
#include "protos/iset.h"
#include "protos/cache.h"
#include "structs/cache.h"

/* Switches an animation player to its queued animation. With `blend` set and a
 * different animation queued, it loads that one and, if `previous` is given,
 * leaves the old state there (frozen on the old animation) to blend from;
 * otherwise, or if the load fails, it reloads the current animation. */


typedef struct {
    u8 pad[2];
    u16 field2;
    u8 pad4[2];
    u16 frames;
} AnimHeader;

s32 CAnimationState__IsLoaded(CAnimationState *);
void *CAnimationState__GetData(CAnimationState *anim);
void Anim_Decompress(void);

typedef struct AngleKeys {
    /* 0x0 */ u8 header[8];
    /* 0x8 */ s16 keys[1];
} AngleKeys;

f32 Angle_Lerp(f32 frac, f32 a, f32 b);

/* The root yaw at `time`, interpolated between the two nearest keys (s16 angles, pi/32768 per unit). */
__inline__ f32 CAnimationState__GetRootYaw(CAnimationState *anim, f32 time) {
    s16 *keys;
    s32 first;
    s32 next;
    s32 last;

    keys = ((AngleKeys *) CISet__GetBlock(CAnimationState__GetData(anim), 0))->keys;
    last = anim->m_nFrames - 1;
    first = (s32) time;
    next = first + 1;
    if (next < last) {
        last = next;
    }
    return Angle_Lerp(time - (f32) first, (f32) keys[first] * 9.587673e-05f, (f32) keys[last] * 9.587673e-05f);
}

__inline__ s32 CAnimationState__Load(CAnimationState *player, sptr file, s32 base, s32 index) {
    s32 size;
    void *tree;

    if (index == -1) {
        player->pmeAnimData = NULL;
        return 0;
    }
    player->pmeAnimData = CCache__Request(0, CISet__GetROMAddressAndSize(file, base, index, &size), size, 4, player, Anim_Decompress, "animation", 0);
    if (!CAnimationState__IsLoaded(player)) {
        return 0;
    }
    tree = CAnimationState__GetData(player);
    /* block 0 is a CUSet (s32 element size, s32 count: the frames); the u16 view reads the
     * count's low half at +6, which on a little-endian host (the archive swaps the words)
     * is at +4 */
    player->m_nFrames = ((s32 *) CISet__GetBlock(tree, 0))[1];
    player->m_nAnimIndex = index;
    player->m_nAnimStartFrame = ((AnimHeader *) CISet__GetBlock(CISet__GetBlock(tree, 3), 0))->field2;
    return 1;
}

void CAnimationState__SwitchToQueued(CAnimationState *player, sptr file, s32 base, s32 blend, s32 *changed, CAnimationState *previous) {
    s32 current = player->m_nAnimIndex;
    s32 next = player->m_nRequestedAnimIndex;
    CAnimationState saved;

    if (next != current && blend) {
        player->m_bAnimActive = 1;
        saved = *player;
        if (CAnimationState__Load(player, file, base, next)) {
            if (previous != NULL) {
                *previous = saved;
                previous->m_nAnimIndex = current;
                previous->m_nRequestedAnimIndex = current;
                previous->m_bAnimActive = 1;
            }
            player->m_nAnimIndex = next;
            player->m_nRequestedAnimIndex = next;
            player->m_AnimationFrame = 0;
            player->m_bCycleCompleted = 0;
            player->m_bAnimActive = 1;
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



void CGameObjectInstance__FireAnimEvents(CGameObjectInstance *actor, CAnimationState *anim, f32 from, f32 to);
void CAnimationState__GetBonePosition(CAnimationState *anim, s32 bone, f32 time, CVector3 *out);
f32 Angle_Lerp(f32 frac, f32 a, f32 b);
f32 Angle_Difference(f32 a, f32 b);
void CVector3__Add(CVector3 *d, CVector3 *a, CVector3 *b);
void CVector3__Subtract(CVector3 *d, CVector3 *a, CVector3 *b);
s32 CAnimationState__GetEventCount(CAnimationState *anim);

void CAnimationState__GetRootMotion(CAnimationState *anim, CVector3 *delta, f32 *turn, CGameObjectInstance *actor) {
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
    if (actor->m_dwFlags & 0x8000000) {
        anim->m_AnimationFrame = (f32) (anim->m_nFrames - 1);
        CGameObjectInstance__FireAnimEvents(actor, anim, 0.0f, anim->m_AnimationFrame);
        actor->m_dwFlags = actor->m_dwFlags & ~0x8000000;
    }
    clip = CISet__GetBlock(CISet__GetBlock(header, 3), 0);
    CAnimationState__GetBonePosition(anim, 0, anim->m_AnimationFrame, &from);
    t = anim->m_AnimationFrame;
    prevAngle = CAnimationState__GetRootYaw(anim, t);
    t1 = anim->m_AnimationFrame;
    if (!(actor->m_dwFlags & 0x8000000)) {
        anim->m_AnimationFrame = t1 + gFrameIncrement * clip->rate * (1.0f / 15);
    }
    CGameObjectInstance__FireAnimEvents(actor, anim, t1, anim->m_AnimationFrame);
    last = anim->m_nFrames - 1;
    end = (f32) last;
    if (end < anim->m_AnimationFrame) {
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
        anim->m_AnimationFrame = (f32) loop + anim->m_AnimationFrame - end;
        if (end < anim->m_AnimationFrame) {
            anim->m_AnimationFrame = (f32) loop;
        }
        if (loop >= last && anim->m_nAnimIndex == anim->m_nRequestedAnimIndex && CAnimationState__GetEventCount(anim) == 0) {
            anim->m_bAnimActive = 0;
        }
        CGameObjectInstance__FireAnimEvents(actor, anim, (f32) loop, anim->m_AnimationFrame);
        CAnimationState__GetBonePosition(anim, 0, (f32) loop, &from);
        prevAngle = CAnimationState__GetRootYaw(anim, (f32) loop);
        anim->m_bCycleCompleted = 1;
    }
    CAnimationState__GetBonePosition(anim, 0, anim->m_AnimationFrame, &to);
    angle = anim->m_AnimationFrame;
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

void CAnimationState__FindKeys(CAnimationState *anim, CAnimationKeys *out) {
    s32 *file;
    u8 *posKeys;
    void *tracks;
    s32 loop;
    f32 time;
    f32 frames;
    f32 scale;
    s32 k0;
    s32 k1;

    if (!isLoaded(anim->pmeAnimData)) {
        return;
    }
    scale = 1.0f;
    file = anim->pmeAnimData->data;
    posKeys = CISet__GetBlock(file, 1);
    tracks = CISet__GetBlock(file, 5);
    CISet__GetBlock(tracks, 0);
    loop = anim->m_nAnimStartFrame;
    time = anim->m_AnimationFrame;
    frames = anim->m_nFrames;
    out->joints = (JointRef *) (posKeys + 8);
    out->frames = (DefaultFrame *) ((u8 *) CISet__GetBlock(file, 2) + 8);
    out->posTable = CISet__GetBlock(tracks, 1);
    out->rotTable = CISet__GetBlock(tracks, 2);
    out->blend = time - FLOOR(time);
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
            CEIL(k0, anim->m_nAnimStartFrame * scale)
        }
        k1 = k0 + 1;
        if (frames - 1.0f <= k1) {
            CEIL(k1, anim->m_nAnimStartFrame * scale)
        }
    }
    out->rotFrame0 = k0 * 4;
    out->rotFrame1 = k1 * 4;
    k0 = FLOOR(time);
    if (frames <= k0) {
        CEIL(k0, loop * scale)
    }
    k1 = k0 + 1;
    if (frames <= k1) {
        CEIL(k1, loop * scale)
    }
    out->posFrame0 = k0 * 4;
    out->posFrame1 = k1 * 4;
}


void CAnimationState__Reset(CAnimationState *player) {
    player->m_nAnimIndex = -1;
    player->m_nRequestedAnimIndex = 0;
    player->m_AnimationFrame = 0.0f;
    player->m_nFrames = 0;
    player->m_bCycleCompleted = 0;
    player->m_bAnimActive = 1;
    player->pmeAnimData = NULL;
}

void CAnimationState__Release(CAnimationState *player) {
    CCache__Unlock(0, player->pmeAnimData);
    player->pmeAnimData = NULL;
}

s32 CAnimationState__IsLoaded(CAnimationState *player) {
    void *data = player->pmeAnimData;

    if (data == NULL) {
        return 0;
    }
    return CMemEntry__IsLocked(data) != 0;
}

void *CAnimationState__GetKeyHeader(CAnimationState *player) {
    return CISet__GetBlock(CISet__GetBlock(CAnimationState__GetData(player), 3), 0);
}

void *CAnimationState__GetEvents(CAnimationState *player) {
    return (u8 *) CISet__GetBlock(CAnimationState__GetData(player), 4) + 8;
}

s32 CAnimationState__GetEventCount(CAnimationState *player) {
    return ((s32 *) CISet__GetBlock(CAnimationState__GetData(player), 4))[1];
}
