#include "common.h"
#include "gfx.h"
#include "gbi_t2.h"
#include "structs/camera.h"
#include "structs/game.h"
#include "globals/game.h"
#include "protos/scene.h"
#ifdef TARGET_PORT
#include "structs/world.h"
#endif


extern u8 gEngineApp_m_SwooshSystem[];
void func_002847D0(void);
s32 Cinema_PathPlaying(void);
void CPlayerPool__ReloadAnimInfo(CPlayerPool *list, CCamera *cam);
void CPlayerPool__DrawCharacters(CPlayerPool *list, CCamera *cam);
void func_00235C30(void *arg0, CCamera *cam);
void CLevelActorPool__Draw(u8 *arg0, CCamera *cam);
void CEmitterPool__Draw(u8 *sky, CCamera *cam);
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
        CPlayerPool__ReloadAnimInfo(&gEngineApp_m_Game.m_PlayerList, cam);
    }
    if (gEngineApp_m_Game.m_Settings.mode == 13) {
        CPlayerPool__DrawCharacters(&gEngineApp_m_Game.m_PlayerList, cam);
    }
    CScene__DrawRegisteredInstances(world, cam);
    func_00235C30(gEngineApp_m_SwooshSystem, cam);
    if (Cinema_PathPlaying() == 0) {
#ifdef TARGET_PORT
        CLevelActorPool__Draw((u8 *) &((CScene *) world)->m_ActorPool, cam);
#else
        CLevelActorPool__Draw(world + 0x10518, cam);
#endif
    }
    CScene__DrawSortedList(world, cam);
    CEmitterPool__Draw(cam->m_SkySystem, cam);
    CScene__DrawParticles(world, cam);
    Trail_DrawAll(gEngineApp_m_SwooshSystem, cam);
    CCamera__DrawGlares(cam);
    if (Cinema_PathPlaying() == 0 && gEngineApp_m_Game.m_Settings.mode == 13) {
        CPlayerPool__DrawWeaponOverlays(&gEngineApp_m_Game.m_PlayerList, cam);
    }
    if (gEngineApp_m_Settings.cheatFlags & 0x200) {
        GFX_CMD(0xE7000000, 0);                   /* gDPPipeSync */
        GFX_CMD(0xE3000A01, 0);                   /* gDPSetCycleType(G_CYC_FILL) */
        GFX_CMD(0xF9000000, 0xFFFFFFFF);          /* gDPSetBlendColor */
        GFX_CMD(0xEE000000, 0xFFFFFFFF);          /* gDPSetPrimDepth */
        GFX_CMD(0xE2001D00, 4);                   /* gDPSetRenderMode, z compare */
        Render_SetRenderMode(0x16);
        {                                           /* gDPFillRectangle over the viewport */
        Gfx *g = gDList++;
        g->w0 = 0xF6000000
              | ((u32) (cam->m_Viewport.m_ScreenPosX + cam->m_Viewport.m_Width) & 0x3FF) << 14
              | ((u32) (cam->m_Viewport.m_ScreenPosY + cam->m_Viewport.m_Height) & 0x3FF) << 2;
        g->w1 = ((u32) cam->m_Viewport.m_ScreenPosX & 0x3FF) << 14
              | ((u32) cam->m_Viewport.m_ScreenPosY & 0x3FF) << 2;
        }
    }
}
