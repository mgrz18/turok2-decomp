#include "common.h"
#include "m2c_macros.h"

s32 Cinema_PathPlaying();                                /* extern */
extern void *D_800C1BB0;

s32 func_002842F8(void)
{
  int new_var2;
  s8 *new_var;
  if (Cinema_PathPlaying() == 0)
  {
    return 0;
  }
  new_var2 = 0;
  new_var = (s8 *) D_800C1BB0;
  return ((*((s32 *) (new_var + 0x74))) & 2) != new_var2;
}
