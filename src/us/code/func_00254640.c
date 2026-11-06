#include "common.h"
#include "globals/game.h"

s32 GameObject_IsOnGround(s32);


void CPlayer__UpdateSpeedScale(void *arg0, void *arg1, void *arg2)
{
  s32 *new_var;
  f32 temp_f1;
  if (((u16) (*((u16 *) (((s8 *) arg0) + 0xA08)))) < 2U)
  {
    *((f32 *) (((s8 *) arg0) + 0xB28)) = 1.0f;
    return;
  }
  temp_f1 = *((f32 *) (((s8 *) (*((void **) (((s8 *) arg1) + 0x14)))) + 0x1C));
  *((f32 *) (((s8 *) arg0) + 0xB28)) = temp_f1;
  new_var = &gEngineApp_m_Settings;
  if ((*((u8 *) (((s8 *) new_var) + 0x19))) != 0)
  {
    *((f32 *) (((s8 *) arg0) + 0xB28)) = (f32) (temp_f1 * (*((f32 *) (((s8 *) new_var) + 0x1C))));
  }
  if (((!((*((s32 *) (((s8 *) arg1) + 0x140))) & 0xC0000)) && (arg2 != 0)) && ((GameObject_IsOnGround((s32) arg1) != 0) || ((*((u16 *) (((s8 *) arg2) + 0x52))) & 0x800)))
  {
    *((f32 *) (((s8 *) arg0) + 0xB28)) = (f32) ((*((f32 *) (((s8 *) arg0) + 0xB28))) * (*((f32 *) (((s8 *) arg2) + 0x30))));
  }
}
