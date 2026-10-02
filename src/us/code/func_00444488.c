#include "common.h"
#include "m2c_macros.h"

s32 func_00243414(s32, s32, s32);
s32 func_00409A64(s32, s32);

extern M2C_UNK D_8012FB70;

void func_00444488(s32 arg0, void *arg1)
{
  s8 *new_var;
  s32 temp_v0;
  if ((*((s8 *) (((s8 *) arg1) + 0xC7))) != 0)
  {
    new_var = (s8 *) (&D_8012FB70);
    if ((*((s32 *) (((s8 *) (&D_8012FB70)) + 0x67C))) == (((*((s32 *) (((s8 *) (&D_8012FB70)) + 0x67C))) / 3) * 3))
    {
      func_00409A64(arg0, (*((s32 *) (new_var + 0x67C))) == 0);
    }
    temp_v0 = (*((s32 *) (((s8 *) (&D_8012FB70)) + 0x67C))) + 1;
    *((s32 *) (((s8 *) (&D_8012FB70)) + 0x67C)) = temp_v0;
    if (temp_v0 >= 0x1F)
    {
      func_00243414(arg0, (s32) arg1, 0x11);
    }
  }
}
