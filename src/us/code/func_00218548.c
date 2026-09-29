#include "common.h"
#include "m2c_macros.h"

void func_00218548(void *arg0, s32 arg1)
{
  s32 var_a2;
  void *var_a0;
  int new_var;
  var_a2 = 1;
  *((s32 *) (((s8 *) arg0) + 0)) = arg1;
  *((s32 *) (((s8 *) arg0) + 4)) = (s32) (((arg1 * 4) + 0xF) & (~7));
  if (arg1 > 0)
  {
    new_var = 0xDEADBEEF;
    var_a0 = arg0 + 4;
    do
    {
      *((s32 *) (((s8 *) var_a0) + 4)) = new_var;
      var_a2 += 1;
      var_a0 += 4;
    }
    while (arg1 >= var_a2);
  }
}
