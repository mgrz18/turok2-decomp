#include "common.h"

extern s32 gEngineApp_m_Settings_match;

s32 StatsMenu_IsMatchOver_2(void)
{
  s8 *new_var;
  s32 var_v1;
  new_var = (s8 *) (&gEngineApp_m_Settings_match);
  var_v1 = 0;
  if (((*((s32 *) (new_var + 0x1C))) != 0) || ((*((s32 *) (new_var + 0x20))) != 0))
  {
    var_v1 = 1;
  }
  return var_v1;
}
