/* NON-MATCHING: 54 of 476 words differ, same size: everything after 0x180 matches except the FP temp of the trunc.w.s ($f7 vs $f5, a consequence of the head). The head (0x84-0x17C) is a sched1 tie: the HI move of ulx and the store to m_PixelXScale both have priority 3 (ulx gets no birth boost because the clamp assigns it twice), and the store wins on potential hazard; in the ROM the m_WarpXScale load that feeds that store comes first, the second m_WarpXScale load (sx) becomes reload_cse's mov.s $f0,$f1, and ulx/uly land in $s5/$s4. Here the copy goes the other way and the x/y truncs swap. 300+ statement orders and 30 min of permuter on the head found nothing closer than 51 (with an unnatural order). */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/CAMERA/80233920_6.CPP, Camera_SetupViewportAndFog, with Turok 2's layouts. */
#include "common.h"
#include "gfx.h"
#include "structs/camera.h"

extern s32 D_800C2030;          /* screen width */
extern s32 D_800C2034;          /* screen height */
extern s32 D_800B6D1C;          /* which of the two frame buffers is drawn */
typedef struct {
    /* 0x00 */ s32 m_Flags;
} GameFlags;
extern GameFlags D_8011AAD8;    /* game flags */
extern Gfx D_800AB100[];        /* RDP setup display lists */
extern Gfx D_800AB130[];
extern CCamera D_801198F0;      /* the main camera (camera pool + 0x40) */
extern s32 D_801198E0;          /* the pool's active camera count */
typedef struct {
    /* 0x000 */ u8 _pad[0x110];
    /* 0x110 */ void *m_ColorImage;
} Frame;
typedef struct {
    /* 0x00 */ Frame *m_pFrame;
} FrameRef;
extern FrameRef D_800F7070;     /* the frame being drawn */
extern void *D_80130A78;        /* the z buffer */
void func_00207A54(void);
void func_00207FEC(s32 arg0);
s32 Cinema_PathPlaying(void);
u32 func_002843F4(void);        /* a cinematic's fog colour, 0xFF00 for none */
void func_002284E0(CCamera *cam);

#define GFX(dl, a, b) { Gfx *_g = (dl)++; _g->w0 = (a); _g->w1 = (b); }

static __inline__ void ResetPixelScale(CCamera *cam) {
    cam->m_PixelXScale = 1.0f;
    cam->m_PixelYScale = 1.0f;
}

/* Opens a camera's part of the frame's display list (LibTEngine's CCamera
 * DisplayListSetup): its projection, viewport, scissor and look-at; then,
 * unless it is the main camera with others active, clears the z buffer and
 * the view to the fog colour, with the fog from m_FogStart to 1000. */
void func_0027E000(CCamera *cam) {
    struct { u8 red, green, blue; } rgb;
    s16 ulx, uly, lrx, lry;
    s32 near, far, denom;
    u32 color;
    u32 fill;
    u32 red, green, blue;
    s16 vx, vy;
    s32 maxx, maxy;
    f32 sx, sy, width, height, x, y;
    Frame *buffer;
    s32 frame;

    {
        Gfx **setup = &D_800D8E24;

        GFX(*setup, 0xE7000000, 0);                         /* gDPPipeSync */
        GFX(*setup, 0xDE000000, (u32) D_800AB100);          /* gSPDisplayList */
        GFX(*setup, 0xDE000000, (u32) D_800AB130);
        ResetPixelScale(cam);
        /* Read twice: the ROM loads m_WarpXScale for the store and again
         * for sx (the second load becomes a mov.s after reload). */
        cam->m_PixelXScale = cam->m_WarpXScale;
        sx = cam->m_WarpXScale;
        sy = cam->m_WarpYScale;
        cam->m_PixelYScale = sy;
        maxx = (u16) D_800C2030 - 1;
        maxy = (u16) D_800C2034 - 1;
        width = cam->m_Viewport.m_Width;
        height = cam->m_Viewport.m_Height;
        x = cam->m_Viewport.m_ScreenPosX;
        y = cam->m_Viewport.m_ScreenPosY;
        ulx = (s32) x;
        uly = (s32) y;
        lrx = (s32) (x + width);
        lry = (s32) (y + height);
        vx = (s32) ((width + width) * sx);
        vy = (s32) ((height + height) * sy);
        if (D_800C2030 - 1 < lrx) lrx = maxx;
        if (ulx < 0) ulx = 0;
        if (uly < 0) uly = 0;
        if (lry > D_800C2034 - 1) lry = maxy;
        buffer = D_800F7070.m_pFrame;
        frame = D_800B6D1C;
        GFX(*setup, 0xDA380007, (u32) &cam->m_mProjection[frame]);   /* gSPMatrix */
        GFX(*setup, 0xDB0E0000, cam->m_PerspNorm);                    /* gSPPerspNormalize */
        cam->m_vp[frame].vscale[0] = vx;
        cam->m_vp[frame].vscale[1] = vy;
        cam->m_vp[frame].vtrans[0] = (s32) ((vx * 0.25f + cam->m_Viewport.m_ScreenPosX) * 4.0f);
        cam->m_vp[frame].vtrans[1] = (s32) ((vy * 0.25f + cam->m_Viewport.m_ScreenPosY) * 4.0f);
        GFX(*setup, 0xDC080008, (u32) &cam->m_vp[frame]);             /* gSPViewport */
        GFX(*setup, 0xED000000 | ((s32) (ulx * 4.0f) & 0xFFF) << 12 | ((s32) (uly * 4.0f) & 0xFFF),
            ((s32) (lrx * 4.0f) & 0xFFF) << 12 | ((s32) (lry * 4.0f) & 0xFFF));   /* gDPSetScissor */
        GFX(*setup, 0xDC08000A, (u32) &cam->m_LookAt[frame]);                        /* gSPLookAtX */
        GFX(*setup, 0xDC08030A, (u32) &cam->m_LookAt[frame] + 0x10);                 /* gSPLookAtY */
        func_00207A54();
    }
    GFX(D_800D8E24, 0xE3001201, 0x2000);                    /* gDPSetTextureFilter(bilerp) */
    if (cam != &D_801198F0 || D_801198E0 == 0) {
        Gfx **clearDl = &D_800D8E24;

        func_00207FEC(0x14);
        GFX(*clearDl, 0xE7000000, 0);
        GFX(*clearDl, 0xE3000A01, 0x300000);                /* gDPSetCycleType(G_CYC_FILL) */
        GFX(*clearDl, 0xFF100000 | ((D_800C2030 - 1) & 0xFFF), (u32) D_80130A78);   /* colour image: z buffer */
        GFX(*clearDl, 0xE7000000, 0);
        GFX(*clearDl, 0xE3000A01, 0x300000);
        GFX(*clearDl, 0xF7000000, 0xFFFCFFFC);              /* gDPSetFillColor(max z) */
        {
            Gfx *cmd = *clearDl;

            GFX(cmd, 0xF6000000 | (lrx & 0x3FF) << 14 | (lry & 0x3FF) << 2,
                (ulx & 0x3FF) << 14 | (uly & 0x3FF) << 2);  /* gDPFillRectangle */
            *clearDl = cmd;
        }
        if (D_8011AAD8.m_Flags & 0x4000) {
            rgb.red = 0;
            rgb.green = 0;
            rgb.blue = 0;
        } else {
            rgb.red = cam->m_FogColor[0] & 0xF8;
            rgb.green = cam->m_FogColor[1] & 0xF8;
            rgb.blue = cam->m_FogColor[2] & 0xF8;
        }
        if (Cinema_PathPlaying() != 0) {
            color = func_002843F4();
            if (color != 0xFF00) {
                rgb.blue = color >> 16;
                rgb.green = color >> 8;
                rgb.red = color;
            }
        }
        GFX(D_800D8E24, 0xD9FFFFFF, 0x10000);               /* gSPSetGeometryMode(G_FOG) */
        {
            Gfx *g = D_800D8E24++;                          /* gSPFogPosition(m_FogStart, 1000) */
            g->w0 = 0xDB080000;
            far = (u16) cam->m_FogStart;
            denom = 1000 - far;
            near = (500 - far) * 256;
            g->w1 = (u32) (0x1F400 / denom) << 16 | ((near / denom) & 0xFFFF);
        }
        {
            Gfx *g = D_800D8E24++;                          /* gDPSetFogColor */
            g->w0 = 0xF8000000;
            red = rgb.red;
            green = rgb.green;
            blue = rgb.blue;
            g->w1 = red << 24 | green << 16 | blue << 8 | 0xFF;
        }
        fill = ((red << 8) & 0xF800) | ((green << 3) & 0x7C0) | ((blue >> 2) & 0x3E);
        GFX(D_800D8E24, 0xFF100000 | ((D_800C2030 - 1) & 0xFFF), (u32) buffer->m_ColorImage);
        GFX(D_800D8E24, 0xF7000000, ((fill | 1) << 16 | 1) + (u16) fill);
        GFX(D_800D8E24, 0xF6000000 | (lrx & 0x3FF) << 14 | (lry & 0x3FF) << 2,
            (ulx & 0x3FF) << 14 | (uly & 0x3FF) << 2);
    }
    GFX(D_800D8E24, 0xE7000000, 0);
    GFX(D_800D8E24, 0xE3001801, 0);                         /* gDPSetAlphaDither */
    GFX(D_800D8E24, 0xE3001A01, 0x10);
    GFX(D_800D8E24, 0xE3000A01, 0x100000);                  /* gDPSetCycleType(G_CYC_2CYCLE) */
    if (cam != &D_801198F0) {
        func_002284E0(cam);
    }
    GFX(D_800D8E24, 0xE7000000, 0);
}
