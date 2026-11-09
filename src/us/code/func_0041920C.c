#include "common.h"
#include "globals/game.h"
#include "protos/menu.h"



s32 TeamBloodMenu_StepTextureSet(s32 arg0, s32 arg1)
{
  s8 *new_var;
  new_var = (s8 *) (&gEngineApp_m_Settings);
  *((s8 *) (((s8 *) (&gEngineApp_m_Settings)) + 0x2D)) = Menu_StepValue(arg1, (s32) (*((s8 *) (new_var + 0x2D))), 1, 0, 8, 1);
  return 0;
}
