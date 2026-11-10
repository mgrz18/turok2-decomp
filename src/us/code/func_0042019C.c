/* First-module twin of src/us/code/PakMenu_SelectCancel_2.c. WRONG CUT: REAL PakMenu_SelectCancel size 0x2B8 (0x0042019C-0x00420454); splat cut its tail into func_004203B0. Matches whole (checked with the size overridden). */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_004085E0.c, func_004085E0, with Turok 2's layouts. */
#include "common.h"
#include "structs/camera.h"
#include "structs/player.h"
#include "structs/menu.h"
#include "structs/game.h"
#include "structs/save.h"
#include "globals/game.h"
#include "globals/menu.h"
#include "globals/pak.h"
#include "globals/menu_desc.h"
#include "protos/menu.h"
#include "structs/camera.h"

/* Controller pak menu: stops the pak sounds and shows the message for the pak's state. */

extern s32 D_8011F144;
extern s32 D_8011B114;
void CCache__Flush(s32 a);
void CCache__Deallocate(s32 a, sptr b);
CPlayer *CPlayerPool__FindByController(void *a, void *b);

s32 PakMenu_SelectCancel(s32 a0, Menu *arg) {
    struct {
        s32 m;
        s32 pad;
    } mm;
    u8 *tbl;
    CPlayer *plr;
    s32 sw;
    CPlayer *u;

    if (gSaveBufferHandle != 0 || gLoadBufferHandle != 0) {
        CCache__Flush(0);
    }
    if (gSaveBufferHandle != 0) {
        CCache__Deallocate(0, gSaveBufferHandle);
    }
    if (gLoadBufferHandle != 0) {
        CCache__Deallocate(0, gLoadBufferHandle);
    }
    gSaveBufferHandle = 0;
    gLoadBufferHandle = 0;
    gSaveBuffer = 0;
    gLoadBuffer = 0;
    gCameraPoolHideViews = 0;
    if (gPakMenuAnyPak != 0) {
        mm.m = gPakMenuPort;
    } else {
        mm.m = arg->controller->port;
    }
    if (gPakMenuAnyPak != 0) {
        gPakCheckDone = 0;
        gPakMenuStartPressed = 0;
        D_8011F144 = 1;
        D_8011B114 = 0;
        MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, (u8 *)&gPakSaveGameAnyPakMenuDesc, 0, (sptr)arg->controller, 0);
        return 1;
    }
    if (gPakMenuIsProfile != 0) {
        if (gPakMenuIsLoad != 0) {
            gPakMenuLeaving = 1;
            plr = CPlayerPool__FindByController(&gEngineApp_m_Game.m_PlayerList, arg->controller);
            MenuPool_OpenMenu((u8 *)plr->m_pCamera->m_MenuPool, (u8 *)&gCharacterMenuDesc, (sptr)plr, plr->m_ContState.m_pContState, mm.m);
            return 1;
        }
        if (gPakMenuIsSave != 0) {
            gPakMenuLeaving = 1;
            u = (CPlayer *)arg->player;
            sw = arg->controller->port;
            mm.m = sw;
            plr = u;
            switch (sw) {
            case 0:
            default:
                tbl = (u8 *)&gStatsAfterPakMenuDesc1;
                break;
            case 1:
                tbl = (u8 *)&gStatsAfterPakMenuDesc2;
                break;
            case 2:
                tbl = (u8 *)&gStatsAfterPakMenuDesc3;
                break;
            case 3:
                tbl = (u8 *)&gStatsAfterPakMenuDesc4;
                break;
            }
            MenuPool_OpenMenu((u8 *)plr->m_pCamera->m_MenuPool, tbl, (sptr)plr, plr->m_ContState.m_pContState, mm.m);
            return 1;
        }
        return 0;
    }
    plr = CPlayerPool__FindByController(&gEngineApp_m_Game.m_PlayerList, arg->controller);
    if (gPakMenuIsLoad != 0) {
        gPakMenuLeaving = 1;
        MenuPool_OpenMenu(gEngineApp_m_Game.m_MenuPool, (u8 *)&gSingleMenuAfterPakDesc, (sptr)plr, plr->m_ContState.m_pContState, mm.m);
        return 1;
    }
    if (gPakMenuIsSave == 0) {
        return 0;
    } else {
        gPakMenuLeaving = 1;
        MenuPool_OpenMenu((u8 *)plr->m_pCamera->m_MenuPool, (u8 *)&gSavePortalMenuDescB, (sptr)plr, plr->m_ContState.m_pContState, mm.m);
        gCameraPoolHideViews = 0;
        return 1;
    }
}
