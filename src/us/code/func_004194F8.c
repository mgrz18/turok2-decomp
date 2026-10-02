#include "common.h"
#include "m2c_macros.h"

s32 func_00416894(s32, s32, s32);

extern M2C_UNK D_00438DC4;
extern M2C_UNK D_8011ACB0;

s32 func_004194F8(s32 arg0, s32 arg1, s32 arg2)
{
  M2C_UNK *new_var;
  new_var = &D_8011ACB0;
  *((s32 *) (((s8 *) (&D_8011ACB0)) + 0x2C)) = 0;
  *((s32 *) (((s8 *) new_var) + 0x30)) = 0;
  *((s32 *) (((s8 *) new_var) + 0x28)) = 0;
  func_00416894(arg2, arg1, (s32) (&D_00438DC4));
  return 1;
}
