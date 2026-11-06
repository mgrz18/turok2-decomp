/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80250938_de_14.c, func_80254B8C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern s32 CCache__FreeOldestMemEntry(s32 unused0, s32 unused1, s32 keep_recent);
extern void *CCache__HeapAlloc(s32 arg0, u32 arg1, s32 arg2, u32 arg3);
extern char gHeap;
extern void *CHeap__AllocTop(void *arg0, s32 arg1);
extern void *CHeap__Alloc(void *, s32);
void *CCache__HeapAlloc(s32 arg0, u32 arg1, s32 arg2, u32 arg3)
{
  void *result;
  s32 value;
  if (arg2 != 0)
  {
    result = CHeap__AllocTop(&gHeap, arg1);
  }
  else
    if (value)
  {
    result = CHeap__Alloc(&gHeap, arg1);
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
