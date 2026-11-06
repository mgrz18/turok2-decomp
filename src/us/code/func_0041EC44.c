/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80409144_de_7.c, func_804070F4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

void PakMenu_UpdateNoteRows(Shared_PakMenu *menu);
extern s32 D_80130994;
extern s32 D_801309BC;
extern s32 D_80130984;
extern s32 D_801309B4;
extern s32 D_801309A0;
extern s32 D_800C2024;
extern s32 D_80130990;
extern s32 D_800C2020;
extern char D_80119E2C[];
extern char D_00439190[];
extern unsigned char D_0043B6F0[];
extern char D_0043BC6C[];
extern char D_0043C05C[];
extern char D_0043BC00[];
extern void Pak_ProbeSlot(s32 ch);
extern s32 Pak_GetStatus(s32 ch);
extern s32 Pak_IsRumblePak(s32 ch);
extern s32 Pak_NeedsRepair(s32 ch);
extern void MenuPool_OpenMenu(char *, char *, ObjRecAt5DC *, PakMenuSlot *, char *);
void PakMenu_InitLoadGame(Shared_PakMenu *menu)
{
  Shared_PakMenu *new_var3;
  int new_var;
  char *new_var2;
  s32 ch;
  new_var = 1;
  D_800C2024 = 2;
  D_80130984 = 0;
  D_801309A0 = 0;
  D_80130994 = new_var;
  D_801309B4 = 0;
  D_801309BC = 0;
  D_80130990 = 0;
  ch = menu->slot->unk4;
  if (D_800C2020 != 0)
  {
    D_801309BC = 1;
    MenuPool_OpenMenu(D_80119E2C, D_00439190, menu->player, menu->slot, 0);
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
  D_801309BC = 1;
  if (Pak_IsRumblePak(ch) != new_var)
  {
    new_var2 = D_0043BC6C;
    MenuPool_OpenMenu(D_80119E2C, new_var2, menu->player, menu->slot, (char *) 1);
  }
  else
  {
    new_var3 = menu;
    if (Pak_NeedsRepair(ch) != 0)
    {
      MenuPool_OpenMenu(D_80119E2C, D_0043C05C, new_var3->player, menu->slot, D_0043B6F0);
    }
    else
    {
      MenuPool_OpenMenu(D_80119E2C, D_0043BC00, new_var3->player, new_var3->slot, D_00439190);
    }
  }
}
