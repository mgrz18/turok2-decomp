#include "common.h"
#include "structs/menu.h"

/* Checks the controller pak in the given slot before a save/load menu:
 * returns the pak status, or -1 after opening the "pak full" dialog; opens the
 * matching warning dialog (templates D_0043xxxx) when the pak is missing,
 * damaged or unformatted. */

extern u8 gEngineApp_m_Game_m_MenuPool[];     /* gEngineApp_m_Game + 0x5BC: the menu system */
extern u8 gPakMsgPakNotFoundDesc[];
extern u8 gPakMsgInsertToManageDesc[];
extern u8 gPakMsgInsertToSaveDesc[];
extern u8 gPakMsgInsertToLoadDesc[];
extern u8 gPakMsgPakChangedDesc[];
extern u8 gDamagedPakAtBootMenuDesc[];
extern s32 gPakMenuIsSave;
extern s32 gPakMenuAnyPak;
extern s32 gPakMenuLeaving;

extern void Pak_ProbeSlot(s32 slot);
extern s32 Pak_GetStatus(s32 slot);
extern void *PakMenu_GetReturnMenu(Menu *c);
extern s32 Controller_TakePakChanged(s32 slot);
extern s32 Pak_IsRumblePak(s32 slot);
extern s32 Pak_NeedsRepair(s32 slot);
extern void MenuPool_OpenMenu(void *menus, void *tmpl, s32 a, s32 b, void *parent);

s32 PakMenu_CheckPak(Menu *c, s32 slot, s32 quiet) {
    s32 status;
    void *parent;
    s32 full;
    void *tmpl;

    Pak_ProbeSlot(slot);
    status = Pak_GetStatus(slot);
    parent = PakMenu_GetReturnMenu(c);
    if (parent != NULL) {
        full = Controller_TakePakChanged(slot);
        if ((status == 0 || full != 0) && status != -2) {
            if (full != 0) {
                MenuPool_OpenMenu(gEngineApp_m_Game_m_MenuPool, gPakMsgPakChangedDesc, c->player, c->controller, parent);
                return -1;
            }
        } else if (Pak_IsRumblePak(slot) != 0) {
            gPakMenuLeaving = 1;
            if (gPakMenuAnyPak != 0) {
                tmpl = gPakMsgInsertToManageDesc;
            } else if (gPakMenuIsSave != 0) {
                tmpl = gPakMsgInsertToSaveDesc;
            } else {
                tmpl = gPakMsgInsertToLoadDesc;
            }
            MenuPool_OpenMenu(gEngineApp_m_Game_m_MenuPool, tmpl, c->player, c->controller, parent);
        } else if (quiet == 0) {
            gPakMenuLeaving = 1;
            if (Pak_NeedsRepair(slot) != 0) {
                MenuPool_OpenMenu(gEngineApp_m_Game_m_MenuPool, gDamagedPakAtBootMenuDesc, c->player, c->controller, parent);
            } else {
                MenuPool_OpenMenu(gEngineApp_m_Game_m_MenuPool, gPakMsgPakNotFoundDesc, c->player, c->controller, parent);
            }
        } else if (status != -2) {
            status = 0;
        }
        return status;
    }
    return 0;
}
