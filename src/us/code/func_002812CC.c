#include "common.h"
#include "gfx.h"
#include "structs/camera.h"
#include "globals/engine.h"


/* Loads the camera's on-screen (2D) projection for this frame's buffer:
 * gSPMatrix(G_MTX_PROJECTION | G_MTX_LOAD). */
void CCamera__LoadOnScreenProjection(CCamera *cam) {
    Gfx *g = gDList++;

    g->w0 = 0xDA380007;
    g->w1 = (u32)&cam->m_mOnScreenProjection[gEvenOdd];
}
