/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802773D4_de_3.c, func_80278890_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern s8 D_800ACAF4[];
extern s32 D_800B6D18;
f32 func_00213E90(void *arg0)
{
  ObjectRecAtBC *volume = arg0;
  u32 temp_a0;
  s32 idx;
  temp_a0 = volume->animationSeed;
  idx = D_800B6D18 + ((((temp_a0 % 7) & 0xFF) + 1) * 0x10);
  idx = idx % 90;
  return (f32) D_800ACAF4[idx];
}
extern s32 D_800B6D18;
