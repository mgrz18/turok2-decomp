/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802BB6C0_de_21.c, func_802BBC20_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/libultra.h"
#include "protos/ultra.h"

extern u32 __osDisableInt(void);
extern void __osRestoreInt(u32 mask);
s32 __osEnqueueAndYield(s32 *, s32);
void osYieldThread(void)
{
  void *temp_s0;
  s32 *new_var2;
  s8 *new_var;
  int new_var3;
  temp_s0 = __osDisableInt();
  ((OSThread *)(new_var = (s8 *) __osRunningThread))->state = (new_var3 = 2);
  new_var2 = &__osRunQueue;
  __osEnqueueAndYield(new_var2, new_var3);
  __osRestoreInt(temp_s0);
}
