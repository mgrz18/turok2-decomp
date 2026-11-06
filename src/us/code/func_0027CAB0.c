/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_002343B8.c, func_002343B8, with Turok 2's layouts. */
#include "common.h"
#include "structs/player.h"
#include "structs/camera.h"
#include "globals/game.h"
#include "globals/camera.h"
#include "globals/menu.h"
#include "globals/scene.h"

/* What a region set gives a camera (the record CScene__GetRegionInfo finds). */
typedef struct {
    /* 0x00 */ u8 m_FogColor[4];
    /* 0x04 */ u8 m_HeldFogColor[4];        /* under water, or as the player's flags ask */
    /* 0x08 */ u32 m_FogStart;
    /* 0x0C */ u32 m_HeldFogStart;
    /* 0x10 */ f32 m_FarClip;
    /* 0x14 */ f32 m_FieldOfView;
    /* 0x18 */ f32 m_HeldFarClip;
    /* 0x1C */ f32 m_HeldFieldOfView;
    /* 0x20 */ f32 m_Darkness;
    /* 0x24 */ f32 m_BlendTime;             /* in 15ths of a second */
    /* 0x28 */ u8 _pad28[0x32];
    /* 0x5A */ u8 m_Music2;
    /* 0x5B */ u8 m_Music1;
} CRegionSetAttributes;

extern s32 gFrameCountGameplay;
extern f32 gFrameIncrement;          /* the frame's time step */
CRegionSetAttributes *CScene__GetRegionInfo(s32 *list, u16 *key);
s32 Cinema_OverridesMusic1(void);
s32 Cinema_OverridesMusic2(void);
void Sound__SetAmbientTrack(u8 track);
void Music_SetTrack(u8 track);
f32 GameObject_GetViewWaterHeight(CPlayer *player);

/* CCamera__GetPlayer, inlined: the player whose camera this is, or NULL. */
static __inline__ CPlayer *GetPlayer(CCamera *cam) {
    CPlayer *player;
    CPlayer *node;

    for (player = gEngineApp_m_Game.m_PlayerList.m_ActiveList.m_pHead; player != NULL; player = player->m_pNext) {
        if (player->m_pCamera == cam) {
            return player;
        }
    }
    return NULL;
}

/* The region set a camera is in decides its fog colour, fog start, far clip
 * and field of view (LibTEngine's CCamera AdvanceAttributes); when the set
 * changes the camera blends from the old values to the new ones over the
 * set's blend time. In game mode 13 the first camera also starts the set's
 * music. */
void CCamera__AdvanceAttributes(CCamera *cam) {
    u8 target[4];
    CRegionSetAttributes *rec;
    CPlayer *player;
    CPlayer *node;
    u16 *key;
    u32 hold, freeze;
    u32 fogStart;
    f32 farClip, fov;
    f32 t, k, kn, last, v;
    f32 darkness;
    s32 dark;
    u32 c8;
    u32 r;
    s32 i;

    key = (u16 *) cam->m_ViewParams.m_pCurrentRegion;
    if (key == NULL) {
        return;
    }
    rec = CScene__GetRegionInfo(&gScene, key);
    if (rec == NULL) {
        return;
    }
    if (gEngineApp_m_Settings.paused == 0 && cam == gEngineApp_m_Game_m_CameraPool.m_Cameras && gEngineApp_m_Settings_mode == 13 && gEngineApp_m_Settings_nextMode == gEngineApp_m_Settings_mode) {
        if (Cinema_OverridesMusic1() == 0) {
            Sound__SetAmbientTrack(rec->m_Music1);
        }
        if (Cinema_OverridesMusic2() == 0) {
            Music_SetTrack(rec->m_Music2);
        }
    }
    freeze = hold = 0;
    node = GetPlayer(cam);
    if (node != NULL) {
        hold = node->m_ModeFlags & 0x40;
        hold |= node->m_CharacterActor.m_AI.m_dwFlags & 0x80;
        freeze = node->m_CharacterActor.m_AI.m_dwFlags & 0x800;
        player = node;
    } else {
        player = NULL;
    }
    if (*key != cam->m_nCurrentRegionSet || hold != 0 || freeze != 0) {
        if (cam->m_nCurrentRegionSet == -1 || hold != 0 || freeze != 0) {
            cam->m_cRegionSetBlend = 1.0f;
        } else {
            cam->m_cRegionSetBlend = 0.0f;
        }
        cam->m_nCurrentRegionSet = *key;
        for (i = 0; i < 4; i++) {
            cam->m_LastFogColor[i] = cam->m_FogColor[i];
        }
        cam->m_LastFarClip = cam->m_FarClip;
        cam->m_LastFogStart = cam->m_FogStart;
        cam->m_LastFieldOfView = cam->m_FieldOfView;
    }
    if (hold != 0 || freeze != 0) {
        if (freeze != 0) {
            for (i = 0; i < 3; i++) {
                target[i] = rec->m_HeldFogColor[i];
            }
            target[3] = 0;
        } else {
            darkness = (player != NULL) ? GameObject_GetViewWaterHeight(player) : rec->m_Darkness;
            dark = (s32) darkness / 16;
            for (i = 0; i < 3; i++) {
                target[i] = (rec->m_HeldFogColor[i] - dark < 0) ? 0 : rec->m_HeldFogColor[i] - dark;
            }
            target[3] = (rec->m_HeldFogColor[3] + dark > 0xC0) ? 0xC0 : rec->m_HeldFogColor[3] + dark;
        }
        fogStart = rec->m_HeldFogStart;
        do {
            farClip = rec->m_HeldFarClip;
            fov = rec->m_HeldFieldOfView;
        } while (0);
    } else {
        if (gCameraPoolHideViews != 0) {
            u8 *p;

            for (i = 3, p = &target[3]; i >= 0; i--) {
                *p-- = 0;
            }
        } else {
            for (i = 0; i < 3; i++) {
                target[i] = rec->m_FogColor[i];
            }
            target[3] = 0;
        }
        fogStart = rec->m_FogStart;
        farClip = rec->m_FarClip;
        fov = rec->m_FieldOfView;
    }
    if (gFrameCountGameplay == 0 || rec->m_BlendTime == 0.0f) {
        cam->m_cRegionSetBlend = 1.0f;
    } else {
        t = cam->m_cRegionSetBlend + gFrameIncrement / (rec->m_BlendTime * 15.0f);
        if (1.0f < t) {
            t = 1.0f;
        }
        cam->m_cRegionSetBlend = t;
    }
    t = cam->m_cRegionSetBlend;
    kn = t * (t * -2.0f);
    kn = kn * t;
    t = t * (t * 3.0f);
    k = kn + t;
    for (i = 0; i < 4; i++) {
        u8 *colour = (u8 *) cam + i;

        last = colour[0x50C];
        v = last + k * ((f32) target[i] - last);
        c8 = (u32) (((255.0f < v) ? 255.0f : v) < 0.0f ? 0.0f : ((255.0f < v) ? 255.0f : v));
        colour[0x508] = c8;
    }
    /* The fog start blends as a float, clamped to [0, 995]. */
#define FOG_START ((f32) (u32) cam->m_LastFogStart + k * ((f32) fogStart - (f32) (u32) cam->m_LastFogStart))
    r = (u32) (((995.0f < FOG_START) ? 995.0f : FOG_START) < 0.0f ? 0.0f : ((995.0f < FOG_START) ? 995.0f : FOG_START));
#undef FOG_START
    cam->m_FogStart = r;
    if (cam->m_ViewParams.m_ZoomBlend > 0.0f) {
        farClip = farClip + cam->m_ViewParams.m_ZoomBlend * 1024.0f;
    }
    cam->m_FarClip = cam->m_LastFarClip + k * (farClip - cam->m_LastFarClip);
    cam->m_FieldOfView = cam->m_LastFieldOfView + k * (fov - cam->m_LastFieldOfView);
}
