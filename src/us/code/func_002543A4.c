#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_00254430(void *, void *);              /* extern */
M2C_UNK func_00254584(void *, void *);              /* extern */

void func_002543A4(void *arg0, void *arg1, void *arg2)
{
  int new_var;
  s32 temp_s0;
  s32 temp_s2;
  s32 temp_v1;
  if (arg2 != 0)
  {
    temp_v1 = *((s32 *) (((s8 *) arg1) + 0x140));
    temp_s2 = (*((u16 *) (0x52 + ((s8 *) arg2)))) & 2;
    new_var = temp_v1 & 0x100;
    temp_s0 = temp_v1 & 0x80;
    if (temp_s0 == 0)
    {
      *((s32 *) (((s8 *) arg0) + 0xBE4)) = 0;
    }
    if ((!new_var) && (temp_s2 != 0))
    {
      func_00254584(arg0, arg1);
    }
    if ((temp_s0 != 0) && (temp_s2 == 0))
    {
      func_00254430(arg0, arg1);
    }
  }
}
