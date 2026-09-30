#include "common.h"
#include "m2c_macros.h"

s32 func_00205904(s32, s32, s32);
s32 func_00213A70(s32, s32, s32, s32, s32);

void func_00214F14(void *arg0, s32 **arg1)
{
  s32 new_var;
  new_var = *(*arg1);
  func_00205904(0, (s32) arg1, func_00213A70(new_var, *(*((s32 **) (((s8 *) arg0) + 0x24))), (s32) (arg0 + 0x68), (s32) (arg0 + 0x28), 0));
}
