#include "common.h"
#include "m2c_macros.h"
#include "structs/cinema.h"

s32 CCache__Deallocate(s32, s32);

extern Cinema *D_800C1BB0;

void Cinema_Reset(void)
{
  s32 temp_a1;
  temp_a1 = (s32) D_800C1BB0->owner;
  if (temp_a1 != 0)
  {
    CCache__Deallocate(0, temp_a1);
  }
  D_800C1BB0->fading = (D_800C1BB0->finished = (D_800C1BB0->active = (s32) (D_800C1BB0->resource = (D_800C1BB0->owner = 0))));
}
