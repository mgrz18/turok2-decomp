#include "common.h"
#include "gfx.h"
#include "structs/camera.h"
#include "structs/game.h"
#include "structs/frame.h"


#define GFX_CMD(a, b) { Gfx *_g = D_800D8E24++; _g->w0 = (a); _g->w1 = (b); }

extern s32 D_8011ACA4;
extern f32 D_800F5A44;              /* seconds the screen is cleared for (splat: D_800F5A40 + 4) */
extern f32 gFrameIncrement;              /* frame time */
extern s32 D_800B6D1C;              /* frame buffer index */
extern s32 D_800C2030;              /* screen width */
extern s32 D_800C2034;              /* screen height */
extern Gfx *D_800D8E24;             /* display list head */
extern s32 D_80130940;
extern Gfx D_800AB100[];
extern Gfx D_800AB130[];

void CCache__LockForDisplayList(s32 unused, void *resource);
void Render_InvalidateModeCache(void);
void CCamera__DisplayListSetup(CCamera *cam);
void CCamera__DrawScene(CCamera *cam, void *world);
void CCamera__DrawTints(CCamera *cam);
void CCameraPool__AdvanceText(CCameraPool *pool, CCamera *cam);
void MenuPool_Draw(void *menuPool);
void CCamera__DrawBorder(CCamera *cam);
void CPlayerPool__DrawMatchClock(CPlayerList *list, CCamera *cam);

/* Updates every camera of the pool, then the main camera. While
 * D_800F5A44 counts down, the frame is first cleared to black. */
void CCameraPool__Draw(CCameraPool *pool, void *world) {
    CCamera *cam;
    CCamera *main;
    s32 w;
    s32 h;

    pool->m_MainCamera.m_FogColor[0] = 0;
    pool->m_MainCamera.m_FogColor[1] = 0;
    pool->m_MainCamera.m_FogColor[2] = 0;
    if (pool->m_pmeCameras != NULL) {
        CCache__LockForDisplayList(0, pool->m_pmeCameras);
    }
    if (D_8011ACA4 != 0 || pool->m_ActiveList.m_nCount == 3) {
        D_800F5A44 = 30.0f;
    }
    if (D_800F5A44 != 0.0f) {
        D_800F5A44 -= gFrameIncrement;
    }
    if (D_800F5A44 < 0.0f) {
        D_800F5A44 = 0.0f;
    }
    if (D_800F5A44 != 0.0f) {
        cam = pool->m_ActiveList.m_pHead;
        if (cam != NULL) {
            GFX_CMD(0xE7000000, 0)
            GFX_CMD(0xDE000000, (u32)D_800AB100)
            GFX_CMD(0xDE000000, (u32)D_800AB130)
            cam->m_vp[D_800B6D1C].vscale[0] = D_800C2030;
            cam->m_vp[D_800B6D1C].vscale[1] = D_800C2034;
            cam->m_vp[D_800B6D1C].vtrans[0] = D_800C2030;
            cam->m_vp[D_800B6D1C].vtrans[1] = D_800C2034;
            GFX_CMD(0xDC080008, (u32)&cam->m_vp[D_800B6D1C])
            w = D_800C2030;
            h = D_800C2034;
            GFX_CMD(0xED000000, ((((s32)((f32)(w - 1) * 4.0f)) & 0xFFF) << 12) | (((s32)((f32)(h - 1) * 4.0f)) & 0xFFF))
            GFX_CMD(0xFF100000 | ((w - 1) & 0xFFF), gEngineApp_m_pCurrentFrameData->m_pFrameBuffer)
            GFX_CMD(0xE3000A01, 0x300000)
            GFX_CMD(0xFCFFFFFF, 0xFFFE793C)
            GFX_CMD(0xE200001C, 0)
            GFX_CMD(0xF7000000, 0x10001)
            GFX_CMD(0xF6000000 | ((w & 0x3FF) << 14) | ((h & 0x3FF) << 2), 0)
            Render_InvalidateModeCache();
        }
    }
    for (cam = pool->m_ActiveList.m_pHead; cam != NULL; cam = cam->m_pNext) {
        CCamera__DisplayListSetup(cam);
        if (D_80130940 == 0) {
            CCamera__DrawScene(cam, world);
        }
        CCamera__DrawTints(cam);
        CCameraPool__AdvanceText(pool, cam);
        MenuPool_Draw(cam->m_MenuPool);
        CCamera__DrawBorder(cam);
    }
    main = &pool->m_MainCamera;
    CCamera__DisplayListSetup(main);
    if (pool->m_ActiveList.m_nCount == 0) {
        CCamera__DrawTints(main);
    }
    CCameraPool__AdvanceText(pool, main);
    if (D_80119870.m_Settings.multiplayer != 0) {
        cam = pool->m_ActiveList.m_pHead;
        if (cam != NULL) {
            CPlayerPool__DrawMatchClock(&D_80119870.m_PlayerList, cam);
        }
    }
    MenuPool_Draw(pool->m_MainCamera.m_MenuPool);
}
