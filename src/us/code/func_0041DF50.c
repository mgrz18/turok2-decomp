/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00406434.c, func_00406434.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"
#include "structs/pak.h"

typedef struct O01Sel { s32 v; s32 w; } O01Sel;
extern s32 D_80130984;
extern s32 D_801309A0;
extern s32 D_80130994;
extern s32 D_801309B4;
extern s32 D_800C2024;
extern s32 D_801309BC;
extern s32 D_80130990;
extern s32 D_800C2020;
extern s32 D_800C2028;
extern PakProfile D_80130968;
extern u8 D_80119E2C[];
extern u8 D_0043BC48[];
extern u8 D_0043C05C[];
extern u8 D_0043BC00[];
extern u8 D_0043B6CC[];
extern u8 D_0043AC80[];
extern u8 D_0043ACA4[];
extern u8 D_0043ACC8[];
extern u8 D_0043ACEC[];
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
        msg = D_0043AC80;
        break;
    case 1:
        msg = D_0043ACA4;
        break;
    case 2:
        msg = D_0043ACC8;
        break;
    case 3:
        msg = D_0043ACEC;
        break;
    }
    return msg;
}
static inline void O01Capture(CPlayer *a) {
    CPlayerOptions *o = a->m_pOptions;
    s32 i;
    D_80130968.character = o->m_nCharacter;
    D_80130968.suicides = o->m_Suicides;
    D_80130968.deaths = o->m_Deaths;
    D_80130968.kills = o->m_Kills;
    D_80130968.tags = o->m_Tags;
    D_80130968.reserved[0] = 0;
    D_80130968.reserved[1] = 0;
    D_80130968.reserved[2] = 0;
    for (i = 0; i < 8; i++) {
        D_80130968.name[i] = o->m_Name[i];
    }
}
void PakMenu_InitSaveProfile(Menu *own) {
    CPlayer *plr;
    O01Sel sel;
    u8 *msg;
    s32 pad;
    CPlayer *pp;
    s32 pv;
    D_80130984 = 1;
    D_801309A0 = 1;
    D_80130994 = 0;
    D_801309B4 = 1;
    D_800C2024 = 0;
    D_801309BC = 0;
    D_80130990 = 0;
    pv = own->controller->port;
    pp = own->player;
    sel.v = pv;
    plr = pp;
    msg = O01Pick(&sel);
    if (D_800C2020 != 0) {
        D_801309BC = 1;
        MenuPool_OpenMenu((u8 *)own->player->m_pCamera + 0x53C, msg, plr, plr->m_ContState.m_pContState, 0);
    } else {
        if (D_80130990 != 0) {
            pad = D_800C2028;
        } else {
            pad = own->controller->port;
        }
        Pak_ProbeSlot(pad);
        if (Pak_GetStatus(sel.v) == 0) {
            O01Capture(plr);
            D_80130968.checksum = Crc32_Compute((u8 *)&D_80130968, 0x14, 4);
            PakMenu_UpdateNoteRows(own);
        } else {
            D_801309BC = 1;
            if (Pak_IsRumblePak(sel.v) != 0) {
                MenuPool_OpenMenu(D_80119E2C, D_0043BC48, plr, plr->m_ContState.m_pContState, msg);
            } else if (Pak_NeedsRepair(sel.v) != 0) {
                MenuPool_OpenMenu(D_80119E2C, D_0043C05C, plr, plr->m_ContState.m_pContState, D_0043B6CC);
            } else {
                MenuPool_OpenMenu(D_80119E2C, D_0043BC00, plr, plr->m_ContState.m_pContState, msg);
            }
        }
    }
}
