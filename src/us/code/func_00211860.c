#include "common.h"
#include "globals/engine.h"

s32 Math_StepClampedInt(s32 arg0, s32 arg1, s32 arg2)
{
  f32 temp_f1;
  s32 var_a0;
  s32 var_a2;
  s32 var_v0;
  var_a2 = arg2;
  temp_f1 = (f32) arg1;
  var_a0 = arg0 + ((s32) (temp_f1 * gFrameIncrement));
  if (temp_f1 < 0.0f)
  {
    var_a2 = -var_a2;
    var_v0 = var_a0 < var_a2;
  }
  else
  {
    var_v0 = var_a2 < var_a0;
  }
  if (var_v0 != 0)
  {
    var_a0 = var_a2;
  }
  if (var_a2)
  {
    return var_a0;
  }
  else
  {
    return var_a0;
  }
}
