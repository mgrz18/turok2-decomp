#include "common.h"
#include "gfx.h"
#include "structs/camera.h"

#define GFX_WORDS(a, b) { Gfx *_g = D_800D8E24++; _g->w0 = (a); _g->w1 = (b); }

extern s32 D_8011AAD8;          /* game flags */
/* The global at D_80119870: what this reads of it. */
typedef struct {
    /* 0x0000 */ u8 _pad[0x1418];
    /* 0x1418 */ s32 m_Mode;           /* D_8011AC88: game mode */
} Game80119870;
extern Game80119870 D_80119870;
extern u8 D_801102B8[];
void func_002847D0(void);
void func_002222E0(u8 *world, CCamera *cam);
s32 Cinema_PathPlaying(void);
void func_00252C70(Game80119870 *game, CCamera *cam);
void func_00253148(Game80119870 *game, CCamera *cam);
void func_00223F8C(u8 *world, CCamera *cam);
void func_00235C30(void *arg0, CCamera *cam);
void func_0022782C(u8 *arg0, CCamera *cam);
void func_0022077C(u8 *world, CCamera *cam);
void func_00283238(u8 *sky, CCamera *cam);
void func_002240C4(u8 *world, CCamera *cam);
void func_002352F8(void *arg0, CCamera *cam);
void func_00280AA0(CCamera *cam);
void func_002531D0(Game80119870 *game, CCamera *cam);
void func_00207FEC(s32 arg0);

/* Draws the scene through a camera, layer by layer (LibTEngine's CCamera
 * DrawScene), and with game flag 0x200 up clears its viewport after: a
 * fill rectangle in fill mode, blend colour and prim depth all ones. */
void func_0027E770(CCamera *cam, u8 *world) {
    func_002847D0();
    func_002222E0(world, cam);
    if (Cinema_PathPlaying() == 0) {
        func_00252C70(&D_80119870, cam);
    }
    if (D_80119870.m_Mode == 13) {
        func_00253148(&D_80119870, cam);
    }
    func_00223F8C(world, cam);
    func_00235C30(D_801102B8, cam);
    if (Cinema_PathPlaying() == 0) {
        func_0022782C(world + 0x10518, cam);
    }
    func_0022077C(world, cam);
    func_00283238(cam->m_SkySystem, cam);
    func_002240C4(world, cam);
    func_002352F8(D_801102B8, cam);
    func_00280AA0(cam);
    if (Cinema_PathPlaying() == 0 && D_80119870.m_Mode == 13) {
        func_002531D0(&D_80119870, cam);
    }
    if (D_8011AAD8 & 0x200) {
        GFX_WORDS(0xE7000000, 0);                   /* gDPPipeSync */
        GFX_WORDS(0xE3000A01, 0);                   /* gDPSetCycleType(G_CYC_FILL) */
        GFX_WORDS(0xF9000000, 0xFFFFFFFF);          /* gDPSetBlendColor */
        GFX_WORDS(0xEE000000, 0xFFFFFFFF);          /* gDPSetPrimDepth */
        GFX_WORDS(0xE2001D00, 4);                   /* gDPSetRenderMode, z compare */
        func_00207FEC(0x16);
        {                                           /* gDPFillRectangle over the viewport */
        Gfx *g = D_800D8E24++;
        g->w0 = 0xF6000000
              | ((u32) (cam->m_Viewport.m_ScreenPosX + cam->m_Viewport.m_Width) & 0x3FF) << 14
              | ((u32) (cam->m_Viewport.m_ScreenPosY + cam->m_Viewport.m_Height) & 0x3FF) << 2;
        g->w1 = ((u32) cam->m_Viewport.m_ScreenPosX & 0x3FF) << 14
              | ((u32) cam->m_Viewport.m_ScreenPosY & 0x3FF) << 2;
        }
    }
}
