/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8029F888_de_2.c, func_802A03F4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern s32 func_00426FFC(u8 *arg0, u8 *arg1);
s32 func_00426FFC(u8 *arg0, u8 *arg1)
{
  s32 c2;
  int new_var;
  s32 c1;
  c2 = *arg1;
  arg1 += 1;
  new_var = 0x41;
  if (c2 >= new_var)
  {
    if (c2 < 0x5B)
    {
      c2 += 0x20;
    }
  }
  c1 = *arg0;
  arg0 += 1;
  if (c1 >= new_var)
  {
    if (c1 < 0x5B)
    {
      c1 += 0x20;
    }
  }
  if (c2 != 0)
  {
    if (c2 == c1)
    {
      return func_00426FFC(arg0, arg1);
    }
  }
  return c1 - c2;
}
