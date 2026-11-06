#include "common.h"
#include "structs/menu.h"

extern char D_800B5AF4[];
extern char D_800B5B10[];
extern s32 gEngineApp_m_Settings;
extern s32 gEngineApp_m_Settings_unlockedCheats;
s32 CheatMenu_FrootyStripesText(MenuItem *item)
{
  s32 var_v0;
  int new_var;
  if (gEngineApp_m_Settings_unlockedCheats & 0x8000)
  {
 new_var = 0x01000000; do { var_v0 = item->flags | new_var; } while (0);
  }
  else
  {
    var_v0 = item->flags & 0xFEFFFFFF;
  }
  *(s32 *)&item->flags = var_v0; /* not a member store: then it may alias gEngineApp_m_Settings and stays first, as in the ROM */
  if (gEngineApp_m_Settings & 0x8000)
  {
    item->text = D_800B5AF4;
  }
  else
  {
    item->text = D_800B5B10;
  }
  return 0;
}
