#include "common.h"
#include "m2c_macros.h"

extern u8 D_800F677B[];

s32 func_002859DC(void)
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
    if (((((u8) (*((u8 *) (D_800F677B + var_v0)))) >> 3) ^ 1) & 1)
    {
      var_a0 += 1;
    }
    var_v1 += 1;
  }
  while (var_v1 < 4);
  return var_a0;
}
