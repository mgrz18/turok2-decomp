#include "common.h"
#include "m2c_macros.h"
#include "protos/menu.h"

s32 Sound__Play2D(s32);

extern u8 gEngineApp_m_Settings_unk15[]; /* local type: the field's real type (or the member path) changes the ROM's code here */

s32 SoundMenu_StepSfxVolume(sptr arg0, sptr arg1)
{
  s32 var_a1;
  s32 new_var;
  var_a1 = gEngineApp_m_Settings_unk15[0];
  new_var = Menu_StepValue(arg1, (s32) var_a1, 8, 0, 0xFF, 0);
  var_a1 = new_var;
  if (var_a1 == 0xF7)
  {
    var_a1 = 0xF8;
  }
  gEngineApp_m_Settings_unk15[0] = (u8) var_a1;
  Sound__Play2D(0x19E);
  return 0;
}
