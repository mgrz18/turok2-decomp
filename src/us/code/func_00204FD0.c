#ifdef TARGET_PORT
/* The port: the same through CMemEntry (structs/cache.h). B12Node is the N64 layout (key = data,
 * unk4 = size, flags at 0xC); on the host `data` is 8 bytes, so its flags slot is lockCount. The
 * address is sptr as protos/cache.h declares it (64BIT.md part 2). */
#include "common.h"
#include "structs/cache.h"
#include "globals/cache.h"
#include "globals/sysrc.h"
#include "protos/cache.h"
#include "protos/ultra.h"

sptr CHeap__AllocAt(CHeap *, sptr, s32);
s32 __osDisableInt(void);
void __osRestoreInt(s32);
struct B12Node *CCache__AllocateAt(s32 arg0, sptr arg1, s32 arg2) {
    s32 mask;
    CMemEntry *entry;

    mask = __osDisableInt();
    if (++gCache_mutex_count != 1) {
        __osRestoreInt(mask);
        osRecvMesg((OSMesgQueue *)&gCache_mutex, 0, 1);
    } else {
        __osRestoreInt(mask);
    }
    entry = CCache__AllocMemEntry(0, 0);
    if (entry != NULL) {
        entry->data = (void *)(uptr)CHeap__AllocAt(&gHeap, arg1, arg2);
        if (entry->data != NULL) {
            entry->size = arg2;
            entry->flags |= 3;
            CCache__AddToAddressList(0, entry);
        } else {
            CCache__FreeMemEntry(0, entry);
            entry = NULL;
        }
    }
    mask = __osDisableInt();
    if (--gCache_mutex_count != 0) {
        __osRestoreInt(mask);
        osSendMesg((OSMesgQueue *)&gCache_mutex, 0, 1);
        return (struct B12Node *)entry;
    }
    __osRestoreInt(mask);
    return (struct B12Node *)entry;
}
#else
#include "rw/junk_types.h" /* B12Node: a cached resource */
#include "globals/cache.h"
#include "globals/sysrc.h"
#include "protos/cache.h"
#include "protos/ultra.h"

sptr CHeap__AllocAt(CHeap *, sptr, s32);
s32 __osDisableInt(void);
void __osRestoreInt(s32);
B12Node *CCache__AllocateAt(s32 arg0, sptr arg1, s32 arg2)
{
  s32 temp_a0;
  sptr temp_v0;
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
#endif
