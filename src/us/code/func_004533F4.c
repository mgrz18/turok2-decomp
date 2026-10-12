#include "common.h"
#include "m2c_macros.h"

extern f32 D_800B6D28;
extern M2C_UNK D_8011ACB0;

s32 func_004533F4(void)
{
  f32 temp_f0;
  M2C_UNK *new_var;
  new_var = &D_8011ACB0;
  temp_f0 = (*((f32 *) (((s8 *) new_var) + 0x10))) - D_800B6D28;
  *((f32 *) (((s8 *) new_var) + 0x10)) = temp_f0;
  if (temp_f0 < 0.0f)
  {
    *((f32 *) (((s8 *) new_var) + 0x10)) = 0.0f;
  }
  return 0;
}
