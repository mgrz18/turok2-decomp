/* t2debug.c: DEBUG-ONLY bring-up switches for `turok2 --game` (docs/port/STATUS.md, round 4 s2).
 *
 * The game cannot leave mode 0 (the Controller Pak check) until the PIF/Controller Pak is
 * modelled (s1, docs/port/INPUT.md). To follow the boot past it these switches move the engine
 * on by itself. They are bring-up aids, not options for players, and go away once input works:
 *
 *   --skip-pak-check  mode 0 ends as if "continue without saving" had been chosen: gPakCheckDone
 *                     is set once mode 0 has started, so EngineMode00_PakCheck_Retrace takes its
 *                     own exit (CEngineApp__SetupFadeTo(app, 1): the legal screen, then the
 *                     modes the game chooses itself).
 *   --start-mode N    once mode 0 has started, switch to mode N (CEngineApp__SetMode: through
 *                     mode 17 like any switch). 7 is the title screen.
 *
 * Called from game.cpp between retraces, while every game thread is blocked. */
#include "common.h"
#include "structs/game.h"
#include "globals/pak.h"
#include "globals/engine.h"

void CEngineApp__SetMode(CEngineApp *app, s32 mode);

int T2Port_EngineMode(void) {
    return gEngineApp.m_Settings.mode;
}

/* skipPak: 1 for --skip-pak-check; startMode: -1, or the mode for --start-mode. */
void T2Port_DebugSwitches(int skipPak, int startMode) {
    static int sStartDone;

    if (gEngineApp.m_Settings.mode != 0 || gPakCheckStarted == 0) {
        return;
    }
    if (startMode >= 0 && !sStartDone) {
        sStartDone = 1;
        CEngineApp__SetMode(&gEngineApp, startMode);
        return;
    }
    if (skipPak) {
        gPakCheckDone = 1;
    }
}

/* T2_STATE_LOG=1 (game.cpp, every 60 frames): the level and the first active player
 * (position, facing, health, its body's AI mode, weapon), to follow gameplay headless. */
#include "structs/world.h"
#include "structs/player.h"
#include "globals/game.h"
#include "globals/scene.h"
#include "structs/camera.h"
#include "structs/menu.h"
#include "structs/controller.h"
#include <stdio.h>
#include <stdlib.h>
#include "structs/rect.h"
#include "globals/engine.h"
#include "structs/weapon.h"

void T2Port_LogState(int frame) {
    CPlayer *p = gEngineApp_m_Game.m_PlayerList.m_ActiveList.m_pHead;

    printf("state: frame %d mode %d level %d warp %d paused %d", frame, (int) gEngineApp.m_Settings.mode,
           (int) gScene.m_nLevel, (int) gScene.m_WarpState, (int) gEngineApp.m_Settings.paused);
    if (p != NULL) {
        CGameObjectInstance *a = &p->m_CharacterActor;
        printf(" player pos (%.1f %.1f %.1f) rotY %.2f health %d aimode %d weapon %d",
               a->header.m_vPos.x, a->header.m_vPos.y, a->header.m_vPos.z, a->m_RotY,
               (int) (p->m_Data.m_Health >> 8), (int) a->m_AI.m_Mode, (int) p->m_Data.m_CurrentWeapon);
        if (a->header.m_pCurrentRegion != NULL) {
            extern f32 CGameRegion__GetHeight(void *, f32, f32);
            printf(" region %p floor %.1f", (void *) a->header.m_pCurrentRegion,
                   CGameRegion__GetHeight(a->header.m_pCurrentRegion, a->header.m_vPos.x, a->header.m_vPos.z));
        } else {
            printf(" region NULL");
        }
        printf(" lives %d", (int) p->m_Data.m_Lives);
        {
            /* round 7 (v2): the current weapon's ammo, what the HUD counter shows */
            extern s32 CPlayer__GetWeaponAmmo(CPlayer *, s32);
            printf(" ammo %d", (int) CPlayer__GetWeaponAmmo(p, p->m_Data.m_CurrentWeapon));
        }
        /* weapons (round 6): the requested weapon, the wheel (state, count, cursor), the pressed
         * bits of this frame */
        printf(" reqw %d wheel %d/%d/%d single %x", (int) p->m_RequestedWeapon, (int) p->m_Barrel.m_State,
               (int) p->m_Barrel.m_nCount, (int) p->m_Barrel.m_nCursor, (unsigned) p->m_ContState.m_dwButtonsSingle);
        /* the weapon actor: its AI mode, animation repeats, animation index and frame */
        printf(" wact %d/%d anim %d %.1f", (int) p->m_WeaponActor.m_AI.m_Mode,
               (int) p->m_WeaponActor.m_AI.m_AnimRepeatedCount,
               (int) p->m_WeaponActor.m_CurrentAnimation.m_nAnimIndex,
               p->m_WeaponActor.m_CurrentAnimation.m_AnimationFrame);
        printf(" pmode %d buttons %x move (%.2f %.2f) look (%.2f %.2f) contp %p", (int) p->m_Mode,
               (unsigned) p->m_ContState.m_dwButtons, p->m_ContState.m_MoveX, p->m_ContState.m_MoveY,
               p->m_ContState.m_LookX, p->m_ContState.m_LookY, (void *) p->m_ContState.m_pContState);
        if (p->m_ContState.m_pContState != NULL) {
            ControllerProfile *c = p->m_ContState.m_pContState;
            printf(" pad %x/%x stick %d %d", (unsigned) c->inputButtons, (unsigned) c->buttons, c->stickX, c->stickY);
        }
    }
    if (p != NULL && p->m_pCamera != NULL && ((MenuPool *) p->m_pCamera->m_MenuPool)->head != NULL) {
        Menu *m = ((MenuPool *) p->m_pCamera->m_MenuPool)->head;
        printf(" cammenu %p cursor %d", (void *) m->desc, (int) m->cursor);
    }
    {
        extern s32 gCameraPoolHideViews;
        CCameraPool *pool = gEngineApp.m_Game.m_CameraPool;
        printf(" cams %d hide %d", (int) pool->m_ActiveList.m_nCount, (int) gCameraPoolHideViews);
    }
    printf("\n");
    if (getenv("T2_SCENE_LOG") != NULL) {
        /* T2_SCENE_LOG=1 (with T2_STATE_LOG): the view of the first player's camera, what
         * CScene__DrawGeometry culls with, and the loaded grid sections */
        CCamera *c = (p != NULL) ? p->m_pCamera : NULL;
        int i;
        if (c != NULL) {
            printf("scene: cam pos (%.1f %.1f %.1f) view (%.1f %.1f %.1f)-(%.1f %.1f %.1f) far %.1f fov %.1f vis %x region %p\n",
                   c->m_vPos.x, c->m_vPos.y, c->m_vPos.z, c->m_ViewMin.x, c->m_ViewMin.y, c->m_ViewMin.z,
                   c->m_ViewMax.x, c->m_ViewMax.y, c->m_ViewMax.z, c->m_FarClip, c->m_FieldOfView,
                   (unsigned) c->m_ViewParams.m_VisBits, (void *) c->m_ViewParams.m_pCurrentRegion);
        }
        {
            extern CBox gCameraVisibleBox;
            extern CBox gCameraViewBox;
            printf("scene: visible box (%.1f %.1f %.1f)-(%.1f %.1f %.1f) view box (%.1f %.1f %.1f)-(%.1f %.1f %.1f)\n",
                   gCameraVisibleBox.m_vMin.x, gCameraVisibleBox.m_vMin.y, gCameraVisibleBox.m_vMin.z,
                   gCameraVisibleBox.m_vMax.x, gCameraVisibleBox.m_vMax.y, gCameraVisibleBox.m_vMax.z,
                   gCameraViewBox.m_vMin.x, gCameraViewBox.m_vMin.y, gCameraViewBox.m_vMin.z,
                   gCameraViewBox.m_vMax.x, gCameraViewBox.m_vMax.y, gCameraViewBox.m_vMax.z);
            if (c != NULL) {
                printf("scene: cam anim (%.1f %.1f %.1f)-(%.1f %.1f %.1f) next %p\n", c->m_AnimMin.x, c->m_AnimMin.y,
                       c->m_AnimMin.z, c->m_AnimMax.x, c->m_AnimMax.y, c->m_AnimMax.z, (void *) c->m_pNext);
            }
        }
        printf("scene: level %d sectors %d difficulty %x multiplayer %d cheats %x\n", (int) gScene.m_nLevel,
               (int) gScene.m_nSectors, (unsigned) gEngineApp_m_Settings.difficulty,
               (int) gEngineApp_m_Settings.multiplayer, (unsigned) gEngineApp_m_Settings.cheatFlags);
        for (i = 0; i < gScene.m_nSectors && i < 8; i++) {
            CRect *r = (CRect *) gScene.m_Sectors[i].m_pBounds;
            printf("scene:   sector %d x %.0f..%.0f z %.0f..%.0f\n", i, r->minX, r->maxX, r->minZ, r->maxZ);
        }
    }
    fflush(stdout);
}

/* T2_GIVE_WEAPONS=1 (game.cpp, every frame; DEBUG-ONLY, round 7 v2): in gameplay (mode 13, a
 * level loaded) every active player gets every weapon of gWeaponInfoTable that can be used on
 * land or under water, with full ammo (CPlayer__FillAllAmmo), once per player and level. To
 * test firing and the weapons without finding their pickups; not a game option. */
void T2Port_GiveWeapons(void) {
    extern s32 CPlayer__FillAllAmmo(CPlayer *);
    static CPlayer *sGiven[4];
    static int sLevel = -1;
    static int sEnabled = -1;
    CPlayer *p;
    int i;

    if (sEnabled < 0) {
        const char *e = getenv("T2_GIVE_WEAPONS");
        sEnabled = (e != NULL && e[0] == '1');
    }
    if (!sEnabled || gEngineApp.m_Settings.mode != 13 || gScene.m_nLevel <= 0) {
        return;
    }
    if (gScene.m_nLevel != sLevel) {
        sLevel = gScene.m_nLevel;
        for (i = 0; i < 4; i++) {
            sGiven[i] = NULL;
        }
    }
    for (p = gEngineApp_m_Game.m_PlayerList.m_ActiveList.m_pHead; p != NULL; p = p->m_pNext) {
        int slot = -1;
        for (i = 0; i < 4; i++) {
            if (sGiven[i] == p) {
                slot = -2;
                break;
            }
            if (sGiven[i] == NULL && slot == -1) {
                slot = i;
            }
        }
        if (slot < 0) {
            continue;
        }
        sGiven[slot] = p;
        for (i = 0; i < 35; i++) {
            if (gWeaponInfoTable[i] != NULL && (gWeaponInfoTable[i]->flags & 3) != 0) {
                p->m_Data.m_WeaponList[i] = 1;
            }
        }
        CPlayer__FillAllAmmo(p);
        printf("debug: T2_GIVE_WEAPONS: level %d player %p: all weapons, full ammo\n", (int) gScene.m_nLevel,
               (void *) p);
    }
}
