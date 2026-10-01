#include "common.h"
#include "m2c_macros.h"

s32 func_00219F74(s32, s32, s32);
s32 func_0026D5E4(s32, s32, s32);
s32 func_0026D628(s32);

M2C_UNK func_0026D5DC(void *);                   /* extern */

s16 func_0021EA3C(s32 arg0, s16 *arg1, s32 arg2)
{
  s32 new_var2;
  u8 sp10[0x68];
  s16 *var_s0;
  int new_var;
  u16 var_a1;
  func_0026D5DC(sp10);
  var_s0 = arg1;
  var_a1 = (u16) (*var_s0);
  if ((*var_s0) != (-1))
  {
    do
    {
      if (func_00219F74(arg0, (s32) ((s16) var_a1), arg2) != (-1))
      {
        new_var = 2;
        new_var2 = (s32) (*((s16 *) (((s8 *) var_s0) + 0)));
        func_0026D5E4((s32) (sp10), new_var2, (s32) (*((s16 *) (((s8 *) var_s0) + new_var))));
      }
      var_s0 = (typeof(var_s0)) ((s8 *) ((typeof(var_s0)) (((s8 *) var_s0) + 4)));
      var_a1 = (u16) (*var_s0);
    }
    while ((*var_s0) != (-1));
  }
  return func_0026D628((s32) (sp10));
}
