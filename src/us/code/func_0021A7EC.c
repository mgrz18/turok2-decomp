/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/particle/80246E34_2.CPP, func_802472F0_de, with Turok 2's layouts. */

#include "common.h"

typedef struct {
    f32 x, y, z;
} Vec3f;

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

typedef struct {
    /* 0x000 */ u8 pad0[4];
    /* 0x004 */ Vec3f position;
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

extern s32 D_800F7078;          /* the world's mode */
extern f32 D_800B6D28;          /* the frame's time step */

void **func_0020367C(s32, s32, s32, s32, s32, s32, char *, s32);
void func_002051F4(s32, void **);
void func_00218B10(AnimTrack *track, s32 data, s32 key, s32 primary, s32 *active, AnimTrack *other);
s32 func_00219968(AnimTrack *track);
AnimBlendEntry *func_00219994(AnimTrack *track);
void func_00219938(AnimTrack *track);
void func_00218DCC(AnimTrack *track, Vec3f *delta, f32 *turn, AnimActor *actor);
void func_0020EF8C(Vec3f *out, f32 t, Vec3f *a, Vec3f *b);
f32 func_00210DB4(f32 t, f32 a, f32 b);
void func_00210EF0(f32 *angle);
void func_002105A4(MatrixF *m, f32 angle);
void func_0020F85C(MatrixF *m, Vec3f *in, Vec3f *out);

/* Advances an actor's animation and applies its root motion: the turn and,
 * with flag 0x20000, the move, blended over the transition from the previous
 * track while flag 0x400 is set. */
void func_0021A7EC(AnimActor *actor) {
    Vec3f first;
    Vec3f second;
    Vec3f saved;
    Vec3f moved;
    MatrixF matrix;
    s32 active;
    f32 first_value;
    f32 second_value;
    void **resource;
    AnimTrack *track0;
    AnimTrack *track1;
    f32 value;

    if (D_800F7078 == 4) {
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

    resource = func_0020367C(0, actor->animationKey, ((actor->animationCount * 4) + 0xF) & ~7, 4, 0, 0,
                             "animations index", 0);
    if (resource == NULL) {
        return;
    }

    track0 = &actor->currentTrack;
    track1 = &actor->previousTrack;
    func_00218B10(track0, (s32) *resource, actor->animationKey, 1, &active, track1);
    if (func_00219968(track0) != 0) {
        AnimBlendEntry *entry;

        func_00218DCC(track0, &first, &first_value, actor);
        value = first_value;
        saved = first;
        if (active != 0 && track1->current != -1) {
            entry = func_00219994(track0);
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
            func_00218B10(track1, (s32) *resource, actor->animationKey, 0, 0, 0);
            if (func_00219968(track1) != 0) {
                f32 time;
                f32 end;

                time = actor->blendTime + D_800B6D28;
                end = (f32) actor->blendFrames;
                actor->blendTime = time;
                if (end < time) {
                    actor->flags &= ~0x400;
                } else {
                    f32 a = (f32) actor->blendEnd;
                    f32 b = (f32) actor->blendStart;
                    f32 t = time / (end + D_800B6D28);

                    actor->blend = ((a + -2.0f + b) * t * t * t) + (((3.0f - a) - (2.0f * b)) * t * t) + (b * t);
                    func_00218DCC(&actor->previousTrack, &second, &second_value, actor);
                    func_0020EF8C(&saved, actor->blend, &second, &first);
                    value = func_00210DB4(actor->blend, second_value, first_value);
                }
                func_00219938(track1);
            } else {
                actor->flags &= ~0x400;
            }
        }
        func_00219938(track0);
        if (actor->flags & 0x20000) {
            actor->heading -= value;
            func_00210EF0(&actor->heading);
            saved.z = saved.y;
            saved.y = 0.0f;
            func_002105A4(&matrix, actor->heading);
            func_0020F85C(&matrix, &saved, &moved);
            actor->position.x += moved.x * actor->scaleX;
            actor->position.z += moved.z * actor->scaleZ;
        } else if (!(actor->flags & 0x300000) && actor->type == 0x137) {
            actor->heading -= value;
            func_00210EF0(&actor->heading);
        }
    }
    func_002051F4(0, resource);
}
