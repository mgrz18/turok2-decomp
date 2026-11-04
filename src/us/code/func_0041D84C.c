#include "common.h"
#include "structs/game.h"

s32 func_0041D84C(void)
{
  s32 var_a0;
  GameMatch *new_var;
  var_a0 = 0;
  new_var = &D_8011ACB0;
  if (((D_8011ACB0.teamOver != 0) || (new_var->over != 0)) || (new_var->done != 0))
  {
    var_a0 = 1;
  }
  return var_a0;
}
