/* WRONG CUT: splat's func_0025AD88 starts two orphan words early (0x25AD88 `sw $t0, 0x20($sp)`,
 * 0x25AD8C `bnez $v0, .L0025B284`, whose delay slot is this function's first instruction).
 * The real function is func_0025AD90 (0x2E8 bytes); this C matches it from 0x25AD90 on. */
#include "common.h"

/* AI head tracking: turns the head bones toward the target (or the player in state 0xC). */

typedef struct {
    f32 x, y, z;
} Vec3;

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s8 neckBone;
    /* 0x19 */ s8 headBone;
    /* 0x1A */ s8 boneCount;
} AnimObjectInfo;

typedef struct {
    /* 0x000 */ u8 pad0[0x4];
    /* 0x004 */ Vec3 pos;
    /* 0x010 */ u8 pad10[0x54 - 0x10];
    /* 0x054 */ f32 eyeOffset;
    /* 0x058 */ u8 pad58[0xA4 - 0x58];
    /* 0x0A4 */ s32 animObject;
    /* 0x0A8 */ u8 padA8[0xB0 - 0xA8];
    /* 0x0B0 */ s32 animIndex;
} Actor;

typedef struct {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ u8 pad8[0x24 - 0x8];
    /* 0x24 */ f32 headYaw;
    /* 0x28 */ f32 headPitch;
    /* 0x2C */ u8 pad2C[0x34 - 0x2C];
    /* 0x34 */ s8 state;
    /* 0x35 */ u8 pad35[0x3C - 0x35];
    /* 0x3C */ s32 flags;
    /* 0x40 */ u8 pad40[0x80 - 0x40];
    /* 0x80 */ Actor *target;
} CAI;

extern Vec3 D_800B06A0; /* player position */

extern s32 *func_0020367C(s32, s32, s32, s32, s32, s32, const char *, s32);
extern AnimObjectInfo *func_002017D4(s32 table, s32 index);
extern void func_002051F4(s32 arg0, s32 *resource);
extern f32 func_002671B4(Actor *actor); /* height */
extern f32 func_00246310(Actor *actor, Vec3 point);
extern void func_0020EF2C(Vec3 *out, Vec3 *a, Vec3 *b);
extern f32 func_0020F5C0(Vec3 *v);
extern void func_002117DC(f32 *value, f32 target, f32 rate);

void func_0025AD90(Actor *actor, CAI *ai, f32 pitchOffset) {
    Vec3 from;
    Vec3 to;
    Vec3 delta;
    f32 value;
    s32 *res;
    AnimObjectInfo *info;
    s32 count;
    f32 yaw;
    f32 pitch;
    f32 n;
    Actor *target;

    res = func_0020367C(0, actor->animObject, actor->animIndex, 0, 0, 0, "anim object info", 1);
    if (res == NULL) {
        return;
    }
    info = func_002017D4(*res, 0);
    if (info->headBone != -1 && info->neckBone != -1 && (count = info->boneCount) != 0) {
        if ((ai->flags & 2) && ai->unk_04 != 0 && (ai->target != NULL || ai->state == 0xC)) {
            from.x = actor->pos.x;
            from.y = actor->pos.y + func_002671B4(actor) * 0.9f + actor->eyeOffset;
            from.z = actor->pos.z;
            if (ai->state == 0xC) {
                to = D_800B06A0;
            } else {
                target = ai->target;
                to.x = target->pos.x;
                to.y = target->pos.y + func_002671B4(target) * 0.9f + target->eyeOffset;
                to.z = target->pos.z;
            }
            yaw = -func_00246310(actor, to);
            if (yaw < -1.5707965f) {
                yaw = -1.5707965f;
            } else if (yaw > 1.5707965f) {
                yaw = 1.5707965f;
            }
            func_0020EF2C(&delta, &to, &from);
            pitch = func_0020F5C0(&delta);
            if (pitch < -1.5707965f) {
                pitch = -1.5707965f;
            } else if (pitch > 1.5707965f) {
                pitch = 1.5707965f;
            }
        } else {
            pitch = yaw = 0.0f;
        }
        n = count;
        value = ai->headYaw * n;
        func_002117DC(&value, yaw, 0.25f);
        ai->headYaw = value / n;
        value = ai->headPitch * n;
        func_002117DC(&value, pitch + pitchOffset, 0.25f);
        ai->headPitch = value / n;
    }
    func_002051F4(0, res);
}
