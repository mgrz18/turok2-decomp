#include "common.h"
#include "structs/menu.h"

extern char D_800B5AF4[];
extern char D_800B5B10[];
extern s32 D_8011AAD8;
extern s32 D_8011AADC;
s32 func_0041B9DC(MenuItem *item)
{
  s32 var_v0;
  int new_var;
  if (D_8011AADC & 0x8000)
  {
 new_var = 0x01000000; do { var_v0 = item->flags | new_var; } while (0);
  }
  else
  {
    var_v0 = item->flags & 0xFEFFFFFF;
  }
  *(s32 *)&item->flags = var_v0; /* not a member store: then it may alias D_8011AAD8 and stays first, as in the ROM */
  if (D_8011AAD8 & 0x8000)
  {
    item->text = D_800B5AF4;
  }
  else
  {
    item->text = D_800B5B10;
  }
  return 0;
}
