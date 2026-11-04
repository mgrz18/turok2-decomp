#include "common.h"
#include "structs/menu.h"

s32 func_00285CC4(void *, void *);              /* extern */
extern s32 D_801309C0[];

void func_00422764(Menu *arg0)
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
  D_801309C0[0] = 8;
  temp_a0[7].flags = (s32) (new_var2 | 8);
  func_00285CC4(temp_a0, temp_a1);
}
