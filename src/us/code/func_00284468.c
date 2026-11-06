
#include "common.h"
#include "structs/cinema.h"
s32 Cinema_RestorePlayer(void);
s32 Sound__StopAllExcept(s32);
extern Cinema *D_800C1BB0;
void Cinema_RunEnd(s32 arg0)
{
  CinemaCallback temp_v1;
  if (D_800C1BB0->ended == 0)
  {
    Sound__StopAllExcept(-1);
    if ((D_800C1BB0->restoreAtEnd != 0) || (arg0 != 0))
    {
      Cinema_RestorePlayer();
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
