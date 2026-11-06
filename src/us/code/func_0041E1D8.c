/* WRONG CUT: real function 0x0041E1D8, size 0x19C (through 0x0041E374): splat cut it in two
 * (PakMenu_InitLoadProfile, func_0041E294) at a jal's delay slot. */
#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"
#include "globals/menu.h"
#include "globals/pak.h"

extern s32 gDamagedPakContinued;
extern s32 gPakMenuMode;
extern u8 gCharacterMenuAfterPakDesc[];
extern u8 gPakLoadProfileMenuDesc[];
extern u8 gPakMsgPakNotFoundDesc[];
extern u8 gPakMsgInsertToLoadDesc[];
extern u8 gDamagedPakAtBootMenuDesc[];

extern void MenuPool_OpenMenu(void *pool, void *tmpl, CPlayer *player, ControllerProfile *ctl, void *back);
extern void Pak_ProbeSlot(s32 kind);
extern s32 Pak_GetStatus(s32 kind);
extern s32 Pak_IsRumblePak(s32 kind);
extern s32 Pak_NeedsRepair(s32 kind);
extern void PakMenu_UpdateNoteRows(Menu *m);

void PakMenu_InitLoadProfile(Menu *m) {
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
        MenuPool_OpenMenu(((u8 *)m->player->m_pCamera) + 0x53C, gCharacterMenuAfterPakDesc, m->player, m->controller, NULL);
        return;
    }
    Pak_ProbeSlot(kind);
    if (!Pak_GetStatus(kind)) {
        PakMenu_UpdateNoteRows(m);
        return;
    }
    gPakMenuLeaving = 1;
    if (Pak_IsRumblePak(kind)) {
        MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, gPakMsgInsertToLoadDesc, m->player, m->controller, gCharacterMenuAfterPakDesc);
    } else if (Pak_NeedsRepair(kind)) {
        MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, gDamagedPakAtBootMenuDesc, m->player, m->controller, gPakLoadProfileMenuDesc);
    } else {
        MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, gPakMsgPakNotFoundDesc, m->player, m->controller, gCharacterMenuAfterPakDesc);
    }
}
