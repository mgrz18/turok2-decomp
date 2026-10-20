#include "common.h"
#include "m2c_macros.h"
#include "structs/camera.h"

M2C_UNK func_0027C074();                            /* extern */
M2C_UNK func_002883E0(M2C_UNK *, void *, s32, s32, s32, s32); /* extern */
extern M2C_UNK D_800F6CB0;

/* Draws the camera's flash: a rectangle of m_FlashColor over its viewport. */
void func_00280FD8(CCamera *cam) {
    f32 x;

    func_0027C074();
    if (cam->m_FlashMode != 0) {
        x = cam->m_Viewport.m_ScreenPosX;
        func_002883E0(&D_800F6CB0, cam->m_FlashColor, (s32) x, (s32) cam->m_Viewport.m_ScreenPosY,
                      (s32) (x + cam->m_Viewport.m_Width),
                      (s32) (cam->m_Viewport.m_ScreenPosY + cam->m_Viewport.m_Height));
    }
}
