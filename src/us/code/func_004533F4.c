#include "common.h"
#include "structs/game.h"

extern f32 D_800B6D28;

s32 GameOverMenu_TickCountdown_2(void)
{
  f32 countdown;
  GameMatch *match;
  match = &D_8011ACB0;
  countdown = match->countdown - D_800B6D28;
  match->countdown = countdown;
  if (countdown < 0.0f)
  {
    match->countdown = 0.0f;
  }
  return 0;
}
