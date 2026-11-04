#include "common.h"

extern s32 D_800B40B0;
extern s32 D_800B40C8;
extern s32 D_8011AB18;

s32 func_0041C718(void *arg0, void *arg1)
{
  s32 *new_var2;
  s32 *var_v1;
  s8 *new_var;
  void *temp_v0;
  temp_v0 = *((void **) (((s8 *) arg1) + 0x1C));
  new_var2 = &D_800B40B0;
  var_v1 = &D_8011AB18;
  if (temp_v0 != 0)
  {
    var_v1 = *((s32 **) (((s8 *) temp_v0) + 0x518));
  }
  new_var = ((s8 *) var_v1) + 0x31;
  if ((*((u8 *) new_var)) == 1)
  {
    *((s32 **) (((s8 *) arg0) + 0x14)) = new_var2;
  }
  else
  {
    *((s32 **) (((s8 *) arg0) + 0x14)) = &D_800B40C8;
  }
  return 0;
}
