/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802609AC_de_3.c, func_80261238_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern s32 Bits_Read(s32 arg0, s32 arg1);
void Anim_DecodeRawChannel(s32 arg0, void *arg1, s32 arg2)
{
  int new_var;
  s32 sp10;
  s32 var_s0;
  u8 *var_s1;
  new_var = 0;
  var_s0 = new_var;
  if (arg2 > new_var)
  {
    var_s1 = (u8 *) arg1;
    do
    {
      var_s0 += 1;
      sp10 = Bits_Read(arg0, 0x20);
      *((f32 *) var_s1) = *((f32 *) (&sp10));
      var_s1 += 0x10;
    }
    while (var_s0 < arg2);
  }
}
