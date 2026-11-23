#include "common.h"
#include "structs/player.h"
#include "globals/game.h"
#include "globals/scene.h"
#include "globals/engine.h"
#include "protos/scene.h"

/* The scope model raised to the view. */


/* Places the scope model and animates it up (state 1) or down (state 3)
 * over 3.75 frames: the sniper scope (0xC83) swings in from the side, the
 * others tip up from below. */
void CPlayer__AnimateScope(CPlayerOverlay *m, CPlayer *owner) {
    s32 index;

    if (m->m_ObjectType == 0xC83) {
        m->m_vPos.x = -550.0f;
        m->m_vPos.y = 520.0f;
        m->m_vPos.z = -1125.0f;
        m->m_vScale.x = 0.28f;
        m->m_vScale.y = 0.33f;
        m->m_vScale.z = 0.25f;
    } else {
        m->m_vPos.x = 0.0f;
        m->m_vPos.y = 451.0f;
        m->m_vPos.z = -675.0f;
        m->m_vScale.x = 0.2f;
        m->m_vScale.y = 0.189f;
        m->m_vScale.z = 0.25f;
        m->m_vRot.y = 3.141593f;
    }
    if (m->m_DesiredObjectType != m->m_ObjectType && m->m_Mode == 0) {
        index = CScene__FindObjectType(&gScene, m->m_DesiredObjectType);
        if (index != -1) {
            m->m_rpObject = CScene__GetObject(&gScene, index);
            m->m_Mode = 1;
            m->m_ObjectType = m->m_DesiredObjectType;
        } else {
            m->m_rpObject = NULL;
        }
    }
    switch (m->m_Mode) {
        case 0:
            break;
        case 1:
            m->m_ModeTime += gFrameIncrement;
            if (m->m_ModeTime >= 3.75f) {
                m->m_Mode = 2;
                m->m_ModeTime = 3.75f;
            }
            if (m->m_ObjectType == 0xC83) {
                if (gEngineApp_m_Settings.multiplayer) {
                    m->m_vRot.y = 0.0f;
                    m->m_vRot.x = 0.0f;
                } else {
                    m->m_vRot.x = 0.0f;
                    m->m_vRot.y = (360.0f - m->m_ModeTime * 48.000004f) * 0.017453294f;
                }
            } else {
                m->m_vRot.x = (1.0f - m->m_ModeTime * 0.26666668f) * 1.5707965f;
            }
            break;
        case 2:
            break;
        case 3:
            if (owner->m_ZoomBlend == 0.0f) {
                m->m_ModeTime -= gFrameIncrement;
                if (m->m_ModeTime <= 0.0f) {
                    m->m_Mode = 0;
                    m->m_ModeTime = 0.0f;
                }
            }
            if (m->m_ObjectType == 0xC83) {
                if (gEngineApp_m_Settings.multiplayer) {
                    m->m_vRot.y = 0.0f;
                } else {
                    m->m_vRot.y = ((1.0f - m->m_ModeTime * 0.26666668f) * 180.0f + 180.0f) * 0.017453294f;
                }
            } else {
                m->m_vRot.x = (1.0f - m->m_ModeTime * 0.26666668f) * 1.5707965f;
            }
            break;
    }
}
