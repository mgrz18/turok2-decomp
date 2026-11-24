#include "common.h"
#include "m2c_macros.h"
#include "protos/menu.h"

void Music_SetTrack(s32 track);

s32 Music_GetTrack();                                /* extern */
#include "globals/game.h"

s32 SoundMenu_StepMusicVolume(sptr arg0, sptr arg1)
{
  s32 new_var;
  s32 var_a1;
  var_a1 = gEngineApp_m_Settings_unk14[0];
  new_var = Menu_StepValue(arg1, (s32) var_a1, 8, 0, 0xFF, 0);
  var_a1 = new_var;
  if (var_a1 == 0xF7)
  {
    var_a1 = 0xF8;
  }
  gEngineApp_m_Settings_unk14[0] = (u8) var_a1;
  if (Music_GetTrack() <= 0)
  {
    Music_SetTrack(1);
  }
  return 0;
}
