#include "common.h"
#include "m2c_macros.h"


f32 func_002118B0(f32 arg0)
{
  f32 var_f12;
  float new_var;
  var_f12 = arg0;
  new_var = -3.1415927f;
  if (var_f12 < new_var)
  {
    do
    {
      var_f12 += 6.2831855f;
    }
    while (var_f12 < (-3.1415927f));
  }
  new_var = 3.1415927f;
  if (var_f12 > new_var)
  {
    do
    {
      var_f12 -= 6.2831855f;
    }
    while (var_f12 > new_var);
  }
  return -var_f12;
}
