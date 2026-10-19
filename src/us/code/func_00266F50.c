/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80265350_de.c, func_80265714_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern float func_002974C0(float x);
extern float D_800A7D1C;
extern float D_800A7D18;
extern float D_800A7D20;
extern f32 func_00266F50(f32 arg0);
f32 func_00266F50(f32 arg0)
{
  f32 var_f1;
  f32 var_f0;
  var_f0 = arg0;
  if (var_f0 < 0.0f)
  {
    var_f1 = 0.0f;
    var_f0 = var_f1;
  }
  else
  {
    var_f1 = D_800A7D18;
    if (var_f1 < var_f0)
    {
      var_f0 = var_f1;
    }
  }
  return (D_800A7D20 - func_002974C0(var_f0 * D_800A7D1C)) * (((struct func_802077F4_S2 *) ((char *) (&D_800A7D20)))->unk4);
}
