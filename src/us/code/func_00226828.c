#include "common.h"
#include "m2c_macros.h"
#include "protos/iset.h"


void CScene__SetSavedBit(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5)
{
  s32 temp_a0;
  s32 new_var;
  s32 temp_a1;
  s32 temp_v0;
  s32 var_a0;
  s32 var_v1;
  u8 *temp_v0_2;
  u8 *temp_v1;
  if (arg1 != 0)
  {
    var_a0 = *((s32 *) (((s8 *) arg0) + 0x6C));
  }
  else
  {
    var_a0 = *((s32 *) (((s8 *) arg0) + 0x70));
  }
  temp_v0 = CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(var_a0, 0), arg3), arg2);
  CISet__GetBlock(temp_v0, 0);
  CISet__GetBlockSize(temp_v0, 1);
  new_var = CISet__GetBlock(temp_v0, 1);
  temp_a1 = new_var;
  temp_a0 = 1 << (arg4 & 7);
  if (arg5 != 0)
  {
    var_v1 = arg4;
    if (arg4 < 0)
    {
      var_v1 = arg4 + 7;
    }
    temp_v1 = temp_a1 + (var_v1 >> 3);
    *temp_v1 |= temp_a0;
    return;
  }
  temp_v0_2 = temp_a1 + (arg4 / 8);
  *temp_v0_2 &= ~temp_a0;
}
