
#include "common.h"
#include "structs/cinema.h"
s32 func_00283A4C(void);
s32 func_00275904(s32);
extern Cinema *D_800C1BB0;
void func_00284468(s32 arg0)
{
  CinemaCallback temp_v1;
  if (D_800C1BB0->ended == 0)
  {
    func_00275904(-1);
    if ((D_800C1BB0->restoreAtEnd != 0) || (arg0 != 0))
    {
      func_00283A4C();
    }
    temp_v1 = D_800C1BB0->onEnd;
    if (temp_v1 != 0)
    {
      if (D_800C1BB0)
      {
        D_800C1BB0->onEnd = 0;
        temp_v1();
      }
      else
      {
        D_800C1BB0->onEnd = 0;
        temp_v1();
      }
    }
    D_800C1BB0->ended = 1;
  }
}
