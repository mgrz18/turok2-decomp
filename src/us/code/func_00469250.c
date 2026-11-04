#include "common.h"
#include "structs/game.h"

void func_0026EDA8(s32);
s32 func_00285410(s32);
s32 func_0025359C(CPlayerList *);                   /* extern */

/* The multiplayer match state at D_8011ACB0: the fields this resets. */

/* The game/menu context this runs on. */

extern s32 D_800F5A50;

void func_00469250(CEngineApp *arg0)
{
  GameMatch *new_var;
  func_00285410(0);
  func_0026EDA8((s32) (&D_800F5A50));
  func_0025359C(&D_80119870.m_PlayerList);
  new_var = &D_8011ACB0;
  new_var->teams = 0;
  D_8011ACB0.tag = 0;
  D_8011ACB0.unk44 = 0;
  arg0->m_Settings.fadeMode = 2;
  arg0->m_Settings.nextMode = 1;
}
