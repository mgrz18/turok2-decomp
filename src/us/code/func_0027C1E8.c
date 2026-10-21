#include "common.h"
#include "structs/player.h"
#include "structs/camera.h"

extern CPlayer *D_80119890;     /* the players, a list through m_pNext */
extern u32 D_800B6CB8;          /* the camera player's AI flags this frame, */
extern u32 D_800B6CBC;          /* the frame before, */
extern u32 D_800B6CC0;          /* and the one before that */
extern s32 D_800AE340;          /* a tint forced over the view */
extern u8 D_800F1FE4[4];        /* its colour */
extern s32 D_8011AAD8;          /* game flags */
extern u8 D_800F6CB0[];
extern f32 D_800B6D28;          /* the frame's time step */
/* The global at D_80119870: what these read of it. */
typedef struct {
    /* 0x0000 */ u8 _pad[0x1418];
    /* 0x1418 */ s32 m_Mode;           /* D_8011AC88: game mode */
} Game80119870;
extern Game80119870 D_80119870;
void func_002883E0(void *ctx, u8 *rgba, s32 left, s32 top, s32 right, s32 bottom);
void func_0027C074(CCamera *cam);
s32 Cinema_PathPlaying(void);
void func_0025322C(Game80119870 *game, CCamera *cam);
s32 func_00284284(CCamera *cam);

/* The tints over a camera's view (LibTEngine's CCamera DrawTints): the fog
 * colour while its player is under water (AI flags 0x880) or a forced
 * tint is up, then the screen flash, then the red flash of a hit fading at
 * m_FlashDec a frame. */
s32 func_0027C1E8(CCamera *cam) {
    CPlayer *player;
    u32 flags;
    u8 tint[4];
    u8 hurt[4];
    u8 *rgba;
    s32 i;

    /* func_00281194, inlined */
    for (player = D_80119890; player != NULL; player = player->m_pNext) {
        if (player->m_pCamera == (void *) cam) {
            goto found;
        }
    }
    player = NULL;
found:
    if (player != NULL) {
        flags = player->m_CharacterActor.m_AI.m_dwFlags;
        D_800B6CC0 = D_800B6CBC;
        D_800B6CBC = D_800B6CB8;
        D_800B6CB8 = flags;
        if ((flags & 0x880) || D_800AE340 != 0) {
            tint[0] = cam->m_FogColor[0] & 0xE0;
            tint[1] = cam->m_FogColor[1] & 0xE0;
            tint[2] = cam->m_FogColor[2] & 0xE0;
            tint[3] = cam->m_FogColor[3] & 0xE0;
            if (D_800AE340 != 0) {
                for (i = 0; i < 4; i++) {
                    tint[i] = D_800F1FE4[i];
                }
            }
            if (D_8011AAD8 & 0x4000) {
                tint[0] >>= 3;
                tint[1] >>= 3;
                tint[2] >>= 3;
            }
            func_002883E0(D_800F6CB0, tint, (s32) cam->m_Viewport.m_ScreenPosX, (s32) cam->m_Viewport.m_ScreenPosY,
                          (s32) (cam->m_Viewport.m_ScreenPosX + cam->m_Viewport.m_Width),
                          (s32) (cam->m_Viewport.m_ScreenPosY + cam->m_Viewport.m_Height));
        }
        func_0027C074(cam);
        if (cam->m_FlashMode != 0) {
            func_002883E0(D_800F6CB0, cam->m_FlashColor, (s32) cam->m_Viewport.m_ScreenPosX,
                          (s32) cam->m_Viewport.m_ScreenPosY,
                          (s32) (cam->m_Viewport.m_ScreenPosX + cam->m_Viewport.m_Width),
                          (s32) (cam->m_Viewport.m_ScreenPosY + cam->m_Viewport.m_Height));
        }
        rgba = hurt;
        if (cam->m_Flash > cam->m_FlashDec) {
            cam->m_Flash -= (u32) (cam->m_FlashDec * D_800B6D28);
        } else {
            cam->m_Flash = 0;
        }
        if (cam->m_Flash != 0) {
            rgba[0] = 0xFF;
            hurt[1] = 0;
            hurt[2] = 0;
            hurt[3] = cam->m_Flash;
            func_002883E0(D_800F6CB0, hurt, (s32) cam->m_Viewport.m_ScreenPosX, (s32) cam->m_Viewport.m_ScreenPosY,
                          (s32) (cam->m_Viewport.m_ScreenPosX + cam->m_Viewport.m_Width),
                          (s32) (cam->m_Viewport.m_ScreenPosY + cam->m_Viewport.m_Height));
        }
        if (Cinema_PathPlaying() == 0 && D_80119870.m_Mode == 13) {
            func_0025322C(&D_80119870, cam);
        }
        return func_00284284(cam);
    }
}
