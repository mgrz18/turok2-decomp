/* WRONG CUT: real function 0x0041E1D8, size 0x19C (through 0x0041E374): splat cut it in two
 * (PakMenu_InitLoadProfile_2, func_0041E294) at a jal's delay slot. */
#include "common.h"
#include "protos/pak_2.h"
#include "structs/camera.h"
#include "structs/player.h"
#include "structs/menu.h"
#include "globals/menu.h"
#include "globals/pak.h"
#include "globals/menu_desc.h"
#include "protos/menu.h"
#include "structs/camera.h"
#include "protos/pak.h"
#include "protos/pakmenu.h"


void PakMenu_InitLoadProfile_2(Menu *m) {
    s32 kind;

    gPakMenuIsSave = 0;
    gPakMenuWrites = 0;
    gPakMenuIsLoad = 1;
    gPakMenuIsProfile = 1;
    gPakMenuMode = 1;
    gPakMenuLeaving = 0;
    gPakMenuAnyPak = 0;
    m->items[20].flags &= ~0x1000000;
    kind = m->controller->port;
    if (gDamagedPakContinued) {
        gPakMenuLeaving = 1;
        MenuPool_OpenMenu_2((u8 *)m->player->m_pCamera->m_MenuPool, &gCharacterMenuAfterPakDesc_2, m->player, m->controller, NULL);
        return;
    }
    Pak_ProbeSlot_2(kind);
    if (!Pak_GetStatus_2(kind)) {
        PakMenu_UpdateNoteRows_2(m);
        return;
    }
    gPakMenuLeaving = 1;
    if (Pak_IsRumblePak_2(kind)) {
        MenuPool_OpenMenu_2(&gEngineApp_m_Game_m_MenuPool, &gPakMsgInsertToLoadDesc_2, m->player, m->controller, &gCharacterMenuAfterPakDesc_2);
    } else if (Pak_NeedsRepair_2(kind)) {
        MenuPool_OpenMenu_2(&gEngineApp_m_Game_m_MenuPool, &gDamagedPakAtBootMenuDesc_2, m->player, m->controller, &gPakLoadProfileMenuDesc_2);
    } else {
        MenuPool_OpenMenu_2(&gEngineApp_m_Game_m_MenuPool, &gPakMsgPakNotFoundDesc_2, m->player, m->controller, &gCharacterMenuAfterPakDesc_2);
    }
}
