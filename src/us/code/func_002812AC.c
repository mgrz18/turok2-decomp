#include "common.h"
#include "m2c_macros.h"
#include "structs/camera.h"

/* Starts warp 1. */
void func_002812AC(CCamera *cam) {
    cam->m_WarpTime = 0.0f;
    cam->m_WarpMode = 1;
}
