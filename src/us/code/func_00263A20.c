/* Boundary fixed in versions/cuts.us.txt: splat's func_00263A14 starts with 3 orphan words (addu a0,s0; addu a1,s0;
 * jal CVector3__Add) whose delay slot is this function's first instruction. The real
 * function is Platform_UpdateSpecial (called from Platform_Step); this file matches it from
 * 0x263A20 to the end of splat's func_00263A14. */
#include "common.h"
#include "structs/game_object.h"

extern f32 gFrameIncrement; /* frame time */

f32 Random_Blend(f32 lo, f32 hi);
f32 SmoothStep(f32 x);

static __inline__ s32 canClose(CPlatformIntelligence *params, CAI *ai) {
    if ((params->m_dwPlatformFlags & 0x8000000) && !(ai->m_dwFlags & 0x20000)) {
        return 0;
    }
    if ((params->m_dwPlatformFlags & 0x10000000) && (ai->m_dwFlags & 0x20000)) {
        return 0;
    }
    return 1;
}

void Platform_UpdateSpecial(CGameObjectInstance *self, CAI *ai) {
    CPlatformIntelligence *params;
    s32 oldState;
    s32 flags;

    params = &self->header.m_pIntelligence->m_Platform;
    oldState = ai->m_Action;
    switch (oldState) {
        case 0:
            flags = params->m_dwPlatformFlags;
            if (!(flags & 0xC00000)) {
                break;
            }
            if (flags & 0x4000000) {
                if (ai->m_dwFlags & 0x20000) {
                    ai->m_Platform.m_SpecialTimer += gFrameIncrement;
                } else {
                    ai->m_Platform.m_SpecialTimer = 0.0f;
                    break;
                }
            } else if (ai->m_Platform.m_SpecialTimer > 0.0f || (ai->m_dwFlags & 0x10000)) {
                ai->m_Platform.m_SpecialTimer += gFrameIncrement;
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
            ai->m_Platform.m_vSpecialOffset.x = Random_Blend(-0.512f, 0.512f);
            ai->m_Platform.m_vSpecialOffset.y = Random_Blend(-0.512f, 0.512f);
            ai->m_Platform.m_vSpecialOffset.z = Random_Blend(-0.512f, 0.512f);
            ai->m_ActionTime += gFrameIncrement;
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
                ai->m_ActionTime += gFrameIncrement;
            } else if (params->m_dwPlatformFlags & 0x2000000) {
                self->m_dwFlags &= ~0x2000;
            } else {
                ai->m_ActionTime = params->m_SpecialSinkTime;
                if (canClose(params, ai)) {
                    ai->m_Action = 2;
                }
            }
            if (params->m_SpecialSinkTime > 0.0f) {
                ai->m_Platform.m_vSpecialOffset.y = -params->m_SpecialSinkDist * SmoothStep(ai->m_ActionTime / params->m_SpecialSinkTime);
            } else {
                ai->m_Platform.m_vSpecialOffset.y = -params->m_SpecialSinkDist;
            }
            break;
        case 2:
            if (ai->m_ActionTime > 0.0f) {
                ai->m_ActionTime -= gFrameIncrement;
            } else {
                ai->m_Action = 0;
                ai->m_Platform.m_SpecialTimer = 0.0f;
            }
            if (params->m_SpecialSinkTime > 0.0f) {
                ai->m_Platform.m_vSpecialOffset.y = -params->m_SpecialSinkDist * SmoothStep(ai->m_ActionTime / params->m_SpecialSinkTime);
            } else {
                ai->m_Platform.m_vSpecialOffset.y = -params->m_SpecialSinkDist;
            }
            break;
        case 4:
            if (ai->m_ActionTime < params->m_SpecialSinkTime) {
                ai->m_ActionTime += gFrameIncrement;
            }
            if (params->m_SpecialSinkTime > 0.0f) {
                ai->m_Platform.m_vSpecialOffset.y = -params->m_SpecialSinkDist * SmoothStep(ai->m_ActionTime / params->m_SpecialSinkTime);
            } else {
                ai->m_Platform.m_vSpecialOffset.y = -params->m_SpecialSinkDist;
            }
            if (params->m_dwPlatformFlags & 0x2000000) {
                self->m_dwFlags &= ~0x2000;
            }
            break;
    }
    if (oldState != ai->m_Action) {
        ai->m_ModeLastFrame = -1;
    }
}
