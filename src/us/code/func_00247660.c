#include "common.h"
#include "structs/player.h"

typedef struct {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ u16 zoomModel;       /* the scope model shown when zoomed, 0 = none */
} WeaponDef;

/* The scope model raised to the view (see func_00247170). */
typedef struct {
    /* 0x00 */ s32 current;
    /* 0x04 */ s32 requested;
    /* 0x08 */ u8 pad8[0xB4 - 8];
    /* 0xB4 */ s32 state;           /* 0 hidden, 1 raising, 2 up, 3 lowering */
    /* 0xB8 */ f32 time;
} HudModel; /* size 0xBC */

extern WeaponDef *D_800AF618[];
extern s32 D_8011AAA0;

void func_00247170(HudModel *m, CPlayer *owner);
void func_002117DC(f32 *value, f32 target, f32 rate);

/* Starts lowering the scope model if it is up or being raised. */
static __inline__ void LowerScope(HudModel *m) {
    if (m->state != 0 && m->state != 3) {
        m->state = 3;
    }
}

/* Shows `model` as the scope, raising it again if it is already that one. */
static __inline__ void RaiseScope(HudModel *m, s32 model) {
    if (m->requested == model) {
        m->state = 1;
    }
    m->requested = model;
}

/* Sniper zoom: toggled by button 0x100 on a weapon with a scope, then
 * adjusted by 8 (in) and 4 (out) while 0x10 is held. */
void func_00247660(CPlayer *p) {
    f32 target;
    f32 t;
    u16 model;

    target = 0.0f;
    if ((s16) D_800AF618[p->m_Data.m_CurrentWeapon]->zoomModel <= 0 || p->m_SniperMode == 0) {
        p->m_SniperMode = 0;
        LowerScope(&p->m_Overlay);
    }
    if (p->m_Data.m_Health != 0) {
        if ((p->m_ContState.m_dwButtonsSingle & 0x100) && (p->m_ContState.m_dwButtonsSingle & 0x100) == (p->m_ContState.m_dwButtons & 0x100)) {
            if (p->m_SniperMode != 0) {
                p->m_SniperMode = 0;
                LowerScope(&p->m_Overlay);
            } else if (D_800AF618[p->m_Data.m_CurrentWeapon]->zoomModel != 0 && (D_8011AAA0 == 0 || !(p->m_ContState.m_dwButtons & 0x20))) {
                p->m_SniperMode = 1;
                if (p->m_Data.m_CurrentWeapon == 0x1B) {
                    p->m_TargetZoomBlend = 0.7f;
                } else {
                    p->m_TargetZoomBlend = 0.2f;
                }
                model = D_800AF618[p->m_Data.m_CurrentWeapon]->zoomModel;
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
    func_00247170(&p->m_Overlay, p);
    if (p->m_Overlay.m_Mode != 1) {
        func_002117DC(&p->m_ZoomBlend, target, 0.5f);
        if (target == 0.0f && p->m_ZoomBlend < 0.01f) {
            p->m_ZoomBlend = 0.0f;
        }
    }
}
