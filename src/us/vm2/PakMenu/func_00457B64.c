/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80409144_de_7.c, func_804070F4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "protos/pak_2.h"
#include "globals/menu.h"
#include "structs/menu.h"
#include "globals/pak.h"
#include "globals/menu_desc.h"
#include "protos/menu.h"
#include "protos/pak.h"
#include "protos/pakmenu.h"

#ifdef TARGET_PORT
/* The port: the Rage Wars view (Shared_PakMenu, PakNotesMenuItem) puts the items, the player and the
 * controller at N64 offsets (0xC, 0x1C, 0x20; items 0x28 apart): on the host it read the channel from
 * the menu's param and cleared the disabled bit of the wrong item (the load button stayed disabled,
 * the cursor bounced between it and CANCEL). The same code on the typed Menu (structs/menu.h). */
void PakMenu_InitLoadGame_2(Menu *menu) {
    s32 ch;

    gPakMenuMode = 2;
    gPakMenuIsSave = 0;
    gPakMenuWrites = 0;
    gPakMenuIsLoad = 1;
    gPakMenuIsProfile = 0;
    gPakMenuLeaving = 0;
    gPakMenuAnyPak = 0;
    ch = menu->controller->port;
    if (gDamagedPakContinued != 0) {
        gPakMenuLeaving = 1;
        MenuPool_OpenMenu_2(&gEngineApp_m_Game_m_MenuPool, (char *) &gSingleMenuAfterPakDesc_2, (sptr) menu->player,
                          (sptr) menu->controller, 0);
        return;
    }
    menu->items[20].flags &= ~0x01000000;
    Pak_ProbeSlot_2(menu->controller->port);
    if (Pak_GetStatus_2(ch) == 0) {
        PakMenu_UpdateNoteRows_2((struct Menu *) menu);
        return;
    }
    gPakMenuLeaving = 1;
    if (Pak_IsRumblePak_2(ch) != 0) {
        MenuPool_OpenMenu_2(&gEngineApp_m_Game_m_MenuPool, (char *) &gPakMsgInsertToLoadDesc_2, (sptr) menu->player,
                          (sptr) menu->controller, 1);
    } else if (Pak_NeedsRepair_2(ch) != 0) {
        MenuPool_OpenMenu_2(&gEngineApp_m_Game_m_MenuPool, (char *) &gDamagedPakAtBootMenuDesc_2, (sptr) menu->player,
                          (sptr) menu->controller, (sptr) &gPakLoadGameMenuDesc_2);
    } else {
        MenuPool_OpenMenu_2(&gEngineApp_m_Game_m_MenuPool, (char *) &gPakMsgPakNotFoundDesc_2, (sptr) menu->player,
                          (sptr) menu->controller, (sptr) &gSingleMenuAfterPakDesc_2);
    }
}
#else
/* The port body's signature; the N64 body reads the menu through the Rage Wars view (a macro: a local copy
 * would move a register). */
#define menu ((Shared_PakMenu *) menuArg)
void PakMenu_InitLoadGame_2(Menu *menuArg)
{
  Shared_PakMenu *new_var3;
  int new_var;
  char *new_var2;
  s32 ch;
  new_var = 1;
  gPakMenuMode = 2;
  gPakMenuIsSave = 0;
  gPakMenuWrites = 0;
  gPakMenuIsLoad = new_var;
  gPakMenuIsProfile = 0;
  gPakMenuLeaving = 0;
  gPakMenuAnyPak = 0;
  ch = menu->slot->unk4;
  if (gDamagedPakContinued != 0)
  {
    gPakMenuLeaving = 1;
    MenuPool_OpenMenu_2(&gEngineApp_m_Game_m_MenuPool, (char *)&gSingleMenuAfterPakDesc_2, menu->player, menu->slot, 0);
    return;
  }
  menu->items[20].flags &= ~0x01000000;
  Pak_ProbeSlot_2(menu->slot->unk4);
  new_var = 0;
  if (Pak_GetStatus_2(ch) == new_var)
  {
    PakMenu_UpdateNoteRows_2((struct Menu *) menu);
    return;
  }
  gPakMenuLeaving = 1;
  if (Pak_IsRumblePak_2(ch) != new_var)
  {
    new_var2 = (char *)&gPakMsgInsertToLoadDesc_2;
    MenuPool_OpenMenu_2(&gEngineApp_m_Game_m_MenuPool, new_var2, menu->player, menu->slot, (char *) 1);
  }
  else
  {
    new_var3 = menu;
    if (Pak_NeedsRepair_2(ch) != 0)
    {
      MenuPool_OpenMenu_2(&gEngineApp_m_Game_m_MenuPool, (char *)&gDamagedPakAtBootMenuDesc_2, new_var3->player, menu->slot, (unsigned char *)&gPakLoadGameMenuDesc_2);
    }
    else
    {
      MenuPool_OpenMenu_2(&gEngineApp_m_Game_m_MenuPool, (char *)&gPakMsgPakNotFoundDesc_2, new_var3->player, new_var3->slot, (char *)&gSingleMenuAfterPakDesc_2);
    }
  }
}
#undef menu
#endif
