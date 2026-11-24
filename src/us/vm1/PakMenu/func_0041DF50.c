/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00406434.c, func_00406434.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/camera.h"
#include "structs/player.h"
#include "structs/menu.h"
#include "structs/pak.h"
#include "globals/menu.h"
#include "globals/pak.h"
#include "globals/menu_desc.h"
#include "protos/menu.h"
#include "structs/camera.h"
#include "protos/pak.h"
#include "protos/pakmenu.h"

/* O01Pick and O01Capture are separate functions in Rage Wars; inlined here in Turok 2. */
static inline u8 *O01Pick(PakChannel *sel) {
    u8 *msg;
    switch (sel->channel) {
    case 0:
    default:
        msg = (u8 *)&gStatsAfterPakMenuDesc1;
        break;
    case 1:
        msg = (u8 *)&gStatsAfterPakMenuDesc2;
        break;
    case 2:
        msg = (u8 *)&gStatsAfterPakMenuDesc3;
        break;
    case 3:
        msg = (u8 *)&gStatsAfterPakMenuDesc4;
        break;
    }
    return msg;
}
static inline void O01Capture(CPlayer *a) {
    CPlayerOptions *o = a->m_pOptions;
    s32 i;
    gPakProfile.character = o->m_nCharacter;
    gPakProfile.suicides = o->m_Suicides;
    gPakProfile.deaths = o->m_Deaths;
    gPakProfile.kills = o->m_Kills;
    gPakProfile.tags = o->m_Tags;
    gPakProfile.reserved[0] = 0;
    gPakProfile.reserved[1] = 0;
    gPakProfile.reserved[2] = 0;
    for (i = 0; i < 8; i++) {
        gPakProfile.name[i] = o->m_Name[i];
    }
}
void PakMenu_InitSaveProfile(Menu *own) {
    CPlayer *plr;
    PakChannel sel;
    u8 *msg;
    s32 pad;
    CPlayer *pp;
    s32 pv;
    gPakMenuIsSave = 1;
    gPakMenuWrites = 1;
    gPakMenuIsLoad = 0;
    gPakMenuIsProfile = 1;
    gPakMenuMode = 0;
    gPakMenuLeaving = 0;
    gPakMenuAnyPak = 0;
    pv = own->controller->port;
    pp = own->player;
    sel.channel = pv;
    plr = pp;
    msg = O01Pick(&sel);
    if (gDamagedPakContinued != 0) {
        gPakMenuLeaving = 1;
        MenuPool_OpenMenu((u8 *)own->player->m_pCamera->m_MenuPool, msg, plr, plr->m_ContState.m_pContState, 0);
    } else {
        if (gPakMenuAnyPak != 0) {
            pad = gPakMenuPort;
        } else {
            pad = own->controller->port;
        }
        Pak_ProbeSlot(pad);
        if (Pak_GetStatus(sel.channel) == 0) {
            O01Capture(plr);
            gPakProfile.checksum = Crc32_Compute((u8 *)&gPakProfile, 0x14, 4);
            PakMenu_UpdateNoteRows(own);
        } else {
            gPakMenuLeaving = 1;
            if (Pak_IsRumblePak(sel.channel) != 0) {
                MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, (u8 *)&gPakMsgInsertToSaveDesc, plr, plr->m_ContState.m_pContState, msg);
            } else if (Pak_NeedsRepair(sel.channel) != 0) {
                MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, (u8 *)&gDamagedPakAtBootMenuDesc, plr, plr->m_ContState.m_pContState, (u8 *)&gPakSaveProfileMenuDesc);
            } else {
                MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, (u8 *)&gPakMsgPakNotFoundDesc, plr, plr->m_ContState.m_pContState, msg);
            }
        }
    }
}
