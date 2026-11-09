#include "common.h"
#include "m2c_macros.h"
#include "globals/game.h"
#include "protos/game.h"

void Controller_UnwatchPak(s32);
s32 Controller_ResetRumble(s32);


s32 RumbleMenu_Play_2(s32 arg0, void *arg1)
{
  void *new_var;
  gEngineApp_m_Settings.match.tagPhase = 0;
  Controller_UnwatchPak((s32) (*((s8 *) (((s8 *) (*((void **) (((s8 *) arg1) + 0x20)))) + 4))));
  Controller_ResetRumble((sptr) (*((void **) (((s8 *) arg1) + 0x20))));
 do { Controller_SetRumbleOption((sptr) (*((void **) (((s8 *) arg1) + 0x20))), 1); new_var = *((void **) (((s8 *) arg1) + 0x1C)); } while (0);
  *((s32 *) (((s8 *) new_var) + 0xBF8)) = 0;
  return 1;
}
