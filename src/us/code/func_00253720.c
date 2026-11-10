
#include "common.h"
#include "globals/game.h"
#ifdef TARGET_PORT
#include "structs/player.h"
/* Host body: the N64 reads player + item - 0xA74, i.e. m_Data.m_SpecialPickupCount[item - 0xFA0]
 * at N64 offsets (and the player as an s32). */
s32 CPlayer__GetPickupCountLow(CPlayer *player, s32 item) {
    if (!(gEngineApp_m_Settings.cheatFlags & 0x10)) {
        return player->m_Data.m_SpecialPickupCount[item - 0xFA0] & 0xF;
    }
    return 1;
}
#else
s32 CPlayer__GetPickupCountLow(sptr arg0, s32 arg1)
{
  unsigned long long new_var;
  new_var = 1;
  if (!(gEngineApp_m_Settings.cheatFlags & 0x10))
  {
    return (*((u8 *) (((s8 *) (arg1 + arg0)) + (-0xA74)))) & 0xF;
  }
  if (new_var)
  {
    return new_var;
  }
  else
  {
    return new_var;
  }
}
#endif
