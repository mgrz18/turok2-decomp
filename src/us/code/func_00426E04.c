#include "common.h"
#include "m2c_macros.h"

u8 *func_00426E04(u8 *arg0, u8 *arg1)
{
  u8 *var_v1;
  u8 temp_v0;
  int new_var;
  u8 temp_v0_2;
  temp_v0 = *arg1;
  arg1 = arg1 + 1;
  *arg0 = temp_v0;
  new_var = 1;
  var_v1 = arg0 + new_var;
  if (temp_v0 & 0xFF)
  {
    do
    {
      temp_v0_2 = *arg1;
      arg1 += 1;
      *var_v1 = temp_v0_2;
      var_v1 += 1;
    }
    while (temp_v0_2 & 0xFF);
  }
  return arg0;
}
