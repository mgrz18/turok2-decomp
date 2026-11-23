/* FAKEMATCH (round 21 audit): an if/else whose two arms are the same code, kept
 * for the ROM's register allocation or layout; with one arm: Cinema_RunEnd: 7 of 34 words, as the plain
 * form. */

#include "common.h"
#include "structs/cinema.h"
#include "globals/cinema.h"
s32 Cinema_RestorePlayer(void);
s32 Sound__StopAllExcept(s32);
void Cinema_RunEnd(s32 arg0)
{
  CinemaCallback temp_v1;
  if (gCinemaPlayerP->ended == 0)
  {
    Sound__StopAllExcept(-1);
    if ((gCinemaPlayerP->restoreAtEnd != 0) || (arg0 != 0))
    {
      Cinema_RestorePlayer();
    }
    temp_v1 = gCinemaPlayerP->onEnd;
    if (temp_v1 != 0)
    {
      if (gCinemaPlayerP)
      {
        gCinemaPlayerP->onEnd = 0;
        temp_v1();
      }
      else
      {
        gCinemaPlayerP->onEnd = 0;
        temp_v1();
      }
    }
    gCinemaPlayerP->ended = 1;
  }
}
