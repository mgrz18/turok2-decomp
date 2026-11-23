/* FAKEMATCH (round 21 audit): an if/else whose two arms are the same code, kept
 * for the ROM's register allocation or layout; with one arm: osPiGetCmdQueue: 4 of 10 words. */
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
