/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80254D44_de.c, func_80254D44_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/cache.h"

extern void CCache__ClearDisplayListLocks(s32, s32);
extern s32 gCache;
extern s32 gCache_dlistCount[];
void CCache__ClearDisplayListLocks(s32 arg0, s32 arg1)
{
  s32 mask;
  u32 count;
  int new_var;
  u32 new_var2;
  s32 offset;
  u32 limit;
  mask = -0x201;
  if (arg1 != 0)
  {
    mask = -0x401;
  }
 do { count = 0; limit = gCacheEntryCount; } while (0);
  new_var2 = limit;
  if (limit != 0)
  {
    offset = count;
    do
    {
      new_var = offset + gCache;
      count += 1;
      *((s32 *) (new_var + 0xC)) &= mask;
      offset += 0x28;
    }
    while (count < new_var2);
  }
  gCache_dlistCount[arg1] = 0;
}
