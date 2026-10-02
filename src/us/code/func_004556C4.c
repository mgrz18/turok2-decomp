#include "common.h"
#include "m2c_macros.h"

extern M2C_UNK D_8011AB18;

s32 func_004556C4(s32 arg0, void *arg1)
{
  M2C_UNK *var_v1;
  void *temp_v0;
  temp_v0 = *((void **) (((s8 *) arg1) + 0x1C));
  var_v1 = &D_8011AB18;
  if (temp_v0 != 0)
  {
    var_v1 = *((M2C_UNK **) (((s8 *) temp_v0) + 0x518));
  }
  if (*((u8 *) (((s8 *) var_v1) + 0x31)) == 1)
  {
    *((u8 *) (((s8 *) var_v1) + 0x31)) = 0;
  }
  else
  {
    *((u8 *) (((s8 *) var_v1) + 0x31)) = 1;
  }
  return 0;
}
