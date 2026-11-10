#include "common.h"
#include "m2c_macros.h"

sptr Video_GetResolutionText(void);

s32 GetMemSize();                                /* extern */

s32 func_0041A1D8(void *arg0)
{
  s32 var_v0;
  int new_var;
  if (GetMemSize() != 0x400000)
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
  *((s32 *) (((s8 *) arg0) + 0x14)) = Video_GetResolutionText();
  return 0;
}
