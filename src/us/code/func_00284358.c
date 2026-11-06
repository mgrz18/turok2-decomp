#include "common.h"
#include "structs/cinema.h"

extern Cinema *gCinemaPlayerP;

s32 Cinema_OverridesMusic1(void)
{
  if (gCinemaPlayerP->active != 0)
  {
    if (gCinemaPlayerP)
    {
      return gCinemaPlayerP->spec.unk40;
    }
    else
    {
      return gCinemaPlayerP->spec.unk40;
    }
  }
  return 0;
}
