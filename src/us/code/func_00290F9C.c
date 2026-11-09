#include "common.h"

s32 alLink(s32, s32);
s32 osSetIntMask(s32);

s32 alCopy(s32, s16 *, s32);         /* extern */
s32 alUnlink(s32);                         /* extern */

s32 alEvtqNextEvent(void *arg0, s16 *arg1)
{
  sptr temp_s0;
  s32 temp_s2;
  s32 var_s0;
  unsigned int new_var;
  new_var = osSetIntMask(1);
  temp_s0 = *((s32 *) (((s8 *) arg0) + 8));
  temp_s2 = new_var;
  if (temp_s0 != 0)
  {
    alUnlink(temp_s0);
    alCopy(temp_s0 + 0xC, arg1, 0x10);
    alLink(temp_s0, (sptr) arg0);
    var_s0 = *((s32 *) (((s8 *) temp_s0) + 8));
  }
  else
  {
    *arg1 = -1;
    var_s0 = 0;
  }
  osSetIntMask(temp_s2);
  return var_s0;
}
