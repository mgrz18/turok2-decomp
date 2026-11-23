/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/WEAPONS/80213ED4_8.CPP, func_802164A8_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/vector.h"
#include "structs/game_object.h"
#include "structs/intelligence.h"
#include "globals/engine.h"

/* Typed on CGameObjectInstance / CAI (m2c's Source/Dest views had the N64 offsets). */

/* The attached looping sound (only its state word). */
typedef struct {
    u8 pad0[0xC];
    s32 unkC;                   /* 0xC: -1 = finished */
} SoundSource;

extern void AI_StopSound(CGameObjectInstance *, CAI *);
extern void AI_UpdateRegionFlags(CGameObjectInstance *, CAI *);
extern void AI_RideMovingPlatform(CGameObjectInstance *, CAI *);
extern void CAI__UpdateModeAnim(CGameObjectInstance *, CAI *);

void CAI__Update(CGameObjectInstance *src, CAI *dst) {
    SoundSource *sound = (SoundSource *) dst->m_pAttachedLoopingSound;

    if (sound != NULL && sound->unkC == -1) {
        AI_StopSound(src, dst);
    }
    if (dst->m_FreezeTime != 0) {
        dst->m_FreezeTime--;
        gFrameIncrement = 0.0f;
        return;
    }
    if (src->m_AI.m_pModeTable != 0) {
        if (src->header.m_pIntelligence->m_Type == 2) {
            dst->m_vPosLastFrame = src->header.m_vPos;
            dst->m_RotYLastFrame = src->m_RotY;
        }
        dst->m_vDesiredPos = src->header.m_vPos;
        dst->m_ModeTime += gFrameIncrement;
        AI_UpdateRegionFlags(src, dst);
        if (src->m_dwFlags & 0x10000) {
            AI_RideMovingPlatform(src, dst);
        }
        if (dst->m_pfStep != NULL) {
            ((void (*)(CGameObjectInstance *, CAI *)) dst->m_pfStep)(src, dst);
        }
        CAI__UpdateModeAnim(src, dst);
        if (src->header.m_pIntelligence->m_Type != 2) {
            dst->m_vPosLastFrame = src->header.m_vPos;
            dst->m_RotYLastFrame = src->m_RotY;
        }
    }
}
