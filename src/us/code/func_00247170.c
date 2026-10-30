#include "common.h"

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

typedef struct {
    /* 0x000 */ u8 pad0[0xB90];
    /* 0xB90 */ f32 zoom;
} ZoomPlayer;

extern u8 D_8011AAF1;           /* multiplayer */
extern f32 D_800B6D28;          /* the frame's time step */
extern u8 D_800F7078[];         /* the level */

s32 func_00224180(void *level, s32 id);
void *func_00224F84(void *level, s32 index);

/* Places the scope model and animates it up (state 1) or down (state 3)
 * over 3.75 frames: the sniper scope (0xC83) swings in from the side, the
 * others tip up from below. */
void func_00247170(HudModel *m, ZoomPlayer *owner) {
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
        index = func_00224180(D_800F7078, m->requested);
        if (index != -1) {
            m->model = func_00224F84(D_800F7078, index);
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
            m->time += D_800B6D28;
            if (m->time >= 3.75f) {
                m->state = 2;
                m->time = 3.75f;
            }
            if (m->current == 0xC83) {
                if (D_8011AAF1) {
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
            if (owner->zoom == 0.0f) {
                m->time -= D_800B6D28;
                if (m->time <= 0.0f) {
                    m->state = 0;
                    m->time = 0.0f;
                }
            }
            if (m->current == 0xC83) {
                if (D_8011AAF1) {
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
