/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80409144_de_13.c, func_80409144_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/menu.h"
#include "globals/pak.h"
#include "globals/menu_desc.h"
#include "protos/pak.h"
#include "protos/pakmenu.h"

extern int PakConfirmMenu_SelectYes(void * owner, struct MenuRecAt24 * menu, void * arg);
s32 PakConfirmMenu_SelectYes(void *owner, MenuRecAt24 *menu, void *arg) {
    extern void MenuPool_OpenMenu(char *, char *, void *, PakMenuSlot *, char *);
    s32 ch;
    switch (gPakConfirmKind) {
        case 4:
            if (gPakMenuAnyPak != 0) {
                ch = gPakMenuPort;
            } else {
                ch = menu->slot->unk4;
            }
            if (PakMenu_CheckPak((struct Menu *) menu, ch, 1) != 0) {
                gPakMenuLeaving = 1;
                MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, menu->prompt, menu->player, menu->slot, 0);
                break;
            }
            Pak_ProbeSlot(ch);
            if (Pak_DeleteNote(ch, gPakOverwriteNote) == 0) {
                MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, (char *)&gPakMsgDeleteDoneDesc, menu->player, menu->slot, menu->prompt);
            } else {
                MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, (char *)&gPakMsgDeleteFailedDesc, menu->player, menu->slot, menu->prompt);
            }
            break;
        case 3:
            if (gPakMenuAnyPak != 0) {
                ch = gPakMenuPort;
            } else {
                ch = menu->slot->unk4;
            }
            if (PakMenu_CheckPak((struct Menu *) menu, ch, 1) != 0) {
                gPakMenuLeaving = 1;
                MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, menu->prompt, menu->player, menu->slot, 0);
                break;
            }
            Pak_ProbeSlot(ch);
            if (Pak_DeleteNote(ch, gPakNoteIndex) == 0) {
                ((s32 (*)(void *, MenuRecAt24 *, void *)) PakMenu_WriteSaveGame)(owner, menu, arg); /* passes a third word the definition does not take */
                return 1;
            }
            MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, (char *)&gPakMsgDeleteFailedDesc, menu->player, menu->slot, menu->prompt);
            break;
        case 0:
            if (gPakMenuAnyPak != 0) {
                ch = gPakMenuPort;
            } else {
                ch = menu->slot->unk4;
            }
            if (PakMenu_CheckPak((struct Menu *) menu, ch, 1) != 0) {
                gPakMenuLeaving = 1;
                MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, menu->prompt, menu->player, menu->slot, 0);
                break;
            }
            Pak_ProbeSlot(ch);
            if (Pak_RepairIfDamaged(ch) == 0) {
                MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, (char *)&gPakMsgRepairedDesc, menu->player, menu->slot, menu->prompt);
            } else {
                MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, (char *)&gDamagedPakInGameMenuDesc, menu->player, menu->slot, menu->prompt);
            }
            break;
        case 1:
        case 2:
            if (gPakMenuAnyPak != 0) {
                gPakMenuConfirmed = 1;
            }
            MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, menu->prompt, menu->player, menu->slot, 0);
            break;
    }
    return 1;
}
