#include "common.h"
#include "m2c_macros.h"

s32 Sound__Play2D(s32);
s32 Menu_StepValue_2(sptr, s32, s32, s32, s32, s32);

extern u8 gEngineApp_m_Settings_unk15[]; /* local type: the field's real type (or the member path) changes the ROM's code here */

s32 SoundMenu_StepSfxVolume_2(s32 arg0, s32 arg1)
{
  s32 var_a1;
  s32 new_var;
  var_a1 = (s32) gEngineApp_m_Settings_unk15[0];
  new_var = Menu_StepValue_2(arg1, var_a1, 8, 0, 0xFF, 0);
  var_a1 = new_var;
  if (var_a1 == 0xF7)
  {
    var_a1 = 0xF8;
  }
  gEngineApp_m_Settings_unk15[0] = (u8) var_a1;
  Sound__Play2D(0x19E);
  var_a1 = 0;
  return var_a1;
}
