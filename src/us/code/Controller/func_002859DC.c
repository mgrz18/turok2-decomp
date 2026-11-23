#include "common.h"
#include "m2c_macros.h"

extern u8 gContStatus_errno[]; /* local: the field symbol; gContStatus[n].errno makes clang load gContStatus as an aligned vector on the host (see the names table) */

s32 Controller_CountConnected(void)
{
  s32 var_a0;
  s32 var_v0;
  s32 var_v1;
  var_a0 = 0;
  var_v1 = 0;
  var_v0 = 0 * 4;
  do
  {
    var_v0 = var_v1 * 4;
    if (((((u8) (*((u8 *) (gContStatus_errno + var_v0)))) >> 3) ^ 1) & 1)
    {
      var_a0 += 1;
    }
    var_v1 += 1;
  }
  while (var_v1 < 4);
  return var_a0;
}
