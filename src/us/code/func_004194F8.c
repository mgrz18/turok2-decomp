#include "common.h"
#include "structs/game.h"

s32 func_00416894(s32, s32, s32);

extern u8 D_00438DC4[];

s32 func_004194F8(s32 arg0, s32 arg1, s32 arg2)
{
  GameMatch *match;
  match = &D_8011ACB0;
  D_8011ACB0.teamScore[0] = 0;
  match->teamScore[1] = 0;
  match->teamOver = 0;
  func_00416894(arg2, arg1, (s32) D_00438DC4);
  return 1;
}
