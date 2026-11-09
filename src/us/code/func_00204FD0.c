#include "rw/junk_types.h" /* B12Node: a cached resource */
#include "globals/cache.h"
#include "globals/sysrc.h"
#include "protos/cache.h"
#include "protos/ultra.h"

s32 CHeap__AllocAt(CHeap *, s32, s32);
s32 __osDisableInt(void);
void __osRestoreInt(s32);
B12Node *CCache__AllocateAt(s32 arg0, s32 arg1, s32 arg2)
{
  s32 temp_a0;
  s32 temp_v0;
  s32 temp_v0_2;
  s32 temp_v1;
  s32 temp_v1_2;
  B12Node *var_s0;
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
  var_s0 = CCache__AllocMemEntry(0, 0);
  if (var_s0 != 0)
  {
    temp_v0 = CHeap__AllocAt(&gHeap, arg1, arg2);
    var_s0->key = temp_v0;
    if (temp_v0 != 0)
    {
      var_s0->unk4 = arg2;
      var_s0->flags = var_s0->flags | 3;
      CCache__AddToAddressList(0, var_s0);
    }
    else
    {
      CCache__FreeMemEntry(0, var_s0);
      var_s0 = 0;
    }
  }
  temp_v0_2 = __osDisableInt();
  temp_v1_2 = gCache_mutex_count - 1;
  gCache_mutex_count = temp_v1_2;
  if (temp_v1_2 != 0)
  {
    if (gCache_mutex_count || var_s0)
    {
      __osRestoreInt(temp_v0_2);
      osSendMesg((&gCache_mutex), 0, 1);
      return var_s0;
    }
    else
    {
      __osRestoreInt(temp_v0_2);
      osSendMesg((&gCache_mutex), 0, 1);
      return var_s0;
    }
  }
  __osRestoreInt(temp_v0_2);
  return var_s0;
}
