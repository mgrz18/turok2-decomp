#include "common.h"
#include "m2c_macros.h"

s32 func_00288E58(s32, s32, s32);

extern s32 D_800B6D70[];
extern s32 D_800F5D2C[];
extern s32 D_8011AD00[];
extern f32 D_8011B11C[];

void func_00289B60(s32 arg0)
{
  int new_var;
  new_var = 0;
  if (D_800B6D70[0] != 0)
  {
    func_00288E58(arg0, 1, 1);
    return;
  }
  if ((D_800F5D2C[0] & 0x1000) && (D_8011AD00[new_var] == 0))
  {
    D_8011AD00[new_var] = 1;
    D_800F5D2C[new_var] = 0;
    D_8011B11C[new_var] = 15.0f;
  }
}
