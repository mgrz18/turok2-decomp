#include "common.h"
#include "structs/controller.h"
#include "m2c_macros.h"


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
    if (((((u8) (*((u8 *) ((u8 *) &gContStatus[0].errno + var_v0)))) >> 3) ^ 1) & 1)
    {
      var_a0 += 1;
    }
    var_v1 += 1;
  }
  while (var_v1 < 4);
  return var_a0;
}
