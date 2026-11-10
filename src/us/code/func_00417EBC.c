#include "common.h"
#include "m2c_macros.h"
#include "globals/game.h"
#include "protos/game.h"

void Controller_UnwatchPak(sptr);
s32 Controller_ResetRumble(sptr);


s32 RumbleMenu_Play(s32 arg0, void *arg1)
{
  gEngineApp_m_Settings.match.tagPhase = 0;
  Controller_UnwatchPak((s32) (*((s8 *) (((s8 *) (*((void **) (((s8 *) arg1) + 0x20)))) + 4))));
  if (1)
  {
    if (arg1)
    {
      Controller_ResetRumble((sptr) (*((void **) (((s8 *) arg1) + 0x20))));
      Controller_SetRumbleOption((sptr) (*((void **) (((s8 *) arg1) + 0x20))), 1);
      *((s32 *) (((s8 *) (*((void **) (((s8 *) arg1) + 0x1C)))) + 0xBF8)) = 0;
    }
    else
    {
      Controller_ResetRumble((sptr) (*((void **) (((s8 *) arg1) + 0x20))));
      Controller_SetRumbleOption((sptr) (*((void **) (((s8 *) arg1) + 0x20))), 1);
      *((s32 *) (((s8 *) (*((void **) (((s8 *) arg1) + 0x1C)))) + 0xBF8)) = 0;
    }
  }
  return 1;
}
