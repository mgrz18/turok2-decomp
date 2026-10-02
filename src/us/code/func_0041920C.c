#include "common.h"
#include "m2c_macros.h"

s32 func_0041648C(s32, s32, s32, s32, s32, s32);

extern M2C_UNK D_8011AAD8;

s32 func_0041920C(s32 arg0, s32 arg1)
{
  s8 *new_var;
  new_var = (s8 *) (&D_8011AAD8);
  *((s8 *) (((s8 *) (&D_8011AAD8)) + 0x2D)) = func_0041648C(arg1, (s32) (*((s8 *) (new_var + 0x2D))), 1, 0, 8, 1);
  return 0;
}
