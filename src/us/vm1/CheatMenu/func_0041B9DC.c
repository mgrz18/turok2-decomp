#include "common.h"
#include "globals/menu_text.h"
#include "structs/menu.h"
#include "globals/game.h"

s32 CheatMenu_FrootyStripesText(MenuItem *item)
{
  s32 var_v0;
  int new_var;
  if (gEngineApp_m_Settings.unlockedCheats & 0x8000)
  {
 new_var = 0x01000000; do { var_v0 = item->flags | new_var; } while (0);
  }
  else
  {
    var_v0 = item->flags & 0xFEFFFFFF;
  }
  *(s32 *)&item->flags = var_v0; /* not a member store: then it may alias gEngineApp_m_Settings and stays first, as in the ROM */
  if (gEngineApp_m_Settings.cheatFlags & 0x8000)
  {
    item->text = sCheatMenuText_FrootyStripesOn;
  }
  else
  {
    item->text = sCheatMenuText_FrootyStripesOff;
  }
  return 0;
}
