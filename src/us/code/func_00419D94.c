#include "common.h"
#include "m2c_macros.h"

s32 func_002759C4(s32);
s32 func_0041648C(s32, s32, s32, s32, s32, s32);

s32 func_002759B4();                                /* extern */
extern u8 D_8011AAEC[];

s32 func_00419D94(s32 arg0, s32 arg1)
{
  s32 new_var;
  s32 var_a1;
  var_a1 = D_8011AAEC[0];
  new_var = func_0041648C(arg1, (s32) var_a1, 8, 0, 0xFF, 0);
  var_a1 = new_var;
  if (var_a1 == 0xF7)
  {
    var_a1 = 0xF8;
  }
  D_8011AAEC[0] = (u8) var_a1;
  if (func_002759B4() <= 0)
  {
    func_002759C4(1);
  }
  return 0;
}
