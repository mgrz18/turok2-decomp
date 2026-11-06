#include "common.h"

extern s32 gEngineApp_m_Settings_match;

s32 CharacterMenu_Poll(void)
{
  s8 *new_var;
  s32 var_a0;
  var_a0 = 0;
  new_var = (s8 *) (&gEngineApp_m_Settings_match);
  if ((((*((s32 *) (((s8 *) (&gEngineApp_m_Settings_match)) + ((0x28 & 0xFFFFFFFF) & 0xFFFFFFFF)))) != 0) || ((*((s32 *) (new_var + 0x1C))) != 0)) || ((*((s32 *) (((s8 *) (&gEngineApp_m_Settings_match)) + 0x20))) != 0))
  {
    var_a0 = 1;
  }
  return var_a0;
}
