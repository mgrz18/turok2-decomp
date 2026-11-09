
#include "protos/menu.h"
extern u8 gEngineApp_m_Settings_unk16[]; /* local type: the field's real type (or the member path) changes the ROM's code here */
s32 SoundMenu_StepSpeechVolume(s32 arg0, s32 arg1)
{
  s32 var_v1;
  s32 new_var;
  var_v1 = gEngineApp_m_Settings_unk16[0];
  new_var = (s32) var_v1;
  var_v1 = Menu_StepValue(arg1, new_var, 8, 0, 0xFF, 0);
  if (var_v1 == 0xF7)
  {
    var_v1 = 0xF8;
  }
  gEngineApp_m_Settings_unk16[0] = (u8) var_v1;
  return 0;
}
