#include "common.h"
#include "globals/game.h"


s32 PlayerSetupMenu_Poll(void)
{
  s8 *new_var;
  s32 var_a0;
  new_var = (s8 *) (&gEngineApp_m_Settings.match);
  var_a0 = 0;
  if ((((*((s32 *) (new_var + 0x28))) != 0) || ((*((s32 *) (((s8 *) (&gEngineApp_m_Settings.match)) + 0x1C))) != 0)) || ((*((s32 *) (new_var + 0x20))) != 0))
  {
    var_a0 = 1;
  }
  return var_a0;
}
