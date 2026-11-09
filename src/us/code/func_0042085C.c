/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_00408E1C.c, func_00408E1C, with Turok 2's layouts. */
/* WRONG CUT: the real function is 0x0042085C-0x00420C30 (size 0x3D4, 245 words): splat split it into PakMessage_SelectOk (0x1C),
 * func_00420878 (0x68) and func_004208E0 (0x350). All 245 words match (checked over the real extent).
 * Twin of src/us/code/PakMessage_SelectOk_2.c (first-module callees and data names, +0xE0). */
#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"
#include "globals/menu.h"
#include "globals/pak.h"
#include "globals/scene.h"
#include "structs/game.h"
#include "globals/menu_desc.h"
#include "protos/scene.h"
#include "protos/menu.h"
#include "protos/game.h"

/* Controller pak menu: closes the current message box and shows the next one for the menu state. */

void Controller_ResetRumble(ControllerProfile *a);
void Controller_UnwatchPak(s32 a);
s32 Controller_TakePakChanged(s32 a);
void Pak_ProbeSlot(s32 a);
s32 Pak_GetStatus(s32 a);
void func_004303AC(u8 *a);

s32 PakMessage_SelectOk(s32 a0, Menu *o) {
    s32 s0, s1, v;
    u8 *p;

    if (gPakMessageKind == 8) {
        return 0;
    }
    if (o->context == &gSavePortalMenuDescB || o->context == &gSavePortalMenuDesc) {
        gCameraPoolHideViews = 0;
    }
    if (gPakMessageKind == 9 || gPakMessageKind == 5) {
        Controller_ResetRumble(o->controller);
        Controller_SetRumbleOption(o->controller, 1);
    }
    if (gPakMessageKind == 9) {
        if (gPakMenuIsLoad != 0 && gPakMenuIsProfile == 0 && sPakMessageRumbleB == 0) {
            if (gPakMenuAnyPak != 0) {
                s1 = gPakMenuPort;
            } else {
                s1 = o->controller->port;
            }
            Controller_UnwatchPak(s1);
            CScene__RequestWarp(&gScene, o->player->m_Data.m_CheckPointWarpID, 2);
            return 1;
        }
        func_004303AC(&gEngineApp);
        return 1;
    }
    if (gPakMenuAnyPak != 0) {
        s1 = gPakMenuPort;
    } else {
        s1 = o->controller->port;
    }
    if (gPakMessageKind == 5) {
        Controller_UnwatchPak(s1);
        CScene__RequestWarp(&gScene, gScene_m_PendingWarpID, 2);
    }
    if (gPakMenuAnyPak != 0) {
        if (gPakMessageKind == 6) {
            gPakCheckDone = 1;
        }
    }
    if (gPakMessageKind == 4 && gPakMenuIsLoad != 0 && gPakMenuIsProfile == 0) {
        Pak_ProbeSlot(s1);
        s0 = Pak_GetStatus(s1);
        v = Controller_TakePakChanged(s1);
        if ((s0 == 0 || v != 0) && s0 != -2 && o->context == (void *)1) {
            MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, &gPakLoadGameMenuDesc, o->player, o->controller, 0);
        } else {
            MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, &gSingleMenuAfterPakDesc, o->player, o->controller, 0);
        }
    } else if (gPakMessageKind == 3 && gSavePortalActive != 0) {
        Pak_ProbeSlot(s1);
        s0 = Pak_GetStatus(s1);
        v = Controller_TakePakChanged(s1);
        if ((s0 == 0 || v != 0) && s0 != -2 && o->context == (void *)1) {
            MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, &gPakSaveGameMenuDesc, o->player, o->controller, 0);
        } else {
            if (o->player != NULL) {
                p = ((u8 *)o->player->m_pCamera) + 0x53C;
            } else {
                p = &gEngineApp_m_Game_m_MenuPool;
            }
            MenuPool_OpenMenu(p, &gSavePortalMenuDescB, o->player, o->controller, 0);
            gCameraPoolHideViews = 0;
        }
    } else {
        if (o->context == (void *)1) {
            o->context = NULL;
        }
        if (o->context != NULL) {
            if (o->context != &gPakSaveProfileMenuDesc && o->player != NULL) {
                p = ((u8 *)o->player->m_pCamera) + 0x53C;
            } else {
                p = &gEngineApp_m_Game_m_MenuPool;
            }
            MenuPool_OpenMenu(p, o->context, o->player, o->controller, s1);
        }
    }
    return 1;
}
