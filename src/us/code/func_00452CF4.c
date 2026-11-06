#include "common.h"
#include "m2c_macros.h"

s32 Music_SetTrack(s32);
s32 Menu_StepValue_2(s32, s32, s32, s32, s32, s32);

s32 Music_GetTrack();                                /* extern */
extern u8 gEngineApp_m_Settings_unk14[]; /* local type: the field's real type (or the member path) changes the ROM's code here */

s32 SoundMenu_StepMusicVolume_2(s32 arg0, unsigned long arg1)
{
  s32 new_var;
  s32 var_a1;
  var_a1 = (s32) gEngineApp_m_Settings_unk14[0];
  new_var = Menu_StepValue_2(arg1, var_a1, 8, 0, 0xFF, 0);
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
