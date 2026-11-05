#include "common.h"
#include "structs/intelligence.h"
#include "structs/level_actor.h"
#include "structs/region.h"

extern s32 D_800B6D00;
extern u8 D_800B6D1B; /* frame counter */
extern f32 D_800B6D28; /* frame time scale */
extern s32 D_8011ACA4;

/* Once-per-frame update. */
void func_002156FC(CLevelActor *obj) {
    CIntelligence *def;
    f32 timer;

    if (obj->m_UpdatedFrame == D_800B6D1B || D_8011ACA4 != 0) {
        return;
    }
    def = obj->header.m_pIntelligence;
    if (def->m_Type == 12) {
        obj->m_AnimSpeed = D_800B6D28 * def->m_Morpher.m_AnimSpeed;
    } else if (obj->m_wFlags & 0x10) {
        timer = obj->m_Timer + D_800B6D28 * (1.0f / 15.0f);
        obj->m_Timer = timer;
        if (timer < 0.75f) {
            obj->m_ModelScale = timer * (0.35f / 0.75f);
        } else {
            obj->m_wFlags &= ~0x10;
            obj->m_ModelScale = 0.35f;
        }
    }
    if (obj->header.m_pCurrentRegion != NULL) {
        obj->m_ViewMask = obj->header.m_pCurrentRegion->m_VisBits;
    } else {
        obj->m_ViewMask = D_800B6D00;
    }
    obj->m_UpdatedFrame = D_800B6D1B;
}
