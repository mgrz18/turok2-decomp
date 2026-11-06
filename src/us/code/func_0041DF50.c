/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00406434.c, func_00406434.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"
#include "structs/pak.h"
#include "globals/menu.h"
#include "globals/pak.h"

typedef struct O01Sel { s32 v; s32 w; } O01Sel;
extern s32 gPakMenuMode;
extern s32 gDamagedPakContinued;
extern s32 gPakMenuPort;
extern u8 gPakMsgInsertToSaveDesc[];
extern u8 gDamagedPakAtBootMenuDesc[];
extern u8 gPakMsgPakNotFoundDesc[];
extern u8 gPakSaveProfileMenuDesc[];
extern u8 gStatsAfterPakMenuDesc1[];
extern u8 gStatsAfterPakMenuDesc2[];
extern u8 gStatsAfterPakMenuDesc3[];
extern u8 gStatsAfterPakMenuDesc4[];
void Pak_ProbeSlot(s32 ch);
s32 Pak_GetStatus(s32 ch);
s32 Pak_IsRumblePak(s32 ch);
s32 Pak_NeedsRepair(s32 ch);
void PakMenu_UpdateNoteRows(Menu *o);
u32 Crc32_Compute(u8 *data, s32 len, u32 crc);
void MenuPool_OpenMenu(u8 *a, void *b, void *c, s32 d, void *e);
/* O01Pick and O01Capture are separate functions in Rage Wars; inlined here in Turok 2. */
static inline u8 *O01Pick(O01Sel *sel) {
    u8 *msg;
    switch (sel->v) {
    case 0:
    default:
        msg = gStatsAfterPakMenuDesc1;
        break;
    case 1:
        msg = gStatsAfterPakMenuDesc2;
        break;
    case 2:
        msg = gStatsAfterPakMenuDesc3;
        break;
    case 3:
        msg = gStatsAfterPakMenuDesc4;
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
    O01Sel sel;
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
    sel.v = pv;
    plr = pp;
    msg = O01Pick(&sel);
    if (gDamagedPakContinued != 0) {
        gPakMenuLeaving = 1;
        MenuPool_OpenMenu((u8 *)own->player->m_pCamera + 0x53C, msg, plr, plr->m_ContState.m_pContState, 0);
    } else {
        if (gPakMenuAnyPak != 0) {
            pad = gPakMenuPort;
        } else {
            pad = own->controller->port;
        }
        Pak_ProbeSlot(pad);
        if (Pak_GetStatus(sel.v) == 0) {
            O01Capture(plr);
            gPakProfile.checksum = Crc32_Compute((u8 *)&gPakProfile, 0x14, 4);
            PakMenu_UpdateNoteRows(own);
        } else {
            gPakMenuLeaving = 1;
            if (Pak_IsRumblePak(sel.v) != 0) {
                MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, gPakMsgInsertToSaveDesc, plr, plr->m_ContState.m_pContState, msg);
            } else if (Pak_NeedsRepair(sel.v) != 0) {
                MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, gDamagedPakAtBootMenuDesc, plr, plr->m_ContState.m_pContState, gPakSaveProfileMenuDesc);
            } else {
                MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, gPakMsgPakNotFoundDesc, plr, plr->m_ContState.m_pContState, msg);
            }
        }
    }
}
