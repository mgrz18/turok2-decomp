#include "common.h"
#include "globals/cache.h"
#include "protos/cache.h"
#include "protos/ultra.h"

s32 __osDisableInt(void);
void __osRestoreInt(s32);


s32 CCache__AllocateMemoryForDecompressionTop(s32 arg0, s32 arg1, s32 arg2)
{
  s32 temp_a0;
  sptr temp_v0;
  s32 temp_v0_2;
  s32 temp_v0_3;
  s32 temp_v1;
  s32 temp_v1_2;
  sptr var_s0;
  temp_a0 = __osDisableInt();
  temp_v1 = gCache_mutex_count + 1;
  gCache_mutex_count = temp_v1;
  if (temp_v1 != 1)
  {
    __osRestoreInt(temp_a0);
    osRecvMesg((&gCache_mutex), 0, 1);
  }
  else
  {
    __osRestoreInt(temp_a0);
  }
  var_s0 = CCache__AllocMemEntry(0, 0x22);
  if (var_s0 != 0)
  {
    *((s32 *) (((s8 *) var_s0) + 8)) = (s32) ((*((s32 *) (((s8 *) var_s0) + 8))) + 1);
    *((s32 *) (((s8 *) var_s0) + 0xC)) = (s32) ((*((s32 *) (((s8 *) var_s0) + 0xC))) | 0x100);
    temp_v0 = CCache__HeapAlloc(0, arg2, 1, 0x22);
    *((s32 *) (((s8 *) var_s0) + 0)) = temp_v0;
    if (temp_v0 != 0)
    {
      *((s32 *) (((s8 *) var_s0) + 4)) = arg2;
      *((s32 *) (((s8 *) var_s0) + 0xC)) = (s32) ((*((s32 *) (((s8 *) var_s0) + 0xC))) | 0x22);
      CCache__AddToAddressList(0, var_s0);
    }
    else
    {
      temp_v0_2 = (*((s32 *) (((s8 *) var_s0) + 8))) - 1;
      *((s32 *) (((s8 *) var_s0) + 8)) = temp_v0_2;
      if (temp_v0_2 == 0)
      {
        *((s32 *) (((s8 *) var_s0) + 0xC)) = (s32) ((*((s32 *) (((s8 *) var_s0) + 0xC))) & (~0x100));
      }
      CCache__FreeMemEntry(0, var_s0);
      var_s0 = 0;
    }
  }
  temp_v0_3 = __osDisableInt();
  temp_v1_2 = gCache_mutex_count - 1;
  gCache_mutex_count = temp_v1_2;
  if (temp_v1_2 != 0)
  {
    if (temp_v1_2 || temp_v0_2)
    {
      __osRestoreInt(temp_v0_3);
      osSendMesg((&gCache_mutex), 0, 1);
      return var_s0;
    }
    else
    {
      __osRestoreInt(temp_v0_3);
      osSendMesg((&gCache_mutex), 0, 1);
      return var_s0;
    }
  }
  __osRestoreInt(temp_v0_3);
  return var_s0;
}
