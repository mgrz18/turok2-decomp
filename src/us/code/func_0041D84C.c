#include "common.h"

/* The multiplayer match state at D_8011ACB0: the fields this reads. */
typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ u8 pad24[4];
    /* 0x28 */ s32 unk28;
} MatchState;

extern MatchState D_8011ACB0;

s32 func_0041D84C(void)
{
  s32 var_a0;
  MatchState *new_var;
  var_a0 = 0;
  new_var = &D_8011ACB0;
  if (((D_8011ACB0.unk28 != 0) || (new_var->unk1C != 0)) || (new_var->unk20 != 0))
  {
    var_a0 = 1;
  }
  return var_a0;
}
