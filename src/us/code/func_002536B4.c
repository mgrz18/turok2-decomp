
#include "common.h"
#include "globals/game.h"
s32 CPlayer__IsSpecialPickup(s32 arg0, s32 arg1)
{
  int new_var;
  if (!(gEngineApp_m_Settings.cheatFlags & 0x10))
  {
 if (gEngineApp_m_Settings.cheatFlags) { new_var = 0xFA0; } else { new_var = 0xFA0; } do { do { } while (0); } while (0);
    return ((u32) (arg1 - new_var)) < 0x401U;
  }
  return 0;
}
