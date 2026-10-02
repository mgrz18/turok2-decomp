#include "common.h"
#include "m2c_macros.h"

extern M2C_UNK D_8011ACB0;

s32 func_00417E84(void)
{
  M2C_UNK *new_var;
  s32 var_a0;
  var_a0 = 0;
  new_var = &D_8011ACB0;
  if ((((*((s32 *) (((s8 *) (&D_8011ACB0)) + 0x28))) != 0) || ((*((s32 *) (((s8 *) (&D_8011ACB0)) + 0x1C))) != 0)) || ((*((s32 *) (((s8 *) new_var) + 0x20))) != 0))
  {
    var_a0 = 1;
  }
  return var_a0;
}
