#include "common.h"
#include "globals/game.h"
#include "structs/player.h"
#include "structs/weapon.h"

/* The scope model raised to the view (see CPlayer__AnimateScope). */

#define DEBUG_LINES_ON (*(s32 *) &gEngineApp_m_Game_m_DebugLines) /* the file's view of gEngineApp_m_Game_m_DebugLines, which globals/game.h declares: this type is what keeps the ROM's code */

void CPlayer__AnimateScope(CPlayerOverlay *m, CPlayer *owner);
void Math_EaseToward(f32 *value, f32 target, f32 rate);

/* Starts lowering the scope model if it is up or being raised. */
static __inline__ void LowerScope(CPlayerOverlay *m) {
    if (m->m_Mode != 0 && m->m_Mode != 3) {
        m->m_Mode = 3;
    }
}

/* Shows `model` as the scope, raising it again if it is already that one. */
static __inline__ void RaiseScope(CPlayerOverlay *m, s32 model) {
    if (m->m_DesiredObjectType == model) {
        m->m_Mode = 1;
    }
    m->m_DesiredObjectType = model;
}

/* Sniper zoom: toggled by button 0x100 on a weapon with a scope, then
 * adjusted by 8 (in) and 4 (out) while 0x10 is held. */
void CPlayer__UpdateZoom(CPlayer *p) {
    f32 target;
    f32 t;
    u16 model;

    target = 0.0f;
    if ((s16) gWeaponInfoTable[p->m_Data.m_CurrentWeapon]->zoomModel <= 0 || p->m_SniperMode == 0) {
        p->m_SniperMode = 0;
        LowerScope(&p->m_Overlay);
    }
    if (p->m_Data.m_Health != 0) {
        if ((p->m_ContState.m_dwButtonsSingle & 0x100) && (p->m_ContState.m_dwButtonsSingle & 0x100) == (p->m_ContState.m_dwButtons & 0x100)) {
            if (p->m_SniperMode != 0) {
                p->m_SniperMode = 0;
                LowerScope(&p->m_Overlay);
            } else if (gWeaponInfoTable[p->m_Data.m_CurrentWeapon]->zoomModel != 0 && (DEBUG_LINES_ON == 0 || !(p->m_ContState.m_dwButtons & 0x20))) {
                p->m_SniperMode = 1;
                if (p->m_Data.m_CurrentWeapon == 0x1B) {
                    p->m_TargetZoomBlend = 0.7f;
                } else {
                    p->m_TargetZoomBlend = 0.2f;
                }
                model = gWeaponInfoTable[p->m_Data.m_CurrentWeapon]->zoomModel;
                RaiseScope(&p->m_Overlay, model);
            }
        }
        if (p->m_SniperMode != 0) {
            target = p->m_TargetZoomBlend;
            p->m_SniperMode = 1;
            if (p->m_ContState.m_dwButtons & 0x10) {
                if (p->m_ContState.m_dwButtons & 8) {
                    t = target + 0.025f;
                    if (t > 1.0f) {
                        t = 1.0f;
                    }
                    target = t;
                    p->m_SniperMode = 2;
                }
                if (p->m_ContState.m_dwButtons & 4) {
                    t = target - 0.025f;
                    if (t < 0.2f) {
                        t = 0.2f;
                    }
                    target = t;
                    p->m_SniperMode = 2;
                }
            }
        }
    }
    p->m_TargetZoomBlend = target;
    CPlayer__AnimateScope(&p->m_Overlay, p);
    if (p->m_Overlay.m_Mode != 1) {
        Math_EaseToward(&p->m_ZoomBlend, target, 0.5f);
        if (target == 0.0f && p->m_ZoomBlend < 0.01f) {
            p->m_ZoomBlend = 0.0f;
        }
    }
}

#undef DEBUG_LINES_ON
