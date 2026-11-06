#include "common.h"
#include "structs/menu.h"

s32 Controller_DisableMenuSounds(void *, void *);              /* extern */
extern s32 gPakMessageKind[];

void PakMessage_InitNoController(Menu *arg0)
{
  s32 new_var2;
  MenuItem *temp_a0;
  MenuItem *temp_a1;
  int new_var;
  temp_a1 = arg0->items;
  new_var = temp_a1[7].flags & 0xFBFFFFFF;
  temp_a1[7].flags = (s32) new_var;
  temp_a0 = arg0->items;
  new_var2 = temp_a0[7].flags;
  gPakMessageKind[0] = 8;
  temp_a0[7].flags = (s32) (new_var2 | 8);
  Controller_DisableMenuSounds(temp_a0, temp_a1);
}
