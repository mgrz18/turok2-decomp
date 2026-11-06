/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00406434.c, func_00406434.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"
#include "structs/pak.h"
#include "globals/menu.h"

typedef struct O01Sel { s32 v; s32 w; } O01Sel;
extern s32 gPakMenuIsSave;
extern s32 gPakMenuWrites;
extern s32 gPakMenuIsLoad;
extern s32 gPakMenuIsProfile;
extern s32 gPakMenuMode;
extern s32 gPakMenuLeaving;
extern s32 gPakMenuAnyPak;
extern s32 gDamagedPakContinued;
extern s32 gPakMenuPort;
extern PakProfile gPakProfile;
extern u8 gPakMsgInsertToSaveDesc_2[];
extern u8 gDamagedPakAtBootMenuDesc_2[];
extern u8 gPakMsgPakNotFoundDesc_2[];
extern u8 gPakSaveProfileMenuDesc_2[];
void Pak_ProbeSlot_2(s32 ch);
s32 Pak_GetStatus_2(s32 ch);
s32 Pak_IsRumblePak_2(s32 ch);
s32 Pak_NeedsRepair_2(s32 ch);
void PakMenu_UpdateNoteRows_2(Menu *o);
u32 Crc32_Compute_2(u8 *data, s32 len, u32 crc);
void MenuPool_OpenMenu_2(u8 *a, void *b, void *c, s32 d, void *e);
extern u8 gPakProfile_name[8];
extern u8 gStatsAfterPakMenuDesc1_2[];
extern u8 gStatsAfterPakMenuDesc2_2[];
extern u8 gStatsAfterPakMenuDesc3_2[];
extern u8 gStatsAfterPakMenuDesc4_2[];
/* Static inlines in the Rage Wars source file (dropped by the importer). */
static inline u8 *O01Pick(O01Sel *sel) {
    u8 *p;
    switch (sel->v) {
    default:
    case 0:
        p = gStatsAfterPakMenuDesc1_2;
        break;
    case 1:
        p = gStatsAfterPakMenuDesc2_2;
        break;
    case 2:
        p = gStatsAfterPakMenuDesc3_2;
        break;
    case 3:
        p = gStatsAfterPakMenuDesc4_2;
        break;
    }
    return p;
}
static inline void O01Capture(CPlayer *a) {
    CPlayerOptions *p = a->m_pOptions;
    s32 i;
    gPakProfile.character = p->m_nCharacter;
    gPakProfile.suicides = p->m_Suicides;
    gPakProfile.deaths = p->m_Deaths;
    gPakProfile.kills = p->m_Kills;
    gPakProfile.tags = p->m_Tags;
    gPakProfile.reserved[0] = 0;
    gPakProfile.reserved[1] = 0;
    gPakProfile.reserved[2] = 0;
    for (i = 0; i < 8; i++) {
        gPakProfile_name[i] = p->m_Name[i];
    }
}
void PakMenu_InitSaveProfile_2(Menu *own) {
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
        MenuPool_OpenMenu_2((u8 *)own->player->m_pCamera + 0x53C, msg, plr, plr->m_ContState.m_pContState, 0);
    } else {
        if (gPakMenuAnyPak != 0) {
            pad = gPakMenuPort;
        } else {
            pad = own->controller->port;
        }
        Pak_ProbeSlot_2(pad);
        if (Pak_GetStatus_2(sel.v) == 0) {
            O01Capture(plr);
            gPakProfile.checksum = Crc32_Compute_2((u8 *)&gPakProfile, 0x14, 4);
            PakMenu_UpdateNoteRows_2(own);
        } else {
            gPakMenuLeaving = 1;
            if (Pak_IsRumblePak_2(sel.v) != 0) {
                MenuPool_OpenMenu_2(&gEngineApp_m_Game_m_MenuPool, gPakMsgInsertToSaveDesc_2, plr, plr->m_ContState.m_pContState, msg);
            } else if (Pak_NeedsRepair_2(sel.v) != 0) {
                MenuPool_OpenMenu_2(&gEngineApp_m_Game_m_MenuPool, gDamagedPakAtBootMenuDesc_2, plr, plr->m_ContState.m_pContState, gPakSaveProfileMenuDesc_2);
            } else {
                MenuPool_OpenMenu_2(&gEngineApp_m_Game_m_MenuPool, gPakMsgPakNotFoundDesc_2, plr, plr->m_ContState.m_pContState, msg);
            }
        }
    }
}
