#include "common.h"
#include "m2c_macros.h"
#include "globals/pak.h"


void Controller_WatchAllPaks(void)
{
  s32 var_v1;
  s8 *var_v0;
  int new_var;
  new_var = 1;
  var_v1 = 3;
  var_v0 = (s8 *) &gPakWatched[3];
  do
  {
    *var_v0 = new_var;
    var_v1 -= 1;
    var_v0 -= 1;
  }
  while (var_v1 >= 0);
}
