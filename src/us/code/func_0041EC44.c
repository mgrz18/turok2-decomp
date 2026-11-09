/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80409144_de_7.c, func_804070F4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/menu.h"
#include "globals/pak.h"
#include "globals/menu_desc.h"
#include "protos/menu.h"

void PakMenu_UpdateNoteRows(Shared_PakMenu *menu);
extern void Pak_ProbeSlot(s32 ch);
extern s32 Pak_GetStatus(s32 ch);
extern s32 Pak_IsRumblePak(s32 ch);
extern s32 Pak_NeedsRepair(s32 ch);
void PakMenu_InitLoadGame(Shared_PakMenu *menu)
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
    MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, (char *)&gSingleMenuAfterPakDesc, menu->player, menu->slot, 0);
    return;
  }
  menu->items[20].flags &= ~0x01000000;
  Pak_ProbeSlot(menu->slot->unk4);
  new_var = 0;
  if (Pak_GetStatus(ch) == new_var)
  {
    PakMenu_UpdateNoteRows(menu);
    return;
  }
  gPakMenuLeaving = 1;
  if (Pak_IsRumblePak(ch) != new_var)
  {
    new_var2 = (char *)&gPakMsgInsertToLoadDesc;
    MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, new_var2, menu->player, menu->slot, (char *) 1);
  }
  else
  {
    new_var3 = menu;
    if (Pak_NeedsRepair(ch) != 0)
    {
      MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, (char *)&gDamagedPakAtBootMenuDesc, new_var3->player, menu->slot, (unsigned char *)&gPakLoadGameMenuDesc);
    }
    else
    {
      MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, (char *)&gPakMsgPakNotFoundDesc, new_var3->player, new_var3->slot, (char *)&gSingleMenuAfterPakDesc);
    }
  }
}
