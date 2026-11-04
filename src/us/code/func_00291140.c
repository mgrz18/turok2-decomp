#include "common.h"

s32 func_002933E0(s32, s32);
s32 osSetIntMask(s32);

s32 func_002933B0(s32 *);                   /* extern */

void func_00291140(void *arg0)
{
  s32 *temp_s0;
  s32 *var_s1;
  s32 temp_s3;
  temp_s3 = osSetIntMask(1);
  var_s1 = *((s32 **) (((s8 *) arg0) + 8));
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
  osSetIntMask(temp_s3);
}
