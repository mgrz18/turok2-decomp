#include "common.h"
#include "m2c_macros.h"

s32 Menu_StepValue_2(s32, s32, s32, s32, s32, s32);

extern u8 gEngineApp_m_Settings_unk16[];

s32 SoundMenu_StepSpeechVolume_2(s32 arg0, s32 arg1)
{
  s32 var_v1;
  s32 new_var;
  var_v1 = (s32) gEngineApp_m_Settings_unk16[0];
  new_var = var_v1;
  var_v1 = Menu_StepValue_2(arg1, new_var, 8, 0, 0xFF, 0);
  if (var_v1 == 0xF7)
  {
    var_v1 = 0xF8;
  }
  gEngineApp_m_Settings_unk16[0] = (u8) var_v1;
  return 0;
}
