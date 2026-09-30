#include "common.h"
#include "m2c_macros.h"

s32 func_00247124(s32 arg0)
{
  int new_var;
  s32 var_v1;
  new_var = 0x80000;
  var_v1 = arg0 & 0xFFF6FFF3;
  if (arg0 & 0x10000)
  {
    var_v1 |= 8;
  }
  if (arg0 & 8)
  {
    var_v1 |= 0x10000;
  }
  if (arg0 & new_var)
  {
    var_v1 |= 4;
  }
  if (arg0 & 4)
  {
    var_v1 |= 0x80000;
  }
  return var_v1;
}
