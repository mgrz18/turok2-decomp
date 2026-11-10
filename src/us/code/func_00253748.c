
#include "common.h"
#include "globals/game.h"
#ifdef TARGET_PORT
#include "structs/player.h"
/* Host body: the N64 reads player + item - 0xA74, i.e. m_Data.m_SpecialPickupCount[item - 0xFA0]
 * at N64 offsets (and the player as an s32). */
u32 CPlayer__GetPickupCountHigh(CPlayer *player, s32 item) {
    if (!(gEngineApp_m_Settings.cheatFlags & 0x10)) {
        return player->m_Data.m_SpecialPickupCount[item - 0xFA0] >> 4;
    }
    return 1;
}
#else
u32 CPlayer__GetPickupCountHigh(sptr arg0, s32 arg1)
{
  s32 new_var;
  if (!(gEngineApp_m_Settings.cheatFlags & 0x10))
  {
    if (new_var)
    {
      new_var = arg0;
      return ((u8) (*((u8 *) (((s8 *) (arg1 + new_var)) + (-0xA74))))) >> 4;
    }
    else
    {
      new_var = arg0;
      return ((u8) (*((u8 *) (((s8 *) (arg1 + new_var)) + (-0xA74))))) >> 4;
    }
  }
  return 1U;
}
#endif
