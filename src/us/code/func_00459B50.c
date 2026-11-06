/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00409170.c, func_00409170.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"
#include "globals/menu.h"

extern s32 gPakConfirmKind;
extern s32 gPakMenuAnyPak;
extern s32 gPakMenuPort;
extern s32 gPakMenuLeaving;
extern s32 gPakOverwriteNote;
extern s32 gPakNoteIndex;
extern s32 D_8011B114;
extern u8 gPakMsgDeleteDoneDesc_2[];
extern u8 gPakMsgDeleteFailedDesc_2[];
extern u8 gPakMsgRepairedDesc_2[];
extern u8 gDamagedPakInGameMenuDesc_2[];
s32 PakMenu_CheckPak_2(Menu *o, s32 pad, s32 z);
void Pak_ProbeSlot_2(s32 ch);
s32 Pak_DeleteNote_2(s32 pad, s32 idx);
s32 Pak_RepairIfDamaged_2(s32 pad);
s32 PakMenu_WriteSaveGame_2(s32 a, Menu *o, s32 c);
void MenuPool_OpenMenu_2(u8 *a, void *b, void *c, void *d, void *e);
s32 PakConfirmMenu_SelectYes_2(s32 a, Menu *own, s32 c) {
    s32 pad;
    switch (gPakConfirmKind) {
    case 4:
        if (gPakMenuAnyPak != 0) {
            pad = gPakMenuPort;
        } else {
            pad = own->controller->port;
        }
        if (PakMenu_CheckPak_2(own, pad, 1) != 0) {
            gPakMenuLeaving = 1;
            MenuPool_OpenMenu_2(&gEngineApp_m_Game_m_MenuPool, own->context, own->player, own->controller, 0);
        } else {
            Pak_ProbeSlot_2(pad);
            if (Pak_DeleteNote_2(pad, gPakOverwriteNote) == 0) {
                MenuPool_OpenMenu_2(&gEngineApp_m_Game_m_MenuPool, gPakMsgDeleteDoneDesc_2, own->player, own->controller, own->context);
            } else {
                MenuPool_OpenMenu_2(&gEngineApp_m_Game_m_MenuPool, gPakMsgDeleteFailedDesc_2, own->player, own->controller, own->context);
            }
        }
        break;
    case 3:
        if (gPakMenuAnyPak != 0) {
            pad = gPakMenuPort;
        } else {
            pad = own->controller->port;
        }
        if (PakMenu_CheckPak_2(own, pad, 1) != 0) {
            gPakMenuLeaving = 1;
            MenuPool_OpenMenu_2(&gEngineApp_m_Game_m_MenuPool, own->context, own->player, own->controller, 0);
        } else {
            Pak_ProbeSlot_2(pad);
            if (Pak_DeleteNote_2(pad, gPakNoteIndex) == 0) {
                PakMenu_WriteSaveGame_2(a, own, c);
                return 1;
            } else {
                MenuPool_OpenMenu_2(&gEngineApp_m_Game_m_MenuPool, gPakMsgDeleteFailedDesc_2, own->player, own->controller, own->context);
            }
        }
        break;
    case 0:
        if (gPakMenuAnyPak != 0) {
            pad = gPakMenuPort;
        } else {
            pad = own->controller->port;
        }
        if (PakMenu_CheckPak_2(own, pad, 1) != 0) {
            gPakMenuLeaving = 1;
            MenuPool_OpenMenu_2(&gEngineApp_m_Game_m_MenuPool, own->context, own->player, own->controller, 0);
        } else {
            Pak_ProbeSlot_2(pad);
            if (Pak_RepairIfDamaged_2(pad) == 0) {
                MenuPool_OpenMenu_2(&gEngineApp_m_Game_m_MenuPool, gPakMsgRepairedDesc_2, own->player, own->controller, own->context);
            } else {
                MenuPool_OpenMenu_2(&gEngineApp_m_Game_m_MenuPool, gDamagedPakInGameMenuDesc_2, own->player, own->controller, own->context);
            }
        }
        break;
    case 1:
    case 2:
        if (gPakMenuAnyPak != 0) {
            D_8011B114 = 1;
        }
        MenuPool_OpenMenu_2(&gEngineApp_m_Game_m_MenuPool, own->context, own->player, own->controller, 0);
        break;
    }
    return 1;
}
