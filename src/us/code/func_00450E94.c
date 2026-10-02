#include "common.h"
#include "m2c_macros.h"

void func_00285A80(s32);
s32 func_00285AE0(s32);
s32 func_00285BD0(s32, s32);

extern s32 D_8011ACF0;

s32 func_00450E94(s32 arg0, void *arg1)
{
  void *new_var;
  D_8011ACF0 = 0;
  func_00285A80((s32) (*((s8 *) (((s8 *) (*((void **) (((s8 *) arg1) + 0x20)))) + 4))));
  func_00285AE0((s32) (*((void **) (((s8 *) arg1) + 0x20))));
 do { func_00285BD0((s32) (*((void **) (((s8 *) arg1) + 0x20))), 1); new_var = *((void **) (((s8 *) arg1) + 0x1C)); } while (0);
  *((s32 *) (((s8 *) new_var) + 0xBF8)) = 0;
  return 1;
}
