/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80250938_de_14.c, func_80254B8C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern s32 func_00203068(s32 unused0, s32 unused1, s32 keep_recent);
extern void *func_002063B4(s32 arg0, u32 arg1, s32 arg2, u32 arg3);
extern char D_800D1CC0;
extern void *func_002025F4(void *arg0, s32 arg1);
extern void *func_002024E0(void *, s32);
void *func_002063B4(s32 arg0, u32 arg1, s32 arg2, u32 arg3)
{
  void *result;
  s32 value;
  if (arg2 != 0)
  {
    result = func_002025F4(&D_800D1CC0, arg1);
  }
  else
    if (value)
  {
    result = func_002024E0(&D_800D1CC0, arg1);
  }
  else
  {
    result = func_002024E0(&D_800D1CC0, arg1);
  }
  if (result == 0)
  {
    arg3 &= 0x10;
    do
    {
      value = func_00203068(0, 2, arg3 == 0);
    }
    while ((value != 0) && (value < arg1));
    if (value >= arg1)
    {
      if (arg2 != 0)
      {
        result = func_002025F4(&D_800D1CC0, arg1);
      }
      else
      {
        result = func_002024E0(&D_800D1CC0, arg1);
      }
    }
  }
  return result;
}
