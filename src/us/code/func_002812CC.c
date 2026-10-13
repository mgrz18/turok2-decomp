#include "common.h"
#include "gfx.h"
#include "structs/camera.h"

extern s32 D_800B6D1C;

/* Loads the camera's on-screen (2D) projection for this frame's buffer:
 * gSPMatrix(G_MTX_PROJECTION | G_MTX_LOAD). */
void func_002812CC(CCamera *cam) {
    Gfx *g = D_800D8E24++;

    g->w0 = 0xDA380007;
    g->w1 = (u32)&cam->m_mOnScreenProjection[D_800B6D1C];
}
