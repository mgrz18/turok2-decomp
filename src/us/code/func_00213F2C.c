#include "common.h"
#include "m2c_macros.h"

f32 func_00213F2C(f32 arg0, f32 arg1)
{
  f32 temp_f12;
  f32 var_f0;
  f32 var_f0_2;
  s32 temp_f1;
  s32 var_v0;
  var_f0 = 0.0f;
  if (arg1 != 0.0f)
  {
    temp_f12 = arg0 / arg1;
    if (temp_f12 >= 0.0f)
    {
      var_v0 = (s32) temp_f12;
      goto block_5;
    }
    temp_f1 = (s32) temp_f12;
    var_f0_2 = (f32) temp_f1;
    if (var_f0_2 != temp_f12)
    {
      var_v0 = temp_f1 - 1;
      block_5:
      var_f0_2 = (f32) var_v0;

    }
    var_f0 = temp_f12 - var_f0_2;
    var_f0 = var_f0 * arg1;
  }
  return var_f0;
}
