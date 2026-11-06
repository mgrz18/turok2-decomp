/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80409144_de_13.c, func_80409144_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/menu_desc.h"

extern s32 gPakMenuLeaving;
extern int PakConfirmMenu_SelectYes(void * owner, struct MenuRecAt24 * menu, void * arg);
extern int gPakNoteIndex;
extern s32 gPakMenuPort;
extern s32 D_8011B114;
extern s32 gPakMenuAnyPak;
extern char gEngineApp_m_Game_m_MenuPool[];
extern s32 Pak_DeleteNote(s32 ch, s32 index);
extern void Pak_ProbeSlot(s32 ch);
extern s32 gPakConfirmKind;
extern s32 gPakOverwriteNote;
extern s32 Pak_RepairIfDamaged(s32 ch);
extern s32 PakMenu_WriteSaveGame(void *owner, MenuRecAt24 *menu, void *arg);
s32 PakConfirmMenu_SelectYes(void *owner, MenuRecAt24 *menu, void *arg) {
    extern void MenuPool_OpenMenu(char *, char *, void *, PakMenuSlot *, char *);
    extern s32 PakMenu_CheckPak(MenuRecAt24 *menu, s32 ch, s32 quiet);
    s32 ch;
    switch (gPakConfirmKind) {
        case 4:
            if (gPakMenuAnyPak != 0) {
                ch = gPakMenuPort;
            } else {
                ch = menu->slot->unk4;
            }
            if (PakMenu_CheckPak(menu, ch, 1) != 0) {
                gPakMenuLeaving = 1;
                MenuPool_OpenMenu(gEngineApp_m_Game_m_MenuPool, menu->prompt, menu->player, menu->slot, 0);
                break;
            }
            Pak_ProbeSlot(ch);
            if (Pak_DeleteNote(ch, gPakOverwriteNote) == 0) {
                MenuPool_OpenMenu(gEngineApp_m_Game_m_MenuPool, (char *)&gPakMsgDeleteDoneDesc, menu->player, menu->slot, menu->prompt);
            } else {
                MenuPool_OpenMenu(gEngineApp_m_Game_m_MenuPool, (char *)&gPakMsgDeleteFailedDesc, menu->player, menu->slot, menu->prompt);
            }
            break;
        case 3:
            if (gPakMenuAnyPak != 0) {
                ch = gPakMenuPort;
            } else {
                ch = menu->slot->unk4;
            }
            if (PakMenu_CheckPak(menu, ch, 1) != 0) {
                gPakMenuLeaving = 1;
                MenuPool_OpenMenu(gEngineApp_m_Game_m_MenuPool, menu->prompt, menu->player, menu->slot, 0);
                break;
            }
            Pak_ProbeSlot(ch);
            if (Pak_DeleteNote(ch, gPakNoteIndex) == 0) {
                PakMenu_WriteSaveGame(owner, menu, arg);
                return 1;
            }
            MenuPool_OpenMenu(gEngineApp_m_Game_m_MenuPool, (char *)&gPakMsgDeleteFailedDesc, menu->player, menu->slot, menu->prompt);
            break;
        case 0:
            if (gPakMenuAnyPak != 0) {
                ch = gPakMenuPort;
            } else {
                ch = menu->slot->unk4;
            }
            if (PakMenu_CheckPak(menu, ch, 1) != 0) {
                gPakMenuLeaving = 1;
                MenuPool_OpenMenu(gEngineApp_m_Game_m_MenuPool, menu->prompt, menu->player, menu->slot, 0);
                break;
            }
            Pak_ProbeSlot(ch);
            if (Pak_RepairIfDamaged(ch) == 0) {
                MenuPool_OpenMenu(gEngineApp_m_Game_m_MenuPool, (char *)&gPakMsgRepairedDesc, menu->player, menu->slot, menu->prompt);
            } else {
                MenuPool_OpenMenu(gEngineApp_m_Game_m_MenuPool, (char *)&gDamagedPakInGameMenuDesc, menu->player, menu->slot, menu->prompt);
            }
            break;
        case 1:
        case 2:
            if (gPakMenuAnyPak != 0) {
                D_8011B114 = 1;
            }
            MenuPool_OpenMenu(gEngineApp_m_Game_m_MenuPool, menu->prompt, menu->player, menu->slot, 0);
            break;
    }
    return 1;
}
