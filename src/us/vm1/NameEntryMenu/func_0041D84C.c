#include "common.h"
#include "structs/game.h"
#include "globals/game.h"

s32 NameEntryMenu_IsMatchOver(void)
{
  s32 var_a0;
  GameMatch *new_var;
  var_a0 = 0;
  new_var = &gEngineApp_m_Settings.match;
  if (((gEngineApp_m_Settings.match.teamOver != 0) || (new_var->over != 0)) || (new_var->done != 0))
  {
    var_a0 = 1;
  }
  return var_a0;
}
