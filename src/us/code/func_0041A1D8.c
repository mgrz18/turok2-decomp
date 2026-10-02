#include "common.h"
#include "m2c_macros.h"

s32 func_004245E0(void);

s32 func_00266B80();                                /* extern */

s32 func_0041A1D8(void *arg0)
{
  s32 var_v0;
  int new_var;
  if (func_00266B80() != 0x400000)
  {
    new_var = 0x800000;
    var_v0 = (*((s32 *) (((s8 *) arg0) + 8))) | new_var;
  }
  else
  {
    var_v0 = (*((s32 *) (((s8 *) arg0) + 8))) & 0xFF7FFFFF;
  }
  new_var = var_v0;
  *((s32 *) (((s8 *) arg0) + 8)) = new_var;
  *((s32 *) (((s8 *) arg0) + 0x14)) = func_004245E0();
  return 0;
}
