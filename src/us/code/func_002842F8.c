#include "common.h"
#include "m2c_macros.h"
#include "structs/cinema.h"
#include "globals/cinema.h"

s32 Cinema_PathPlaying();                                /* extern */

s32 Cinema_FollowsPlayer(void)
{
  int new_var2;
  Cinema *new_var;
  if (Cinema_PathPlaying() == 0)
  {
    return 0;
  }
  new_var2 = 0;
  new_var = gCinemaPlayerP;
  return (new_var->spec.flags & 2) != new_var2;
}
