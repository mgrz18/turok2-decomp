/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802BB6C0_de_21.c, func_802BBC20_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern u32 func_0029DFF0(void);
extern void func_0029E010(u32 mask);
extern void func_0029DF70();
s32 func_0029C6DC(s32 *, s32);
extern s32 D_800B8918;
extern void *D_800B8920;
void func_0029DF70(void)
{
  void *temp_s0;
  s32 *new_var2;
  s8 *new_var;
  int new_var3;
  temp_s0 = func_0029DFF0();
  ((OSThread_s *)(new_var = (s8 *) D_800B8920))->state = (new_var3 = 2);
  new_var2 = &D_800B8918;
  func_0029C6DC(new_var2, new_var3);
  func_0029E010(temp_s0);
}
