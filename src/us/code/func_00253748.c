
#include "common.h"
#include "globals/game.h"
u32 CPlayer__GetPickupCountHigh(s32 arg0, s32 arg1)
{
  s32 new_var;
  if (!(gEngineApp_m_Settings.cheatFlags & 0x10))
  {
    if (new_var)
    {
      new_var = arg0;
      return ((u8) (*((u8 *) (((s8 *) (arg1 + new_var)) + (-0xA74))))) >> 4;
    }
    else
    {
      new_var = arg0;
      return ((u8) (*((u8 *) (((s8 *) (arg1 + new_var)) + (-0xA74))))) >> 4;
    }
  }
  return 1U;
}
