
#include "protos/menu.h"
#include "globals/game.h"
s32 SoundMenu_StepSpeechVolume(sptr arg0, sptr arg1)
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
