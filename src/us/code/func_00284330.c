
#include "common.h"
#include "structs/cinema.h"
#include "globals/cinema.h"
s32 Cinema_OverridesMusic2(void)
{
  if (0 != gCinemaPlayerP->active)
  {
    if (gCinemaPlayerP)
    {
      return gCinemaPlayerP->spec.unk44;
    }
    else
    {
      return gCinemaPlayerP->spec.unk44;
    }
  }
  return 0;
}
