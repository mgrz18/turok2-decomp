#include "common.h"
#include "m2c_macros.h"

void func_004481D8(s32 arg0, s32 arg1, void **arg2, s32 *arg3, s32 *arg4)
{
  s32 var_t0;
  void **var_a2;
  void *temp_a0;
  var_a2 = arg2;
  var_t0 = 0;
  if (arg1 > 0)
  {
    do
    {
      temp_a0 = *var_a2;
      *arg3 += *((s32 *) (((s8 *) temp_a0) + 0x148));
      if (((*((s32 *) (((s8 *) temp_a0) + 0x144))) != 0) && ((*((s32 *) (((s8 *) temp_a0) + 0xD4))) & 0x100))
      {
        *arg4 += *((s32 *) (((s8 *) temp_a0) + 0x148));
      }
      var_t0 += 1;
      var_a2 = (typeof(var_a2)) ((s8 *) ((typeof(var_a2)) (((s8 *) var_a2) + 4)));
    }
    while (var_t0 < arg1);
  }
}
