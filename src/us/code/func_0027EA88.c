#include "common.h"
#include "structs/camera.h"

extern f32 D_800F5A44;
s32 SavedPlayers_IsActive(void);

/* One step of a viewport value toward its target: 16 pixels a frame, or
 * there at once when SavedPlayers_IsActive says so; while it moves, D_800F5A44 is
 * set to 30. */
#define STEP_TOWARD(field, target)              \
    {                                           \
        f32 v = (field);                        \
        f32 speed = 16.0f;                      \
        if (SavedPlayers_IsActive()) {                  \
            v = (target);                       \
        }                                       \
        if (v < (target)) {                     \
            v += speed;                         \
            D_800F5A44 = 30.0f;                 \
            if ((target) < v) {                 \
                v = (target);                   \
            }                                   \
        } else if ((target) < v) {              \
            v -= speed;                         \
            D_800F5A44 = 30.0f;                 \
            if (v < (target)) {                 \
                v = (target);                   \
            }                                   \
        }                                       \
        (field) = v;                            \
    }

/* Moves the camera's viewport toward a size and a place on the screen (a
 * split screen opening or closing). */
void CCamera__MoveViewport(CCamera *cam, f32 width, f32 height, f32 x, f32 y) {
    STEP_TOWARD(cam->m_Viewport.m_Width, width);
    STEP_TOWARD(cam->m_Viewport.m_Height, height);
    STEP_TOWARD(cam->m_Viewport.m_ScreenPosX, x);
    STEP_TOWARD(cam->m_Viewport.m_ScreenPosY, y);
}
