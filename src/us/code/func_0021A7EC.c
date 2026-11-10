/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/particle/80246E34_2.CPP, func_802472F0_de, with Turok 2's layouts. */

#include "common.h"
#include "structs/vector.h"
#ifdef TARGET_PORT
#include "structs/game_object.h"
#endif
#include "globals/scene.h"
#include "globals/engine.h"
#include "protos/cache.h"

typedef struct {
    f32 m[16];
} MatrixF;

/* An animation track (0x14 bytes). */
typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s16 current;
    /* 0x06 */ u8 pad6[4];
    /* 0x0A */ s8 blending;
    /* 0x0B */ s8 active;
    /* 0x0C */ u8 padC[8];
} AnimTrack;

typedef struct {
    /* 0x0 */ u16 frames;
    /* 0x2 */ u8 pad2[3];
    /* 0x5 */ u8 start;
    /* 0x6 */ u8 pad6;
    /* 0x7 */ u8 end;
} AnimBlendEntry;

#ifdef TARGET_PORT
typedef struct AnimActor {
/* the view at the host offsets of CGameObjectInstance (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_AnimActor(m) __builtin_offsetof(CGameObjectInstance, m)
    union {
        struct { u8 _hp_position[T2_HOFF_AnimActor(header.m_vPos)]; CVector3 position; }; /* N64 0x4 */
        struct { u8 _hp_scaleX[T2_HOFF_AnimActor(m_vScale.x)]; f32 scaleX; }; /* N64 0x34 */
        struct { u8 _hp_scaleY[T2_HOFF_AnimActor(m_vScale.y)]; f32 scaleY; }; /* N64 0x38 */
        struct { u8 _hp_scaleZ[T2_HOFF_AnimActor(m_vScale.z)]; f32 scaleZ; }; /* N64 0x3C */
        struct { u8 _hp_heading[T2_HOFF_AnimActor(m_RotY)]; f32 heading; }; /* N64 0x50 */
        struct { u8 _hp_animationKey[T2_HOFF_AnimActor(m_rpAnims)]; sptr animationKey; }; /* N64 0xA8: a ROM address */
        struct { u8 _hp_type[T2_HOFF_AnimActor(m_nTypeFlag)]; u16 type; }; /* N64 0xB8 */
        struct { u8 _hp_animationCount[T2_HOFF_AnimActor(m_nAnims)]; s16 animationCount; }; /* N64 0xBA */
        struct { u8 _hp_flags[T2_HOFF_AnimActor(m_dwFlags)]; s32 flags; }; /* N64 0xD4 */
        struct { u8 _hp_currentTrack[T2_HOFF_AnimActor(m_CurrentAnimation)]; AnimTrack currentTrack; }; /* N64 0xD8 */
        struct { u8 _hp_previousTrack[T2_HOFF_AnimActor(m_BlendAnimation)]; AnimTrack previousTrack; }; /* N64 0xEC */
        struct { u8 _hp_blendFrames[T2_HOFF_AnimActor(m_BlendLength)]; s16 blendFrames; }; /* N64 0x100 */
        struct { u8 _hp_blend[T2_HOFF_AnimActor(m_Blend)]; f32 blend; }; /* N64 0x104 */
        struct { u8 _hp_blendTime[T2_HOFF_AnimActor(m_BlendPercent)]; f32 blendTime; }; /* N64 0x108 */
        struct { u8 _hp_blendStart[T2_HOFF_AnimActor(m_BlendStart)]; u8 blendStart; }; /* N64 0x10C */
        struct { u8 _hp_blendEnd[T2_HOFF_AnimActor(m_BlendFinish)]; u8 blendEnd; }; /* N64 0x10D */
    };
} AnimActor;
#else
typedef struct {
    /* 0x000 */ u8 pad0[4];
    /* 0x004 */ CVector3 position;
    /* 0x010 */ u8 pad10[0x24];
    /* 0x034 */ f32 scaleX;
    /* 0x038 */ f32 scaleY;
    /* 0x03C */ f32 scaleZ;
    /* 0x040 */ u8 pad40[0x10];
    /* 0x050 */ f32 heading;
    /* 0x054 */ u8 pad54[0x54];
    /* 0x0A8 */ s32 animationKey;
    /* 0x0AC */ u8 padAC[0xC];
    /* 0x0B8 */ u16 type;
    /* 0x0BA */ s16 animationCount;
    /* 0x0BC */ u8 padBC[0x18];
    /* 0x0D4 */ s32 flags;
    /* 0x0D8 */ AnimTrack currentTrack;
    /* 0x0EC */ AnimTrack previousTrack;
    /* 0x100 */ s16 blendFrames;
    /* 0x102 */ u8 pad102[2];
    /* 0x104 */ f32 blend;
    /* 0x108 */ f32 blendTime;
    /* 0x10C */ u8 blendStart;
    /* 0x10D */ u8 blendEnd;
} AnimActor;
#endif

void CAnimationState__SwitchToQueued(AnimTrack *track, sptr data, s32 key, s32 primary, s32 *active, AnimTrack *other);
s32 CAnimationState__IsLoaded(AnimTrack *track);
AnimBlendEntry *CAnimationState__GetKeyHeader(AnimTrack *track);
void CAnimationState__Release(AnimTrack *track);
void CAnimationState__GetRootMotion(AnimTrack *track, CVector3 *delta, f32 *turn, AnimActor *actor);
void CVector3__Lerp(CVector3 *out, f32 t, CVector3 *a, CVector3 *b);
f32 Angle_Lerp(f32 t, f32 a, f32 b);
void Angle_Wrap(f32 *angle);
void CMtxF__SetRotateY(MatrixF *m, f32 angle);
void CMtxF__TransformPoint(MatrixF *m, CVector3 *in, CVector3 *out);

/* Advances an actor's animation and applies its root motion: the turn and,
 * with flag 0x20000, the move, blended over the transition from the previous
 * track while flag 0x400 is set. */
void CGameObjectInstance__AdvanceAnimation(AnimActor *actor) {
    CVector3 first;
    CVector3 second;
    CVector3 saved;
    CVector3 moved;
    MatrixF matrix;
    s32 active;
    f32 first_value;
    f32 second_value;
    void **resource;
    AnimTrack *track0;
    AnimTrack *track1;
    f32 value;

    if (gScene.m_Mode == 4) {
        if (actor->currentTrack.active == 0) {
            actor->currentTrack.blending = 1;
        }
        if (actor->previousTrack.active == 0) {
            actor->previousTrack.blending = 1;
        }
        if (actor->currentTrack.active == 0 && !(actor->flags & 0x400)) {
            return;
        }
        if (!(actor->flags & 0x2000)) {
            return;
        }
    }

    resource = CCache__Request(0, actor->animationKey, ((actor->animationCount * 4) + 0xF) & ~7, 4, 0, 0,
                             "animations index", 0);
    if (resource == NULL) {
        return;
    }

    track0 = &actor->currentTrack;
    track1 = &actor->previousTrack;
    CAnimationState__SwitchToQueued(track0, (sptr) *resource, actor->animationKey, 1, &active, track1);
    if (CAnimationState__IsLoaded(track0) != 0) {
        AnimBlendEntry *entry;

        CAnimationState__GetRootMotion(track0, &first, &first_value, actor);
        value = first_value;
        saved = first;
        if (active != 0 && track1->current != -1) {
            entry = CAnimationState__GetKeyHeader(track0);
            if (entry->frames != 0 && (actor->flags & 0x200)) {
                u16 count;

                actor->flags |= 0x400;
                count = entry->frames;
                actor->blendTime = 0.0f;
                actor->blendFrames = count;
                actor->blendStart = entry->start;
                actor->blendEnd = entry->end;
            } else {
                actor->flags &= ~0x400;
            }
        }
        if (actor->flags & 0x400) {
            CAnimationState__SwitchToQueued(track1, (sptr) *resource, actor->animationKey, 0, 0, 0);
            if (CAnimationState__IsLoaded(track1) != 0) {
                f32 time;
                f32 end;

                time = actor->blendTime + gFrameIncrement;
                end = (f32) actor->blendFrames;
                actor->blendTime = time;
                if (end < time) {
                    actor->flags &= ~0x400;
                } else {
                    f32 a = (f32) actor->blendEnd;
                    f32 b = (f32) actor->blendStart;
                    f32 t = time / (end + gFrameIncrement);

                    actor->blend = ((a + -2.0f + b) * t * t * t) + (((3.0f - a) - (2.0f * b)) * t * t) + (b * t);
                    CAnimationState__GetRootMotion(&actor->previousTrack, &second, &second_value, actor);
                    CVector3__Lerp(&saved, actor->blend, &second, &first);
                    value = Angle_Lerp(actor->blend, second_value, first_value);
                }
                CAnimationState__Release(track1);
            } else {
                actor->flags &= ~0x400;
            }
        }
        CAnimationState__Release(track0);
        if (actor->flags & 0x20000) {
            actor->heading -= value;
            Angle_Wrap(&actor->heading);
            saved.z = saved.y;
            saved.y = 0.0f;
            CMtxF__SetRotateY(&matrix, actor->heading);
            CMtxF__TransformPoint(&matrix, &saved, &moved);
            actor->position.x += moved.x * actor->scaleX;
            actor->position.z += moved.z * actor->scaleZ;
        } else if (!(actor->flags & 0x300000) && actor->type == 0x137) {
            actor->heading -= value;
            Angle_Wrap(&actor->heading);
        }
    }
    CCache__Unlock(0, resource);
}
