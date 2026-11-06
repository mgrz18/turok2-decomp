
#include "common.h"
#include "globals/game.h"
s32 CPlayer__GetPickupCountLow(s32 arg0, s32 arg1)
{
  unsigned long long new_var;
  new_var = 1;
  if (!(gEngineApp_m_Settings.cheatFlags & 0x10))
  {
    return (*((u8 *) (((s8 *) (arg1 + arg0)) + (-0xA74)))) & 0xF;
  }
  if (new_var)
  {
    return new_var;
  }
  else
  {
    return new_var;
  }
}
