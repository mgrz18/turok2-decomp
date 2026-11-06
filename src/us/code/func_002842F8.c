#include "common.h"
#include "m2c_macros.h"
#include "structs/cinema.h"

s32 Cinema_PathPlaying();                                /* extern */
extern Cinema *D_800C1BB0;

s32 Cinema_FollowsPlayer(void)
{
  int new_var2;
  Cinema *new_var;
  if (Cinema_PathPlaying() == 0)
  {
    return 0;
  }
  new_var2 = 0;
  new_var = D_800C1BB0;
  return (new_var->spec.flags & 2) != new_var2;
}
