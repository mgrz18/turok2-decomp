/* WRONG CUT: splat's func_0025AD88 starts two orphan words early (0x25AD88 `sw $t0, 0x20($sp)`,
 * 0x25AD8C `bnez $v0, .L0025B284`, whose delay slot is this function's first instruction).
 * The real function is func_0025AD90 (0x2E8 bytes); this C matches it from 0x25AD90 on. */
#include "common.h"
#include "structs/game_object.h"
#include "structs/vector.h"

/* AI head tracking: turns the head bones toward the target (or the player in state 0xC). */

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s8 neckBone;
    /* 0x19 */ s8 headBone;
    /* 0x1A */ s8 boneCount;
} AnimObjectInfo;

extern CVector3 D_800B06A0; /* player position */

extern s32 *func_0020367C(s32, s32, s32, s32, s32, s32, const char *, s32);
extern AnimObjectInfo *func_002017D4(s32 table, s32 index);
extern void func_002051F4(s32 arg0, s32 *resource);
extern f32 func_002671B4(CGameObjectInstance *actor); /* height */
extern f32 func_00246310(CGameObjectInstance *actor, CVector3 point);
extern void func_0020EF2C(CVector3 *out, CVector3 *a, CVector3 *b);
extern f32 func_0020F5C0(CVector3 *v);
extern void func_002117DC(f32 *value, f32 target, f32 rate);

void func_0025AD90(CGameObjectInstance *actor, CAI *ai, f32 pitchOffset) {
    CVector3 from;
    CVector3 to;
    CVector3 delta;
    f32 value;
    s32 *res;
    AnimObjectInfo *info;
    s32 count;
    f32 yaw;
    f32 pitch;
    f32 n;
    CGameObjectInstance *target;

    res = func_0020367C(0, actor->m_rpObject, actor->m_rpObjectHeader, 0, 0, 0, "anim object info", 1);
    if (res == NULL) {
        return;
    }
    info = func_002017D4(*res, 0);
    if (info->headBone != -1 && info->neckBone != -1 && (count = info->boneCount) != 0) {
        if ((ai->m_ModeFlags & 2) && ai->m_Health != 0 && (ai->m_pSightTarget != NULL || ai->m_Mode == 0xC)) {
            from.x = actor->header.m_vPos.x;
            from.y = actor->header.m_vPos.y + func_002671B4(actor) * 0.9f + actor->m_CollisionYOffset;
            from.z = actor->header.m_vPos.z;
            if (ai->m_Mode == 0xC) {
                to = D_800B06A0;
            } else {
                target = ai->m_pSightTarget;
                to.x = target->header.m_vPos.x;
                to.y = target->header.m_vPos.y + func_002671B4(target) * 0.9f + target->m_CollisionYOffset;
                to.z = target->header.m_vPos.z;
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
        value = ai->m_HeadTurn * n;
        func_002117DC(&value, yaw, 0.25f);
        ai->m_HeadTurn = value / n;
        value = ai->m_HeadTilt * n;
        func_002117DC(&value, pitch + pitchOffset, 0.25f);
        ai->m_HeadTilt = value / n;
    }
    func_002051F4(0, res);
}
