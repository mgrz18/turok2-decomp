/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80250938_de_14.c, func_80254B8C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/sysrc.h"
#include "protos/cache.h"

extern void *CHeap__AllocTop(CHeap *arg0, s32 arg1);
extern void *CHeap__Alloc(CHeap *, s32);
void *CCache__HeapAlloc(s32 arg0, u32 arg1, s32 arg2, u32 arg3)
{
  void *result;
  s32 value;
  if (arg2 != 0)
  {
    result = CHeap__AllocTop(&gHeap, arg1);
  }
#ifdef TARGET_PORT
  /* The N64 tests `value` before it is set, and both branches make the same call. In C a branch
   * on an unset local is undefined: clang -O2 (no -ftrivial-auto-var-init) took this path as
   * unreachable and always allocated from the top of the heap (round 15 d5: the Blind One's
   * fight diverged at frame 138,436 of the whole-game run). */
  else
  {
    result = CHeap__Alloc(&gHeap, arg1);
  }
#else
  else
    if (value)
  {
    result = CHeap__Alloc(&gHeap, arg1);
  }
  else
  {
    result = CHeap__Alloc(&gHeap, arg1);
  }
#endif
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
