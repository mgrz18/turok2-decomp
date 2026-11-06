#include "common.h"
#include "m2c_macros.h"
#include "structs/cinema.h"

s32 CCache__Deallocate(s32, s32);

extern Cinema *gCinemaPlayerP;

void Cinema_Reset(void)
{
  s32 temp_a1;
  temp_a1 = (s32) gCinemaPlayerP->owner;
  if (temp_a1 != 0)
  {
    CCache__Deallocate(0, temp_a1);
  }
  gCinemaPlayerP->fading = (gCinemaPlayerP->finished = (gCinemaPlayerP->active = (s32) (gCinemaPlayerP->resource = (gCinemaPlayerP->owner = 0))));
}
