#include "common.h"
#include "m2c_macros.h"

s32 func_002051F4(s32, s32);
s32 func_00224DF4(s32, s32, s32, s32);
s32 func_0027AE44(s32, s32, s32, s32);

extern s32 D_800B6D54;
extern M2C_UNK D_800F7078;

void func_0027AEA8(s32 arg0, s32 arg1, s32 *arg2, s32 *arg3)
{
  void *sp10;
  M2C_UNK sp14;
  void *new_var;
  s32 temp_v0;
  if (D_800B6D54 != 0)
  {
    temp_v0 = func_00224DF4((s32) (&D_800F7078), arg0, 0, 1);
    if (temp_v0 != 0)
    {
      func_0027AE44(temp_v0, arg1, (s32) (&sp10), (s32) (&sp14));
      new_var = sp10;
      *arg2 = (s32) (*((u16 *) (((s8 *) new_var) + 4)));
      *arg3 = (s32) (*((u16 *) (((s8 *) new_var) + 6)));
      func_002051F4(0, temp_v0);
    }
  }
}
