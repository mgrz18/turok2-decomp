/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/PAK/80405DC0_7.CPP, func_804066BC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/camera.h"
#include "structs/player.h"
#include "structs/menu.h"
#include "globals/menu.h"
#include "globals/pak.h"
#include "globals/menu_desc.h"
#include "structs/camera.h"

void PakMenu_UpdateNoteRows_2(Menu *menu);
extern void Pak_ProbeSlot_2(s32 ch);
extern s32 Pak_GetStatus_2(s32 ch);
extern s32 Pak_IsRumblePak_2(s32 ch);
extern s32 Pak_NeedsRepair_2(s32 ch);
extern void MenuPool_OpenMenu_2(char *, char *, CPlayer *, ControllerProfile *, char *);
void PakMenu_InitLoadProfile_2(Menu *menu) {
    s32 ch;
    gPakMenuIsSave = 0;
    gPakMenuWrites = 0;
    gPakMenuIsLoad = 1;
    gPakMenuIsProfile = 1;
    gPakMenuMode = 1;
    gPakMenuLeaving = 0;
    gPakMenuAnyPak = 0;
    menu->items[20].flags &= ~0x01000000;
    ch = menu->controller->port;
    if (gDamagedPakContinued != 0) {
        gPakMenuLeaving = 1;
        MenuPool_OpenMenu_2(((char *)menu->player->m_pCamera) + T2_OFFSETOF(CCamera, m_MenuPool), (char *)&gCharacterMenuAfterPakDesc_2, menu->player, menu->controller, 0);
        return;
    }
    Pak_ProbeSlot_2(ch);
    if (Pak_GetStatus_2(ch) == 0) {
        PakMenu_UpdateNoteRows_2(menu);
        return;
    }
    gPakMenuLeaving = 1;
    if (Pak_IsRumblePak_2(ch) != 0) {
        MenuPool_OpenMenu_2(&gEngineApp_m_Game_m_MenuPool, (char *)&gPakMsgInsertToLoadDesc_2, menu->player, menu->controller, (char *)&gCharacterMenuAfterPakDesc_2);
    } else if (Pak_NeedsRepair_2(ch) != 0) {
        MenuPool_OpenMenu_2(&gEngineApp_m_Game_m_MenuPool, (char *)&gDamagedPakAtBootMenuDesc_2, menu->player, menu->controller, (char *)&gPakLoadProfileMenuDesc_2);
    } else {
        MenuPool_OpenMenu_2(&gEngineApp_m_Game_m_MenuPool, (char *)&gPakMsgPakNotFoundDesc_2, menu->player, menu->controller, (char *)&gCharacterMenuAfterPakDesc_2);
    }
}
