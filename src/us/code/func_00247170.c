#include "common.h"
#include "structs/player.h"
#include "globals/scene.h"

/* The scope model raised to the view. */
typedef struct {
    /* 0x00 */ s32 current;
    /* 0x04 */ s32 requested;
    /* 0x08 */ void *model;
    /* 0x0C */ u8 padC[0x90 - 0xC];
    /* 0x90 */ f32 scaleX;
    /* 0x94 */ f32 scaleY;
    /* 0x98 */ f32 scaleZ;
    /* 0x9C */ f32 pitch;
    /* 0xA0 */ f32 roll;
    /* 0xA4 */ u8 padA4[4];
    /* 0xA8 */ f32 x;
    /* 0xAC */ f32 y;
    /* 0xB0 */ f32 z;
    /* 0xB4 */ s32 state;           /* 0 hidden, 1 raising, 2 up, 3 lowering */
    /* 0xB8 */ f32 time;
} HudModel; /* size 0xBC */

extern u8 gEngineApp_m_Settings_multiplayer;           /* multiplayer */
extern f32 gFrameIncrement;          /* the frame's time step */

s32 CScene__FindObjectType(void *level, s32 id);
void *CScene__GetObject(void *level, s32 index);

/* Places the scope model and animates it up (state 1) or down (state 3)
 * over 3.75 frames: the sniper scope (0xC83) swings in from the side, the
 * others tip up from below. */
void CPlayer__AnimateScope(HudModel *m, CPlayer *owner) {
    s32 index;

    if (m->current == 0xC83) {
        m->x = -550.0f;
        m->y = 520.0f;
        m->z = -1125.0f;
        m->scaleX = 0.28f;
        m->scaleY = 0.33f;
        m->scaleZ = 0.25f;
    } else {
        m->x = 0.0f;
        m->y = 451.0f;
        m->z = -675.0f;
        m->scaleX = 0.2f;
        m->scaleY = 0.189f;
        m->scaleZ = 0.25f;
        m->roll = 3.141593f;
    }
    if (m->requested != m->current && m->state == 0) {
        index = CScene__FindObjectType(&gScene, m->requested);
        if (index != -1) {
            m->model = CScene__GetObject(&gScene, index);
            m->state = 1;
            m->current = m->requested;
        } else {
            m->model = NULL;
        }
    }
    switch (m->state) {
        case 0:
            break;
        case 1:
            m->time += gFrameIncrement;
            if (m->time >= 3.75f) {
                m->state = 2;
                m->time = 3.75f;
            }
            if (m->current == 0xC83) {
                if (gEngineApp_m_Settings_multiplayer) {
                    m->roll = 0.0f;
                    m->pitch = 0.0f;
                } else {
                    m->pitch = 0.0f;
                    m->roll = (360.0f - m->time * 48.000004f) * 0.017453294f;
                }
            } else {
                m->pitch = (1.0f - m->time * 0.26666668f) * 1.5707965f;
            }
            break;
        case 2:
            break;
        case 3:
            if (owner->m_ZoomBlend == 0.0f) {
                m->time -= gFrameIncrement;
                if (m->time <= 0.0f) {
                    m->state = 0;
                    m->time = 0.0f;
                }
            }
            if (m->current == 0xC83) {
                if (gEngineApp_m_Settings_multiplayer) {
                    m->roll = 0.0f;
                } else {
                    m->roll = ((1.0f - m->time * 0.26666668f) * 180.0f + 180.0f) * 0.017453294f;
                }
            } else {
                m->pitch = (1.0f - m->time * 0.26666668f) * 1.5707965f;
            }
            break;
    }
}
