#include "common.h"
#include "m2c_macros.h"

extern M2C_UNK D_8011ACB0;

s32 func_004559D8(void)
{
  s8 *new_var;
  s32 var_v1;
  new_var = (s8 *) (&D_8011ACB0);
  var_v1 = 0;
  if (((*((s32 *) (new_var + 0x1C))) != 0) || ((*((s32 *) (new_var + 0x20))) != 0))
  {
    var_v1 = 1;
  }
  return var_v1;
}
