#include "common.h"
#include "m2c_macros.h"

extern M2C_UNK D_800B40B0;
extern M2C_UNK D_800B40C8;
extern M2C_UNK D_8011AB18;

s32 func_0041C718(void *arg0, void *arg1)
{
  M2C_UNK *new_var2;
  M2C_UNK *var_v1;
  s8 *new_var;
  void *temp_v0;
  temp_v0 = *((void **) (((s8 *) arg1) + 0x1C));
  new_var2 = &D_800B40B0;
  var_v1 = &D_8011AB18;
  if (temp_v0 != 0)
  {
    var_v1 = *((M2C_UNK **) (((s8 *) temp_v0) + 0x518));
  }
  new_var = ((s8 *) var_v1) + 0x31;
  if ((*((u8 *) new_var)) == 1)
  {
    *((M2C_UNK **) (((s8 *) arg0) + 0x14)) = new_var2;
  }
  else
  {
    *((M2C_UNK **) (((s8 *) arg0) + 0x14)) = &D_800B40C8;
  }
  return 0;
}
