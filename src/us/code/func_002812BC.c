#include "common.h"
#include "m2c_macros.h"
#include "structs/camera.h"

/* Starts warp 2. */
void func_002812BC(CCamera *cam) {
    cam->m_WarpTime = 0.0f;
    cam->m_WarpMode = 2;
}
