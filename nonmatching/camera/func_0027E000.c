#include "common.h"
#include "gfx.h"
#include "structs/camera.h"

extern s32 D_800C2030;          /* screen width */
extern s32 D_800C2034;          /* screen height */
extern s32 D_800B6D1C;          /* which of the two frame buffers is drawn */
extern s32 D_8011AAD8;          /* game flags */
extern Gfx D_800AB100[];        /* RDP setup display lists */
extern Gfx D_800AB130[];
extern CCameraPool D_801198B0;  /* the camera pool */
extern u8 *D_800F7070;          /* the frame being drawn; its colour image at 0x110 */
extern void *D_80130A78;        /* the z buffer */
void func_00207A54(void);
void func_00207FEC(s32 arg0);
s32 Cinema_PathPlaying(void);
u32 func_002843F4(void);        /* a cinematic's fog colour, 0xFF00 for none */
void func_002284E0(CCamera *cam);

/* Opens a camera's part of the frame's display list (LibTEngine's CCamera
 * DisplayListSetup): its projection, viewport, scissor and look-at; then,
 * unless it is the main camera with others active, clears the z buffer and
 * the view to the fog colour, with the fog from m_FogStart to 1000. The
 * commands go through a local copy of the display list position
 * (D_800D8E24), stored back before each call that writes commands too. */
void func_0027E000(CCamera *cam) {
    Gfx *gdl;
    Gfx *g;
    Vp *vp;
    LookAt *lookAt;
    s16 left, top, right, bottom;
    s32 range;
    u32 colour;
    u32 fill;
    u8 rgb[3];

    gdl = D_800D8E24;
    gdl[0].w0 = 0xE7000000;                     /* gDPPipeSync */
    gdl[0].w1 = 0;
    gdl[1].w1 = (u32) D_800AB100;               /* gSPDisplayList: RDP setup */
    gdl[1].w0 = 0xDE000000;
    gdl[2].w0 = 0xDE000000;
    gdl[2].w1 = (u32) D_800AB130;
    cam->m_PixelXScale = 1.0f;
    cam->m_PixelYScale = 1.0f;
    cam->m_PixelXScale = cam->m_WarpXScale;
    left = (u32) cam->m_Viewport.m_ScreenPosX;
    top = (u32) cam->m_Viewport.m_ScreenPosY;
    cam->m_PixelYScale = cam->m_WarpYScale;
    right = (u32) (cam->m_Viewport.m_ScreenPosX + cam->m_Viewport.m_Width);
    bottom = (u32) (cam->m_Viewport.m_ScreenPosY + cam->m_Viewport.m_Height);
    if (right > D_800C2030 - 1) {
        right = D_800C2030 - 1;
    }
    if (left < 0) {
        left = 0;
    }
    if (top < 0) {
        top = 0;
    }
    if (bottom > D_800C2034 - 1) {
        bottom = D_800C2034 - 1;
    }
    gdl[3].w0 = 0xDA380007;                     /* gSPMatrix(projection, load) */
    gdl[3].w1 = (u32) &cam->m_mProjection[D_800B6D1C];
    vp = &cam->m_vp[D_800B6D1C];
    gdl[4].w0 = 0xDB0E0000;                     /* gSPPerspNormalize */
    gdl[4].w1 = cam->m_PerspNorm;
    vp->vscale[0] = (cam->m_Viewport.m_Width + cam->m_Viewport.m_Width) * cam->m_PixelXScale;
    vp->vscale[1] = (cam->m_Viewport.m_Height + cam->m_Viewport.m_Height) * cam->m_PixelYScale;
    vp->vtrans[0] = (vp->vscale[0] * 0.25f + cam->m_Viewport.m_ScreenPosX) * 4.0f;
    lookAt = &cam->m_LookAt[D_800B6D1C];
    D_800D8E24 = gdl + 9;
    vp->vtrans[1] = (vp->vscale[1] * 0.25f + cam->m_Viewport.m_ScreenPosY) * 4.0f;
    gdl[5].w0 = 0xDC080008;                     /* gSPViewport */
    gdl[5].w1 = (u32) vp;
    gdl[6].w0 = 0xED000000                      /* gDPSetScissor */
              | ((s32) (left * 4.0f) & 0xFFF) << 12 | ((s32) (top * 4.0f) & 0xFFF);
    gdl[6].w1 = ((s32) (right * 4.0f) & 0xFFF) << 12 | ((s32) (bottom * 4.0f) & 0xFFF);
    gdl[7].w0 = 0xDC08000A;                     /* gSPLookAtX */
    gdl[7].w1 = (u32) lookAt;
    gdl[8].w0 = 0xDC08030A;                     /* gSPLookAtY */
    gdl[8].w1 = (u32) lookAt + 0x10;
    func_00207A54();
    g = D_800D8E24++;
    g->w1 = 0x2000;
    g->w0 = 0xE3001201;
    if (cam != &D_801198B0.m_MainCamera || D_801198B0.m_ActiveList.m_nCount == 0) {
        func_00207FEC(0x14);
        gdl = D_800D8E24;                       /* clear the z buffer */
        gdl[0].w0 = 0xE7000000;
        gdl[0].w1 = 0;
        gdl[1].w0 = 0xE3000A01;                 /* gDPSetCycleType(G_CYC_FILL) */
        gdl[1].w1 = 0x300000;
        gdl[2].w0 = 0xFF100000 | ((D_800C2030 - 1) & 0xFFF);     /* gDPSetColorImage(z buffer) */
        gdl[2].w1 = (u32) D_80130A78;
        gdl[3].w0 = 0xE7000000;
        gdl[3].w1 = 0;
        gdl[4].w0 = 0xE3000A01;
        gdl[4].w1 = 0x300000;
        gdl[5].w0 = 0xF7000000;                 /* gDPSetFillColor(the farthest z) */
        gdl[5].w1 = 0xFFFCFFFC;
        gdl[6].w0 = 0xF6000000 | (right & 0x3FF) << 14 | (bottom & 0x3FF) << 2;   /* gDPFillRectangle */
        gdl[6].w1 = (left & 0x3FF) << 14 | (top & 0x3FF) << 2;
        D_800D8E24 = gdl + 7;
        if (!(D_8011AAD8 & 0x4000)) {
            rgb[0] = cam->m_FogColor[0] & 0xF8;
            rgb[1] = cam->m_FogColor[1] & 0xF8;
            rgb[2] = cam->m_FogColor[2] & 0xF8;
        } else {
            rgb[0] = 0;
            rgb[1] = 0;
            rgb[2] = 0;
        }
        if (Cinema_PathPlaying() != 0 && (colour = func_002843F4()) != 0xFF00) {
            rgb[2] = colour >> 16;
            rgb[1] = colour >> 8;
            rgb[0] = colour;
        }
        gdl = D_800D8E24;                       /* the fog, and the view cleared to it */
        gdl[0].w0 = 0xD9FFFFFF;                 /* gSPSetGeometryMode(G_FOG) */
        gdl[0].w1 = 0x10000;
        gdl[1].w0 = 0xDB080000;                 /* gSPFogPosition(m_FogStart, 1000) */
        range = 1000 - ((u16 *) &cam->m_FogStart)[1];
        gdl[1].w1 = (128000 / range) << 16 | (((500 - ((u16 *) &cam->m_FogStart)[1]) << 8) / range & 0xFFFF);
        gdl[2].w0 = 0xF8000000;                 /* gDPSetFogColor */
        gdl[2].w1 = rgb[0] << 24 | rgb[1] << 16 | rgb[2] << 8 | 0xFF;
        fill = (rgb[0] & 0xF8) << 8 | (rgb[1] & 0xF8) << 3 | (rgb[2] >> 2 & 0x3E);
        gdl[3].w0 = 0xFF100000 | ((D_800C2030 - 1) & 0xFFF);     /* gDPSetColorImage(the frame) */
        D_800D8E24 = gdl + 6;
        gdl[3].w1 = *(u32 *) (D_800F7070 + 0x110);
        gdl[4].w0 = 0xF7000000;                 /* gDPSetFillColor(the fog colour) */
        gdl[4].w1 = (fill | 1) << 16 | 1 | fill;
        gdl[5].w0 = 0xF6000000 | (right & 0x3FF) << 14 | (bottom & 0x3FF) << 2;
        gdl[5].w1 = (left & 0x3FF) << 14 | (top & 0x3FF) << 2;
    }
    gdl = D_800D8E24;
    gdl[0].w0 = 0xE7000000;
    gdl[0].w1 = 0;
    gdl[1].w0 = 0xE3001801;
    gdl[1].w1 = 0;
    gdl[2].w0 = 0xE3001A01;
    gdl[2].w1 = 0x10;
    D_800D8E24 = gdl + 4;
    gdl[3].w0 = 0xE3000A01;
    gdl[3].w1 = 0x100000;
    if (cam != &D_801198B0.m_MainCamera) {
        func_002284E0(cam);
    }
    g = D_800D8E24++;
    g->w0 = 0xE7000000;
    g->w1 = 0;
}
