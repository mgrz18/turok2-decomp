#include "common.h"
#include "m2c_macros.h"

s32 func_0025E52C(s32, s32, s32, s32);

void func_00225C08(void *arg0, s32 arg1, s32 arg2)
{
  s32 temp_a0;
  s32 temp_s2;
  s32 var_s0;
  void *var_s1;
  if (temp_s2)
  {
    temp_s2 = *((s32 *) (((s8 *) arg0) + 0xEFC));
    var_s0 = 0;
  }
  else
  {
    temp_s2 = *((s32 *) (((s8 *) arg0) + 0xEFC));
    var_s0 = 0;
  }
  if (temp_s2 > 0)
  {
    var_s1 = arg0;
    do
    {
      temp_a0 = *((s32 *) (((s8 *) var_s1) + 0xDFC));
      var_s1 += 4;
      var_s0 += 1;
      func_0025E52C(temp_a0, temp_a0 + 0x140, arg2, arg1);
    }
    while (var_s0 < temp_s2);
  }
}
