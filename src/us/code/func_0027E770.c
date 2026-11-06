#include "common.h"
#include "gfx.h"
#include "structs/camera.h"
#include "structs/game.h"

#define GFX_WORDS(a, b) { Gfx *_g = D_800D8E24++; _g->w0 = (a); _g->w1 = (b); }

extern u8 D_801102B8[];
void func_002847D0(void);
void CScene__DrawGeometry(u8 *world, CCamera *cam);
s32 Cinema_PathPlaying(void);
void CPlayerPool__ReloadAnimInfo(CPlayerPool *list, CCamera *cam);
void CPlayerPool__DrawCharacters(CPlayerPool *list, CCamera *cam);
void CScene__DrawRegisteredInstances(u8 *world, CCamera *cam);
void func_00235C30(void *arg0, CCamera *cam);
void CLevelActorPool__Draw(u8 *arg0, CCamera *cam);
void CScene__DrawSortedList(u8 *world, CCamera *cam);
void CEmitterPool__Draw(u8 *sky, CCamera *cam);
void CScene__DrawParticles(u8 *world, CCamera *cam);
void Trail_DrawAll(void *arg0, CCamera *cam);
void CCamera__DrawGlares(CCamera *cam);
void CPlayerPool__DrawWeaponOverlays(CPlayerPool *list, CCamera *cam);
void Render_SetRenderMode(s32 arg0);

/* Draws the scene through a camera, layer by layer (LibTEngine's CCamera
 * DrawScene), and with game flag 0x200 up clears its viewport after: a
 * fill rectangle in fill mode, blend colour and prim depth all ones. */
void CCamera__DrawScene(CCamera *cam, u8 *world) {
    func_002847D0();
    CScene__DrawGeometry(world, cam);
    if (Cinema_PathPlaying() == 0) {
        CPlayerPool__ReloadAnimInfo(&D_80119870.m_PlayerList, cam);
    }
    if (D_80119870.m_Settings.mode == 13) {
        CPlayerPool__DrawCharacters(&D_80119870.m_PlayerList, cam);
    }
    CScene__DrawRegisteredInstances(world, cam);
    func_00235C30(D_801102B8, cam);
    if (Cinema_PathPlaying() == 0) {
        CLevelActorPool__Draw(world + 0x10518, cam);
    }
    CScene__DrawSortedList(world, cam);
    CEmitterPool__Draw(cam->m_SkySystem, cam);
    CScene__DrawParticles(world, cam);
    Trail_DrawAll(D_801102B8, cam);
    CCamera__DrawGlares(cam);
    if (Cinema_PathPlaying() == 0 && D_80119870.m_Settings.mode == 13) {
        CPlayerPool__DrawWeaponOverlays(&D_80119870.m_PlayerList, cam);
    }
    if (D_8011AAD8.cheatFlags & 0x200) {
        GFX_WORDS(0xE7000000, 0);                   /* gDPPipeSync */
        GFX_WORDS(0xE3000A01, 0);                   /* gDPSetCycleType(G_CYC_FILL) */
        GFX_WORDS(0xF9000000, 0xFFFFFFFF);          /* gDPSetBlendColor */
        GFX_WORDS(0xEE000000, 0xFFFFFFFF);          /* gDPSetPrimDepth */
        GFX_WORDS(0xE2001D00, 4);                   /* gDPSetRenderMode, z compare */
        Render_SetRenderMode(0x16);
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
