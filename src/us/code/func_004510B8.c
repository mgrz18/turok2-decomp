#include "common.h"
#include "globals/game.h"


s32 PlayerSetupMenu_NameLabel_2(void *arg0, void *arg1)
{
  *((void **) (((s8 *) arg0) + 0x14)) = (void *) (((*((s8 *) (((s8 *) (*((void **) (((s8 *) arg1) + 0x20)))) + 4))) << 6) + ((s8 *) gEngineApp_m_Settings.slots[0].m_Name));
  return 0;
}
