#include "common.h"
#include "m2c_macros.h"

s32 Music_SetTrack(s32);
s32 Menu_StepValue(s32, s32, s32, s32, s32, s32);

s32 Music_GetTrack();                                /* extern */
extern u8 D_8011AAEC[];

s32 SoundMenu_StepMusicVolume(s32 arg0, s32 arg1)
{
  s32 new_var;
  s32 var_a1;
  var_a1 = D_8011AAEC[0];
  new_var = Menu_StepValue(arg1, (s32) var_a1, 8, 0, 0xFF, 0);
  var_a1 = new_var;
  if (var_a1 == 0xF7)
  {
    var_a1 = 0xF8;
  }
  D_8011AAEC[0] = (u8) var_a1;
  if (Music_GetTrack() <= 0)
  {
    Music_SetTrack(1);
  }
  return 0;
}
