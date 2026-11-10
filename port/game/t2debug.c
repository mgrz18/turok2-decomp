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
    fflush(stdout);
}
