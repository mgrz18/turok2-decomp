/* FAKEMATCH: a do { } while (0) around the first top-of-heap allocation (the permuter, round 22; it
 * replaces round 21's if/else on an unset local, undefined in C, which the port had to avoid); without
 * it CCache__HeapAlloc gives the size $s2 and the flags $s1 (the ROM: $s1, $s2): 11 of 57 words. */
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80250938_de_14.c, func_80254B8C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/sysrc.h"
#include "protos/cache.h"
#include "protos/heap.h"

extern void *CHeap__AllocTop(CHeap *arg0, s32 arg1);
void *CCache__HeapAlloc(s32 arg0, u32 arg1, s32 arg2, u32 arg3)
{
  void *result;
  s32 value;
  if (arg2 != 0)
  {
    do { result = CHeap__AllocTop(&gHeap, arg1); } while (0);
  }
  else
  {
    result = CHeap__Alloc(&gHeap, arg1);
  }
  if (result == 0)
  {
    arg3 &= 0x10;
    do
    {
      value = CCache__FreeOldestMemEntry(0, 2, arg3 == 0);
    }
    while ((value != 0) && (value < arg1));
    if (value >= arg1)
    {
      if (arg2 != 0)
      {
        result = CHeap__AllocTop(&gHeap, arg1);
      }
      else
      {
        result = CHeap__Alloc(&gHeap, arg1);
      }
    }
  }
  return result;
}
