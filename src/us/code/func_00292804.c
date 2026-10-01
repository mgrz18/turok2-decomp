#include "common.h"
#include "m2c_macros.h"

volatile unsigned int func_00292804(void *arg0, void *arg1)
{
  int new_var;
  s32 var_v1;
  new_var = 0x40;
  var_v1 = (*((u8 *) (((s8 *) (((*((u8 *) (((s8 *) arg0) + 0x31))) * 0x10) + (*((s32 *) (((s8 *) arg1) + 0x60))))) + 7))) + ((*((u8 *) (((s8 *) (*((void **) (((s8 *) arg0) + 0x20)))) + 0xC))) - new_var);
  new_var = var_v1 < 0;
  if (new_var)
  {
    var_v1 = 0;
  }
  if (var_v1 >= 0x80)
  {
    var_v1 = 0x7F;
  }
  return var_v1 & 0xFF;
}
