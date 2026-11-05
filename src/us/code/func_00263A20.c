/* Boundary fixed in versions/cuts.us.txt: splat's func_00263A14 starts with 3 orphan words (addu a0,s0; addu a1,s0;
 * jal func_0020EEF8) whose delay slot is this function's first instruction. The real
 * function is func_00263A20 (called from func_00264378); this file matches it from
 * 0x263A20 to the end of splat's func_00263A14. */
#include "common.h"
#include "structs/game_object.h"
#include "structs/intelligence.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ CIntelligence *def;
    /* 0x18 */ u8 pad18[0xD4 - 0x18];
    /* 0xD4 */ s32 flags;
} Self;

extern f32 D_800B6D28; /* frame time */

f32 func_002119FC(f32 lo, f32 hi);
f32 func_00266F50(f32 x);

static __inline__ s32 canClose(CPlatformIntelligence *params, CAI *ai) {
    if ((params->m_dwPlatformFlags & 0x8000000) && !(ai->m_dwFlags & 0x20000)) {
        return 0;
    }
    if ((params->m_dwPlatformFlags & 0x10000000) && (ai->m_dwFlags & 0x20000)) {
        return 0;
    }
    return 1;
}

void func_00263A20(Self *self, CAI *ai) {
    CPlatformIntelligence *params;
    s32 oldState;
    s32 flags;

    params = &self->def->m_Platform;
    oldState = ai->m_Action;
    switch (oldState) {
        case 0:
            flags = params->m_dwPlatformFlags;
            if (!(flags & 0xC00000)) {
                break;
            }
            if (flags & 0x4000000) {
                if (ai->m_dwFlags & 0x20000) {
                    ai->m_Platform.m_SpecialTimer += D_800B6D28;
                } else {
                    ai->m_Platform.m_SpecialTimer = 0.0f;
                    break;
                }
            } else if (ai->m_Platform.m_SpecialTimer > 0.0f || (ai->m_dwFlags & 0x10000)) {
                ai->m_Platform.m_SpecialTimer += D_800B6D28;
            }
            if (ai->m_Platform.m_SpecialTimer >= params->m_SpecialDelay) {
                ai->m_ActionTime = 0.0f;
                flags = params->m_dwPlatformFlags;
                if (flags & 0x1000000) {
                    ai->m_Action = 3;
                } else if (flags & 0x400000) {
                    ai->m_Action = 1;
                } else if (flags & 0x800000) {
                    ai->m_Action = 4;
                }
            }
            break;
        case 3:
            ai->m_Platform.m_vSpecialOffset.x = func_002119FC(-0.512f, 0.512f);
            ai->m_Platform.m_vSpecialOffset.y = func_002119FC(-0.512f, 0.512f);
            ai->m_Platform.m_vSpecialOffset.z = func_002119FC(-0.512f, 0.512f);
            ai->m_ActionTime += D_800B6D28;
            if (ai->m_ActionTime > 45.0f) {
                ai->m_ActionTime = 0.0f;
                flags = params->m_dwPlatformFlags;
                if (flags & 0x400000) {
                    ai->m_Action = 1;
                } else if (flags & 0x800000) {
                    ai->m_Action = 4;
                }
            }
            break;
        case 1:
            if (ai->m_ActionTime < params->m_SpecialSinkTime) {
                ai->m_ActionTime += D_800B6D28;
            } else if (params->m_dwPlatformFlags & 0x2000000) {
                self->flags &= ~0x2000;
            } else {
                ai->m_ActionTime = params->m_SpecialSinkTime;
                if (canClose(params, ai)) {
                    ai->m_Action = 2;
                }
            }
            if (params->m_SpecialSinkTime > 0.0f) {
                ai->m_Platform.m_vSpecialOffset.y = -params->m_SpecialSinkDist * func_00266F50(ai->m_ActionTime / params->m_SpecialSinkTime);
            } else {
                ai->m_Platform.m_vSpecialOffset.y = -params->m_SpecialSinkDist;
            }
            break;
        case 2:
            if (ai->m_ActionTime > 0.0f) {
                ai->m_ActionTime -= D_800B6D28;
            } else {
                ai->m_Action = 0;
                ai->m_Platform.m_SpecialTimer = 0.0f;
            }
            if (params->m_SpecialSinkTime > 0.0f) {
                ai->m_Platform.m_vSpecialOffset.y = -params->m_SpecialSinkDist * func_00266F50(ai->m_ActionTime / params->m_SpecialSinkTime);
            } else {
                ai->m_Platform.m_vSpecialOffset.y = -params->m_SpecialSinkDist;
            }
            break;
        case 4:
            if (ai->m_ActionTime < params->m_SpecialSinkTime) {
                ai->m_ActionTime += D_800B6D28;
            }
            if (params->m_SpecialSinkTime > 0.0f) {
                ai->m_Platform.m_vSpecialOffset.y = -params->m_SpecialSinkDist * func_00266F50(ai->m_ActionTime / params->m_SpecialSinkTime);
            } else {
                ai->m_Platform.m_vSpecialOffset.y = -params->m_SpecialSinkDist;
            }
            if (params->m_dwPlatformFlags & 0x2000000) {
                self->flags &= ~0x2000;
            }
            break;
    }
    if (oldState != ai->m_Action) {
        ai->m_ModeLastFrame = -1;
    }
}
