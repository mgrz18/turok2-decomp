#include "common.h"
#include "m2c_macros.h"
#include "protos/iset.h"
#include "protos/scene.h"


void CScene__SetLevelBit(void *arg0, s32 arg1, s32 arg2)
{
  s32 temp_a0;
  s32 new_var;
  s32 temp_a1;
  s32 temp_s0;
  s32 temp_v0;
  s32 var_a1;
  s32 var_v1;
  u8 *temp_v0_2;
  u8 *temp_v1;
  temp_s0 = *((s32 *) (((s8 *) arg0) + 0x18FA8));
  temp_v0 = CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(*((s32 *) (((s8 *) arg0) + 0x70)), 0), temp_s0), 0);
  CISet__GetBlock(temp_v0, 0);
  CISet__GetBlockSize(temp_v0, 1);
  new_var = CISet__GetBlock(temp_v0, 1);
  temp_a1 = new_var;
  temp_a0 = 1 << (arg1 & 7);
  if (arg2 != 0)
  {
    var_v1 = arg1;
    if (arg1 < 0)
    {
      var_v1 = arg1 + 7;
    }
    temp_v1 = temp_a1 + (var_v1 >> 3);
    *temp_v1 |= temp_a0;
    return;
  }
  var_a1 = arg1;
  if (var_a1 < 0)
  {
    var_a1 += 7;
  }
  temp_v0_2 = temp_a1 + (var_a1 >> 3);
  *temp_v0_2 &= ~temp_a0;
}
