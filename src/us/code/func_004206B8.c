/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00408C78.c, func_00408C78.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/game.h"
#include "structs/player.h"
#include "structs/menu.h"
#include "structs/pak.h"
#include "globals/menu_desc.h"

extern s8 gPakProfileRead_character[];
extern s32 gPakMenuAnyPak;
extern s32 gPakMenuPort;
extern s32 gPakMenuLeaving;
extern s32 sCharNoteLoad;
extern s32 gPakNoteIndex;
extern u8 gEngineApp_m_Game_m_MenuPool[];
s32 PakMenu_CheckPak(Menu *o, s32 pad, s32 z);
CPlayer *CPlayerPool__FindByController(CPlayerPool *world, ControllerProfile *s);
void CPlayer__SetCharacter(CPlayer *a, s32 b);
void MenuPool_OpenChildMenu(u8 *a, Menu *o, u8 *c);
s32 Pak_DeleteNote(s32 pad, s32 idx);
void MenuPool_OpenMenu(u8 *a, void *b, void *c, void *d, void *e);
extern PakProfile gPakProfile;
/* Turok 2 inlines this (Rage Wars calls it) */
static inline void O01Restore(CPlayer *a) {
    CPlayerOptions *s = a->m_pOptions;
    s32 i;
    s->m_nCharacter = gPakProfile.character;
    s->m_Suicides = gPakProfile.suicides;
    s->m_Deaths = gPakProfile.deaths;
    s->m_Kills = gPakProfile.kills;
    s->m_Tags = gPakProfile.tags;
    for (i = 0; i < 8; i++) {
        s->m_Name[i] = gPakProfile.name[i];
    }
}
s32 CharNoteMenu_SelectYes(s32 unused, Menu *own) {
    s32 pad;
    CPlayer *plr;
    if (gPakMenuAnyPak != 0) {
        pad = gPakMenuPort;
    } else {
        pad = own->controller->port;
    }
    if (PakMenu_CheckPak(own, pad, 0) != 0) {
        gPakMenuLeaving = 1;
        return 1;
    }
    if (sCharNoteLoad != 0) {
        plr = CPlayerPool__FindByController(&gEngineApp_m_Game.m_PlayerList, own->controller);
        own->player = plr;
        O01Restore(plr);
        CPlayer__SetCharacter(plr, gPakProfileRead_character[0]);
        MenuPool_OpenChildMenu((u8 *)plr->m_pCamera + 0x53C, own, (u8 *)&gPlayerSetupMenuDesc);
        return 1;
    }
    if (Pak_DeleteNote(pad, gPakNoteIndex) == 0) {
        MenuPool_OpenMenu(gEngineApp_m_Game_m_MenuPool, (u8 *)&gPakMsgDeleteDoneDesc, own->player, own->controller, (void *)own->context);
    } else {
        MenuPool_OpenMenu(gEngineApp_m_Game_m_MenuPool, (u8 *)&gPakMsgDeleteFailedDesc, own->player, own->controller, (void *)own->context);
    }
    return 1;
}
