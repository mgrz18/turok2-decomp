#include "common.h"
#include "m2c_macros.h"

void Controller_UnwatchPak(s32);
s32 Controller_ResetRumble(s32);
s32 Controller_SetRumbleOption(s32, s32);

extern s32 D_8011ACF0;

s32 RumbleMenu_Play(s32 arg0, void *arg1)
{
  D_8011ACF0 = 0;
  Controller_UnwatchPak((s32) (*((s8 *) (((s8 *) (*((void **) (((s8 *) arg1) + 0x20)))) + 4))));
  if (1)
  {
    if (arg1)
    {
      Controller_ResetRumble((s32) (*((void **) (((s8 *) arg1) + 0x20))));
      Controller_SetRumbleOption((s32) (*((void **) (((s8 *) arg1) + 0x20))), 1);
      *((s32 *) (((s8 *) (*((void **) (((s8 *) arg1) + 0x1C)))) + 0xBF8)) = 0;
    }
    else
    {
      Controller_ResetRumble((s32) (*((void **) (((s8 *) arg1) + 0x20))));
      Controller_SetRumbleOption((s32) (*((void **) (((s8 *) arg1) + 0x20))), 1);
      *((s32 *) (((s8 *) (*((void **) (((s8 *) arg1) + 0x1C)))) + 0xBF8)) = 0;
    }
  }
  return 1;
}
