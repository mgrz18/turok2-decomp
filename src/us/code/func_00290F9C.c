#include "common.h"
#include "m2c_macros.h"

s32 func_002933E0(s32, s32);
s32 func_0029E230(s32);

M2C_UNK func_00291270(s32, s16 *, M2C_UNK);         /* extern */
M2C_UNK func_002933B0(s32);                         /* extern */

s32 func_00290F9C(void *arg0, s16 *arg1)
{
  s32 temp_s0;
  s32 temp_s2;
  s32 var_s0;
  unsigned int new_var;
  new_var = func_0029E230(1);
  temp_s0 = *((s32 *) (((s8 *) arg0) + 8));
  temp_s2 = new_var;
  if (temp_s0 != 0)
  {
    func_002933B0(temp_s0);
    func_00291270(temp_s0 + 0xC, arg1, 0x10);
    func_002933E0(temp_s0, (s32) arg0);
    var_s0 = *((s32 *) (((s8 *) temp_s0) + 8));
  }
  else
  {
    *arg1 = -1;
    var_s0 = 0;
  }
  func_0029E230(temp_s2);
  return var_s0;
}
