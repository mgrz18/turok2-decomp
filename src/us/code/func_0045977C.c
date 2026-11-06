/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_00408E1C.c, func_00408E1C, with Turok 2's layouts. */
#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"
#include "globals/menu.h"
#include "globals/pak.h"

/* Controller pak menu: closes the current message box and shows the next one for the menu state. */

extern s32 gPakMenuPort; /* pad */
extern s32 gScene_m_PendingWarpID;
extern u8 gScene[];
extern u8 gEngineApp[];
extern u8 gSavePortalMenuDescB_2[];
extern u8 gSavePortalMenuDesc_2[];
extern u8 gPakLoadGameMenuDesc_2[];
extern u8 gSingleMenuAfterPakDesc_2[];
extern u8 gPakSaveGameMenuDesc_2[];
extern u8 gPakSaveProfileMenuDesc_2[];
void Controller_ResetRumble(ControllerProfile *a);
void Controller_SetRumbleOption(ControllerProfile *a, s32 b);
void Controller_UnwatchPak(s32 a);
s32 Controller_TakePakChanged(s32 a);
void Pak_ProbeSlot_2(s32 a);
s32 Pak_GetStatus_2(s32 a);
void CScene__RequestWarp_2(u8 *a, s32 b, s32 c);
void func_004692CC(u8 *a);
void MenuPool_OpenMenu_2(u8 *a, void *b, CPlayer *c, ControllerProfile *d, s32 e);

s32 PakMessage_SelectOk_2(s32 a0, Menu *o) {
    s32 s0, s1, v;
    u8 *p;

    if (gPakMessageKind == 8) {
        return 0;
    }
    if (o->context == gSavePortalMenuDescB_2 || o->context == gSavePortalMenuDesc_2) {
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
            CScene__RequestWarp_2(gScene, o->player->m_Data.m_CheckPointWarpID, 2);
            return 1;
        }
        func_004692CC(gEngineApp);
        return 1;
    }
    if (gPakMenuAnyPak != 0) {
        s1 = gPakMenuPort;
    } else {
        s1 = o->controller->port;
    }
    if (gPakMessageKind == 5) {
        Controller_UnwatchPak(s1);
        CScene__RequestWarp_2(gScene, gScene_m_PendingWarpID, 2);
    }
    if (gPakMenuAnyPak != 0) {
        if (gPakMessageKind == 6) {
            gPakCheckDone = 1;
        }
    }
    if (gPakMessageKind == 4 && gPakMenuIsLoad != 0 && gPakMenuIsProfile == 0) {
        Pak_ProbeSlot_2(s1);
        s0 = Pak_GetStatus_2(s1);
        v = Controller_TakePakChanged(s1);
        if ((s0 == 0 || v != 0) && s0 != -2 && o->context == (void *)1) {
            MenuPool_OpenMenu_2(&gEngineApp_m_Game_m_MenuPool, gPakLoadGameMenuDesc_2, o->player, o->controller, 0);
        } else {
            MenuPool_OpenMenu_2(&gEngineApp_m_Game_m_MenuPool, gSingleMenuAfterPakDesc_2, o->player, o->controller, 0);
        }
    } else if (gPakMessageKind == 3 && gSavePortalActive != 0) {
        Pak_ProbeSlot_2(s1);
        s0 = Pak_GetStatus_2(s1);
        v = Controller_TakePakChanged(s1);
        if ((s0 == 0 || v != 0) && s0 != -2 && o->context == (void *)1) {
            MenuPool_OpenMenu_2(&gEngineApp_m_Game_m_MenuPool, gPakSaveGameMenuDesc_2, o->player, o->controller, 0);
        } else {
            if (o->player != NULL) {
                p = ((u8 *)o->player->m_pCamera) + 0x53C;
            } else {
                p = &gEngineApp_m_Game_m_MenuPool;
            }
            MenuPool_OpenMenu_2(p, gSavePortalMenuDescB_2, o->player, o->controller, 0);
            gCameraPoolHideViews = 0;
        }
    } else {
        if (o->context == (void *)1) {
            o->context = NULL;
        }
        if (o->context != NULL) {
            if (o->context != gPakSaveProfileMenuDesc_2 && o->player != NULL) {
                p = ((u8 *)o->player->m_pCamera) + 0x53C;
            } else {
                p = &gEngineApp_m_Game_m_MenuPool;
            }
            MenuPool_OpenMenu_2(p, o->context, o->player, o->controller, s1);
        }
    }
    return 1;
}
