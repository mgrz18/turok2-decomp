#include "common.h"
#include "m2c_macros.h"
#include "structs/cinema.h"
#include "globals/cinema.h"
#include "protos/cache.h"



void Cinema_Reset(void)
{
  sptr temp_a1;
  temp_a1 = (sptr) gCinemaPlayerP->owner;
  if (temp_a1 != 0)
  {
    CCache__Deallocate(0, temp_a1);
  }
  gCinemaPlayerP->fading = (gCinemaPlayerP->finished = (gCinemaPlayerP->active = (sptr) (gCinemaPlayerP->resource = (gCinemaPlayerP->owner = 0))));
}
