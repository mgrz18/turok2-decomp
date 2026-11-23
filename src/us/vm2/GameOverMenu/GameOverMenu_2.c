/* WRONG CUT: the real function is GameOverMenu_Init_2, size 0x2C: splat's GameOverMenu_Init_2 starts with a stray
 * never-executed word (c.lt.s) ahead of the 16-aligned real start. */
#include "common.h"
#include "structs/game.h"
#include "globals/game.h"

typedef struct Layer {
    /* 0x00 */ u8 pad0[0x30];
    /* 0x30 */ s32 flags;
} Layer;
typedef struct Screen {
    /* 0x0 */ u8 pad0[0xC];
    /* 0xC */ Layer *layer;
} Screen;

void GameOverMenu_Init_2(Screen *screen) {
    gEngineApp_m_Settings.match.countdown = 120.0f;
    screen->layer->flags &= ~0x4000000;
}

#include "structs/vector.h"
#include "ultra_types.h"
#include "gfx.h"
#include "structs/list.h"

s32 GameOverMenu_Poll_2(void);
extern void CEngineApp__QuitToLegal_2(void *);

s32 GameOverMenu_Poll_2(void) {
    if (gEngineApp_m_Settings_match_countdown <= 0.0f) {
        CEngineApp__QuitToLegal_2(&gEngineApp);
        return 1;
    }
    return 0;
}

#include "globals/engine.h"

s32 GameOverMenu_TickCountdown_2(void)
{
  f32 countdown;
  GameMatch *match;
  match = &gEngineApp_m_Settings.match;
  countdown = match->countdown - gFrameIncrement;
  match->countdown = countdown;
  if (countdown < 0.0f)
  {
    match->countdown = 0.0f;
  }
  return 0;
}
