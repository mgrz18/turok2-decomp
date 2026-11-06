#include "common.h"
#include "structs/player.h"
#include "structs/camera.h"
#include "structs/game.h"
#include "globals/player.h"

extern CPlayer *gEngineApp_m_Game_m_PlayerList_m_ActiveList_m_pHead;     /* the players, a list through m_pNext */
extern u32 sTintFlags;          /* the camera player's AI flags this frame, */
extern u32 sTintFlagsPrev;          /* the frame before, */
extern u32 sTintFlagsPrev2;          /* and the one before that */
extern u8 gDeathTintColor[4];        /* its colour */
extern f32 gFrameIncrement;          /* the frame's time step */
void CEngineApp__DrawFilledRect(void *ctx, u8 *rgba, s32 left, s32 top, s32 right, s32 bottom);
void CCamera__UpdateFlash(CCamera *cam);
s32 Cinema_PathPlaying(void);
void CPlayerPool__DrawHuds(CPlayerPool *list, CCamera *cam);
s32 Cinema_DrawOverlay(CCamera *cam);

/* The tints over a camera's view (LibTEngine's CCamera DrawTints): the fog
 * colour while its player is under water (AI flags 0x880) or a forced
 * tint is up, then the screen flash, then the red flash of a hit fading at
 * m_FlashDec a frame. */
s32 CCamera__DrawTints(CCamera *cam) {
    CPlayer *player;
    u32 flags;
    u8 tint[4];
    u8 hurt[4];
    u8 *rgba;
    s32 i;

    /* CCamera__GetPlayer, inlined */
    for (player = gEngineApp_m_Game_m_PlayerList_m_ActiveList_m_pHead; player != NULL; player = player->m_pNext) {
        if (player->m_pCamera == (void *) cam) {
            goto found;
        }
    }
    player = NULL;
found:
    if (player != NULL) {
        flags = player->m_CharacterActor.m_AI.m_dwFlags;
        sTintFlagsPrev2 = sTintFlagsPrev;
        sTintFlagsPrev = sTintFlags;
        sTintFlags = flags;
        if ((flags & 0x880) || gDeathTint != 0) {
            tint[0] = cam->m_FogColor[0] & 0xE0;
            tint[1] = cam->m_FogColor[1] & 0xE0;
            tint[2] = cam->m_FogColor[2] & 0xE0;
            tint[3] = cam->m_FogColor[3] & 0xE0;
            if (gDeathTint != 0) {
                for (i = 0; i < 4; i++) {
                    tint[i] = gDeathTintColor[i];
                }
            }
            if (gEngineApp_m_Settings.cheatFlags & 0x4000) {
                tint[0] >>= 3;
                tint[1] >>= 3;
                tint[2] >>= 3;
            }
            CEngineApp__DrawFilledRect(&gEngineApp, tint, (s32) cam->m_Viewport.m_ScreenPosX, (s32) cam->m_Viewport.m_ScreenPosY,
                          (s32) (cam->m_Viewport.m_ScreenPosX + cam->m_Viewport.m_Width),
                          (s32) (cam->m_Viewport.m_ScreenPosY + cam->m_Viewport.m_Height));
        }
        CCamera__UpdateFlash(cam);
        if (cam->m_FlashMode != 0) {
            CEngineApp__DrawFilledRect(&gEngineApp, cam->m_FlashColor, (s32) cam->m_Viewport.m_ScreenPosX,
                          (s32) cam->m_Viewport.m_ScreenPosY,
                          (s32) (cam->m_Viewport.m_ScreenPosX + cam->m_Viewport.m_Width),
                          (s32) (cam->m_Viewport.m_ScreenPosY + cam->m_Viewport.m_Height));
        }
        rgba = hurt;
        if (cam->m_Flash > cam->m_FlashDec) {
            cam->m_Flash -= (u32) (cam->m_FlashDec * gFrameIncrement);
        } else {
            cam->m_Flash = 0;
        }
        if (cam->m_Flash != 0) {
            rgba[0] = 0xFF;
            hurt[1] = 0;
            hurt[2] = 0;
            hurt[3] = cam->m_Flash;
            CEngineApp__DrawFilledRect(&gEngineApp, hurt, (s32) cam->m_Viewport.m_ScreenPosX, (s32) cam->m_Viewport.m_ScreenPosY,
                          (s32) (cam->m_Viewport.m_ScreenPosX + cam->m_Viewport.m_Width),
                          (s32) (cam->m_Viewport.m_ScreenPosY + cam->m_Viewport.m_Height));
        }
        if (Cinema_PathPlaying() == 0 && gEngineApp_m_Game.m_Settings.mode == 13) {
            CPlayerPool__DrawHuds(&gEngineApp_m_Game.m_PlayerList, cam);
        }
        return Cinema_DrawOverlay(cam);
    }
}
