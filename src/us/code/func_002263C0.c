#include "common.h"
#include "m2c_macros.h"
#include "globals/engine.h"

s32 CStaticObject__SpawnEvents(s32, s32);


void CScene__AdvanceListedInstances(void *arg0)
{
  f32 temp_f1;
  int new_var;
  s32 temp_a0;
  s32 var_s0;
  s32 var_s3;
  void *var_s1;
  temp_f1 = (*((f32 *) (((s8 *) arg0) + 0x18EB8))) + gFrameIncrement;
  *((f32 *) (((s8 *) arg0) + 0x18EB8)) = temp_f1;
  var_s3 = 1;
  if (!(temp_f1 > 0.0f))
  {
    var_s3 = 0;
  }
  if (var_s3 != 0)
  {
    *((f32 *) (((s8 *) arg0) + 0x18EB8)) = (f32) (temp_f1 - ((f32) (((s32) temp_f1) + 1)));
  }
  var_s0 = 0;
  new_var = 0;
  if ((*((s32 *) (((s8 *) arg0) + 0xBF4))) > new_var)
  {
    var_s1 = arg0;
    do
    {
      temp_a0 = *((s32 *) (((s8 *) var_s1) + 0xAF4));
      var_s1 += 4;
      CStaticObject__SpawnEvents(temp_a0, var_s3);
      var_s0 += 1;
    }
    while (var_s0 < (*((s32 *) (((s8 *) arg0) + 0xBF4))));
  }
}
