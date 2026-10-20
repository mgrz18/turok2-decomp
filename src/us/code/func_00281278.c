#include "common.h"
#include "m2c_macros.h"
#include "structs/camera.h"

/* Resets the warp: none, still, centred, no spin, scale 1. */
void func_00281278(CCamera *cam) {
    cam->m_WarpMode = 0;
    cam->m_WarpTime = 0.0f;
    cam->m_vWarpPos.x = 0.0f;
    cam->m_vWarpPos.y = 0.0f;
    cam->m_vWarpPos.z = 0.0f;
    cam->m_WarpSpin = 0.0f;
    cam->m_WarpXScale = 1.0f;
    cam->m_WarpYScale = 1.0f;
}
