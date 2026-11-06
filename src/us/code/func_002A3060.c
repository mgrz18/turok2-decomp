#include "common.h"
#include "globals/libultra.h"


s32 osPiGetCmdQueue(void)
{
  s32 *new_var;
  s32 new_var2;
  if (__osPiDevMgr.active != 0)
  {
    if (new_var)
    {
      new_var = (s32 *)&__osPiDevMgr.cmdQueue;
      ;
      return *new_var;
    }
    else
    {
      new_var = (s32 *)&__osPiDevMgr.cmdQueue;
      ;
      return *new_var;
    }
  }
  return 0;
}
