#include "common.h"

s32 MenuPool_OpenChildMenu(s32, s32, s32);

extern s32 D_00438CE4;
extern s32 D_8011ACB0;

s32 DeathmatchMenu_OpenTeamBlood_2(s32 arg0, s32 arg1, s32 arg2)
{
  s8 *new_var;
  new_var = (s8 *) (&D_8011ACB0);
  *((s32 *) (new_var + 0x2C)) = 0;
  *((s32 *) (new_var + 0x30)) = 0;
  *((s32 *) (new_var + 0x28)) = 0;
  MenuPool_OpenChildMenu(arg2, arg1, (s32) (&D_00438CE4));
  return 1;
}
