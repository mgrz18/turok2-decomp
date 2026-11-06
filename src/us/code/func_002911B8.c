#include "common.h"

s32 alLink(s32, s32);
s32 osSetIntMask(s32);

s32 alUnlink(void *);                      /* extern */

void alEvtqFlushType(void *arg0, s16 arg1)
{
  s32 temp_s4;
  void *temp_s1;
  s32 new_var;
  void *var_s0;
  new_var = osSetIntMask(1);
  var_s0 = *((void **) (((s8 *) arg0) + 8));
  temp_s4 = new_var;
  if (var_s0 != 0)
  {
    do
    {
      temp_s1 = *((void **) (((s8 *) var_s0) + 0));
      if ((*((s16 *) (((s8 *) var_s0) + 0xC))) == arg1)
      {
        if (temp_s1 != 0)
        {
          *((s32 *) (((s8 *) temp_s1) + 8)) = (s32) ((*((s32 *) (((s8 *) temp_s1) + 8))) + (*((s32 *) (((s8 *) var_s0) + 8))));
        }
        alUnlink(var_s0);
        alLink((s32) var_s0, (s32) arg0);
      }
      var_s0 = temp_s1;
    }
    while (var_s0 != 0);
  }
  osSetIntMask(temp_s4);
}
