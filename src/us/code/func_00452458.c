#include "common.h"
#include "m2c_macros.h"

s32 func_00416894(s32, s32, s32);

extern M2C_UNK D_00438CE4;
extern M2C_UNK D_8011ACB0;

s32 func_00452458(s32 arg0, s32 arg1, s32 arg2)
{
  s8 *new_var;
  new_var = (s8 *) (&D_8011ACB0);
  *((s32 *) (new_var + 0x2C)) = 0;
  *((s32 *) (new_var + 0x30)) = 0;
  *((s32 *) (new_var + 0x28)) = 0;
  func_00416894(arg2, arg1, (s32) (&D_00438CE4));
  return 1;
}
