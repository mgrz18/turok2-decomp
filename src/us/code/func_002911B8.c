#include "common.h"
#include "m2c_macros.h"

s32 func_002933E0(s32, s32);
s32 func_0029E230(s32);

M2C_UNK func_002933B0(void *);                      /* extern */

void func_002911B8(void *arg0, s16 arg1)
{
  s32 temp_s4;
  void *temp_s1;
  s32 new_var;
  void *var_s0;
  new_var = func_0029E230(1);
  var_s0 = *((void **) (((s8 *) arg0) + 8));
  temp_s4 = new_var;
  if (var_s0 != 0)
  {
    do
    {
      temp_s1 = *((void **) (((s8 *) var_s0) + 0));
      if ((*((s16 *) (((s8 *) var_s0) + 0xC))) == arg1)
      {
        if (temp_s1 != 0)
        {
          *((s32 *) (((s8 *) temp_s1) + 8)) = (s32) ((*((s32 *) (((s8 *) temp_s1) + 8))) + (*((s32 *) (((s8 *) var_s0) + 8))));
        }
        func_002933B0(var_s0);
        func_002933E0((s32) var_s0, (s32) arg0);
      }
      var_s0 = temp_s1;
    }
    while (var_s0 != 0);
  }
  func_0029E230(temp_s4);
}
