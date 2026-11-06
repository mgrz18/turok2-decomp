#include "common.h"
#include "m2c_macros.h"

s32 Sound__Play2D(s32);
s32 Menu_StepValue(s32, s32, s32, s32, s32, s32);

extern u8 D_8011AAED[];

s32 SoundMenu_StepSfxVolume(s32 arg0, s32 arg1)
{
  s32 var_a1;
  s32 new_var;
  var_a1 = D_8011AAED[0];
  new_var = Menu_StepValue(arg1, (s32) var_a1, 8, 0, 0xFF, 0);
  var_a1 = new_var;
  if (var_a1 == 0xF7)
  {
    var_a1 = 0xF8;
  }
  D_8011AAED[0] = (u8) var_a1;
  Sound__Play2D(0x19E);
  return 0;
}
