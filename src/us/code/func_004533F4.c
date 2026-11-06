#include "common.h"
#include "structs/game.h"
#include "globals/engine.h"


s32 GameOverMenu_TickCountdown_2(void)
{
  f32 countdown;
  GameMatch *match;
  match = &gEngineApp_m_Settings_match;
  countdown = match->countdown - gFrameIncrement;
  match->countdown = countdown;
  if (countdown < 0.0f)
  {
    match->countdown = 0.0f;
  }
  return 0;
}
