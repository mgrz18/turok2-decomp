#include "common.h"
#include "m2c_macros.h"

s32 func_002933E0(s32, s32);
s32 func_0029E230(s32);

M2C_UNK func_002933B0(M2C_UNK *);                   /* extern */

void func_00291140(void *arg0)
{
  M2C_UNK *temp_s0;
  M2C_UNK *var_s1;
  s32 temp_s3;
  temp_s3 = func_0029E230(1);
  var_s1 = *((M2C_UNK **) (((s8 *) arg0) + 8));
  if (var_s1 != 0)
  {
    do
    {
      temp_s0 = *var_s1;
      func_002933B0(var_s1);
      func_002933E0((s32) var_s1, (s32) arg0);
      var_s1 = temp_s0;
    }
    while (var_s1 != 0);
  }
  func_0029E230(temp_s3);
}
