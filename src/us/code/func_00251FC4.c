#include "common.h"
#include "structs/vector.h"
#include "structs/player.h"
#include "structs/controller.h"
#include "structs/camera.h"
#include "globals/game.h"
#include "globals/camera.h"
#include "globals/scene.h"

typedef struct {
    /* 0x00 */ CPlayer *head;
    /* 0x04 */ u8 pad4[0x10 - 4];
    /* 0x10 */ u32 count;
} PlayerCList;

typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ PlayerCList idle;        /* players not in the game */
    /* 0x20 */ PlayerCList active;      /* players in the game */
    /* 0x34 */ s32 cinema;
    /* 0x38 */ s32 cinemaStarted;
} PlayerList;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ f32 timeLeft;
    /* 0x18 */ s32 fragLimit;
    /* 0x1C */ s32 over;
} MatchState;

/* The globals from gEngineApp_m_Game_m_MenuPool: a menu pool first. */
typedef struct {
    /* 0x000 */ void *menus;            /* CMenuPool */
    /* 0x004 */ u8 pad4[0xCC5 - 4];
    /* 0xCC5 */ u8 timed;               /* gEngineApp_m_Settings_multiplayer */
    /* 0xCC6 */ u8 padCC6[0xE78 - 0xCC6];
    /* 0xE78 */ s32 paused;             /* gEngineApp_m_Settings_paused */
    /* 0xE7C */ u8 padE7C[0xE84 - 0xE7C];
    /* 0xE84 */ MatchState match;       /* gEngineApp_m_Settings_match */
} Globals;

typedef struct {
    /* 0x00 */ s8 character;
    /* 0x01 */ u8 pad1[0x40 - 1];
} PortSetup;

extern Globals gEngineApp_m_Game_m_MenuPool; /* local type: globals/menu.h declares it; this view (its type here) is what keeps the ROM's code */
extern f32 gFrameIncrement;          /* frame time */
extern PortSetup D_8011AB88[];
extern char gPauseMenuDesc;         /* death screen */
extern char gSavePortalMenuDesc;
extern char gMissionFailedMenuDesc;         /* key screen */

void CPlayer__Update(CPlayer *p);
void CPlayer__UpdateOrbitCamera(CPlayer *p, CPlayer *other);
void Automap_SetPlayer(void *, s32 region, f32 yaw, CVector3 pos);
void CPlayer__ReadInput(void *input, CPlayer *p);
s32 Controller_StartPressedNoCinema(ControllerProfile *c);
s32 Cinema_IsActive(void);
void Cinema_TrySkip(void);
s32 MenuPool_CountOpen(Globals *g);
void Music_DuckVolume(void);
void Sound__StopAllExcept(s32);
void Sound__DisableAmbientTrack(void);
void MenuPool_OpenMenu(void *pool, void *screen, CPlayer *p, ControllerProfile *c, s32);
s32 Level_GetHub(s32 level);
s32 Cinema_PathPlaying(void);
s32 Cinema_GetScriptId(void);
s32 Cinema_IsFinished(void);
void CPlayer__NewGame(CPlayer *p, s32 character, s32);
void CAI__SetMode(void *actor, void *ai, s32 mode);
void CPlayer__Warp(CPlayer *p, s32, s32, s32);
void CList__Remove(void *list, void *node);
void CList__AddHead(void *list, void *node);
void CCameraPool__FreeCamera(void *pool, CCamera *cam);
void CPlayerPool__UpdateMatchClock(PlayerList *list);
void CPlayerPool__PickTagIt(PlayerList *list);

static __inline__ void reset_strokes(CBarrel *s) {
    s->m_State = 0;
    s->m_Turn = 0.0f;
    s->m_nCursor = -1;
    s->m_Open = 0.0f;
}

/* CPlayer__IsPausedOrInMenu: paused, or a menu other than the death screen open on the
 * player's camera. */
static __inline__ s32 in_menu(CPlayer *p) {
    s32 result;

    if (gEngineApp_m_Settings.paused) {
        return 1;
    }
    result = 0;
    if (p->m_pCamera != NULL && p->m_pCamera->m_Menus.head != NULL) {
        result = p->m_pCamera->m_Menus.head->desc != (MenuDesc *) &gPauseMenuDesc;
    }
    return result;
}

/* Update the players: move each one in the game (unless paused or in a menu:
 * CPlayer__IsPausedOrInMenu), handle pause (the death screen), the hub key screen and the level-end
 * cinema; let idle players join while the match allows it and send players
 * who left back to the idle list; then run the match clock and tag. */
void CPlayerPool__Update(PlayerList *list) {
    CPlayer *p;
    CPlayer *next;
    f32 frameTime;
    u32 hub;
    s32 character;

    if (gEngineApp_m_Settings.nextMode == 0xB || gEngineApp_m_Settings.nextMode == 8) {
        return;
    }
    frameTime = gFrameIncrement;
    for (p = list->active.head; p != NULL; p = p->m_pNext) {
        if (!in_menu(p)) {
            CPlayer__Update(p);
        }
        CPlayer__UpdateOrbitCamera(p, p);
        Automap_SetPlayer(&gEngineApp_m_Game.m_Automap, p->m_CharacterActor.header.m_pCurrentRegion, p->m_CharacterActor.m_RotY, p->m_CharacterActor.header.m_vPos);
    }
    for (p = list->active.head; p != NULL; p = p->m_pNext) {
        CPlayer__ReadInput(&p->m_ContState, p);
        if (Controller_StartPressedNoCinema(p->m_ContState.m_pContState)) {
            if (Cinema_IsActive()) {
                Cinema_TrySkip();
            } else if (!MenuPool_CountOpen(&gEngineApp_m_Game_m_MenuPool) && p->m_Data.m_Health != 0 && p->m_pCamera != NULL
                       && p->m_pCamera->m_Menus.count == 0 && (gEngineApp_m_Game_m_MenuPool.match.over == 0 || gEngineApp_m_Game_m_MenuPool.timed == 0)) {
                gEngineApp_m_Game_m_MenuPool.paused = 1;
                Music_DuckVolume();
                Sound__StopAllExcept(300);
                Sound__DisableAmbientTrack();
                reset_strokes(&p->m_Barrel);
                p->m_LivesOverlay.m_Value = -1;
                p->m_TokensOverlay.m_Value = -1;
                MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, &gPauseMenuDesc, p, (p->m_ContState.m_pContState), 0);
            }
        }
        if (p->m_bLevelNotCompleteExit) {
            hub = Level_GetHub(gScene_m_LevelID) - 1;
            if (hub < 6 && (p->m_Data.m_dwLevelFirstTimeFlags & (1 << hub))) {
                MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, &gMissionFailedMenuDesc, p, (p->m_ContState.m_pContState), 0);
            }
            p->m_bLevelNotCompleteExit = 0;
        }
        if (!MenuPool_CountOpen(&gEngineApp_m_Game_m_MenuPool) && Cinema_PathPlaying() && Cinema_GetScriptId() == 400 && !list->cinema) {
            if (p->m_pCamera != NULL && p->m_pCamera->m_Menus.count == 0) {
                list->cinema = 1;
                list->cinemaStarted = 0;
            }
        }
        if (list->cinema) {
            if (Cinema_IsFinished() && !list->cinemaStarted) {
                list->cinemaStarted = 1;
                MenuPool_OpenMenu(p->m_pCamera->m_MenuPool, &gSavePortalMenuDesc, p, (p->m_ContState.m_pContState), 0);
            }
        }
    }
    for (p = list->idle.head; p != NULL; p = next) {
        MatchState *match;

        CPlayer__ReadInput(&p->m_ContState, p);
        next = p->m_pNext;
        match = &gEngineApp_m_Game_m_MenuPool.match;
        if (p != NULL && !match->over && (match->timeLeft >= 900.0f || match->timeLeft < 0.0f || !gEngineApp_m_Game_m_MenuPool.timed)
            && ((p->m_ContState.m_pContState->buttonsSingle & 0x1000) || p->m_bActive)) {
            p->m_bActive = 1;
            character = D_8011AB88[p->m_nController].character;
            p->m_pOptions->m_bActive = 1;
            CPlayer__NewGame(p, character, 1);
            CAI__SetMode(&p->m_WeaponActor, &p->m_WeaponActor.m_AI, 2);
            CPlayer__Warp(p, gEngineApp_m_Settings.matchMode, 0, 1);
            CList__Remove(&list->idle, p);
            CList__AddHead(&list->active, p);
        }
    }
    for (p = list->active.head; p != NULL; p = next) {
        next = p->m_pNext;
        if (p != NULL && !p->m_bActive && list->active.count >= 2) {
            if (p->m_pCamera != NULL) {
                CCameraPool__FreeCamera(&gEngineApp_m_Game_m_CameraPool, p->m_pCamera);
                p->m_pCamera = NULL;
            }
            CList__Remove(&list->active, p);
            CList__AddHead(&list->idle, p);
        }
    }
    CPlayerPool__UpdateMatchClock(list);
    CPlayerPool__PickTagIt(list);
    gFrameIncrement = frameTime;
}
