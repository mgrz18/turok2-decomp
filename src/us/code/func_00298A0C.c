#include "globals/libultra.h"

void __osPackResetData(void)
{
  s32 var_v0;
  s8 *var_v1;
  int new_var;
  var_v1 = (s8 *) &__osContBuffer;
  __osContBuffer.pifstatus = 1;
  new_var = 0xFD;
  var_v0 = 3;
  do
  {
    *var_v1 = new_var;
    var_v0 -= 1;
    var_v1 += 1;
  }
  while (var_v0 >= 0);
  var_v0 = 0xFE;
  *var_v1 = var_v0;
}
