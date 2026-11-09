#include "globals/cache.h"
#include "protos/cache.h"
#include "protos/ultra.h"

s32 __osDisableInt(void);
void __osRestoreInt(s32);
#ifndef TARGET_PORT
sptr CCache__AllocateMemoryForDecompression(s32 arg0, void *arg1, s32 arg2)
{
  s32 temp_a0;
  s32 temp_s1;
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
  temp_s1 = *((s32 *) (((s8 *) arg1) + 0x1C));
  var_s0 = CCache__AllocMemEntry(0, temp_s1);
  if (var_s0 != 0)
  {
    *((s32 *) (((s8 *) var_s0) + 8)) = (s32) ((*((s32 *) (((s8 *) var_s0) + 8))) + 1);
    *((s32 *) (((s8 *) var_s0) + 0xC)) = (s32) ((*((s32 *) (((s8 *) var_s0) + 0xC))) | 0x100);
    temp_v0 = CCache__HeapAlloc(0, arg2, (((u32) temp_s1) >> 5) & 1, temp_s1);
    *((s32 *) (((s8 *) var_s0) + 0)) = temp_v0;
    if (temp_v0 != 0)
    {
      *((s32 *) (((s8 *) var_s0) + 4)) = arg2;
      *((s32 *) (((s8 *) var_s0) + 0xC)) = (s32) ((*((s32 *) (((s8 *) var_s0) + 0xC))) | temp_s1);
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
    if (temp_v1 || gCache_mutex_count)
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
#endif

#ifdef TARGET_PORT
#include "structs/cache.h"

/* The host form of the function above, typed (its m2c body writes CMemEntry fields at N64
 * offsets and returns the entry as s32). ProPackDecompress (func_00200B00.c) calls it for the
 * block a load unit is copied into: a fresh entry with the request's memFlags, locked, `size`
 * bytes from the heap (from the top when memFlags has 0x20), on the address list. */
CMemEntry *CCache__AllocDecompressFlags(u32 flags, u32 size) {
    CMemEntry *node;
    s32 token = __osDisableInt();
    if (++gCache_mutex_count != 1) {
        __osRestoreInt(token);
        osRecvMesg(&gCache_mutex.queue, 0, 1);
    } else {
        __osRestoreInt(token);
    }
    node = CCache__AllocMemEntry(0, flags);
    if (node != NULL) {
        node->lockCount++;
        node->flags |= 0x100;
        node->data = (void *)CCache__HeapAlloc(0, size, (flags >> 5) & 1, flags);
        if (node->data != NULL) {
            node->size = size;
            node->flags |= flags;
            CCache__AddToAddressList(0, node);
        } else {
            if (--node->lockCount == 0) {
                node->flags &= ~0x100;
            }
            CCache__FreeMemEntry(0, node);
            node = NULL;
        }
    }
    token = __osDisableInt();
    if (--gCache_mutex_count != 0) {
        __osRestoreInt(token);
        osSendMesg(&gCache_mutex.queue, 0, 1);
    } else {
        __osRestoreInt(token);
    }
    return node;
}

CMemEntry *CCache__AllocDecompressBlock(CCacheEntry *request, u32 size) {
    return CCache__AllocDecompressFlags(request->memFlags, size);
}

/* The game's entry point (protos/cache.h still returns the entry as s32: 64BIT.md part 2). */
sptr CCache__AllocateMemoryForDecompression(s32 arg0, void *arg1, s32 arg2) {
    return (sptr)CCache__AllocDecompressBlock((CCacheEntry *)arg1, (u32)arg2);
}
#endif
