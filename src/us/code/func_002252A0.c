#include "common.h"
#include "m2c_macros.h"

s32 func_0026E294(s32);

void func_002252A0(void *arg0, s32 arg1)
{
  s32 var_s0;
  s32 var_s3;
  s32 var_s4;
  int new_var;
  void *var_s1;
  void *var_s2;
  var_s2 = arg0;
  var_s0 = *((s32 *) (((s8 *) var_s2) + 0x1168));
  var_s1 = *((void **) (((s8 *) var_s2) + 0x1178));
  var_s0 -= 1;
  var_s3 = arg1;
  if (var_s0 != (-1))
  {
    var_s4 = -1;
    do
    {
      if ((*((u8 *) (((s8 *) var_s1) + 0xF))) == var_s3)
      {
        func_0026E294((s32) var_s1);
      }
      var_s0 -= 1;
      var_s1 += 0x14;
    }
    while (var_s0 != (-1));
  }
  var_s0 = *((s32 *) (((s8 *) var_s2) + 0x116C));
  var_s1 = *((void **) (((s8 *) var_s2) + 0x117C));
  var_s0 -= 1;
  new_var = -1;
  var_s2 = (void *) (-1);
  if (var_s0 != new_var)
  {
    do
    {
      if ((*((u8 *) (((s8 *) var_s1) + 0xF))) == var_s3)
      {
        func_0026E294((s32) var_s1);
      }
      var_s0 -= 1;
      var_s1 += 0x14;
    }
    while (var_s0 != (-1));
  }
}
