#include "common.h"
#include "structs/cinema.h"

extern Cinema *D_800C1BB0;

s32 Cinema_OverridesMusic1(void)
{
  if (D_800C1BB0->active != 0)
  {
    if (D_800C1BB0)
    {
      return D_800C1BB0->spec.unk40;
    }
    else
    {
      return D_800C1BB0->spec.unk40;
    }
  }
  return 0;
}
