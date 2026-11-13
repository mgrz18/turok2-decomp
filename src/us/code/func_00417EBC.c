#include "common.h"
#include "m2c_macros.h"
#include "globals/game.h"
#include "protos/game.h"

void Controller_UnwatchPak(sptr);
s32 Controller_ResetRumble(sptr);


#ifdef TARGET_PORT
/* The port (round 13 b5): the select callback of the rumble notice's last item (a MenuItemDesc
 * select: the item, its window). The m2c body read the window's player and controller at their N64
 * offsets (0x1C, 0x20; host 0x30, 0x38: it took the descriptor for the controller and wrote 0
 * through a pointer made of padding); the host body reads Menu's members. 0xBF8 is
 * CPlayer.m_bEnteringDeathmatch. (n64viewcheck: the a5 sweep's unresolved RumbleMenu_Play.) */
#include "structs/menu.h"
#include "structs/player.h"
#include "structs/controller.h"
s32 RumbleMenu_Play(s32 arg0, void *arg1) {
    Menu *menu = (Menu *) arg1;
    gEngineApp_m_Settings.match.tagPhase = 0;
    Controller_UnwatchPak((sptr) menu->controller->port);
    Controller_ResetRumble((sptr) menu->controller);
    Controller_SetRumbleOption(menu->controller, 1);
    if (menu->player != NULL) {
        menu->player->m_bEnteringDeathmatch = 0;
    }
    return 1;
}
#else
s32 RumbleMenu_Play(s32 arg0, void *arg1)
{
  gEngineApp_m_Settings.match.tagPhase = 0;
  Controller_UnwatchPak((s32) (*((s8 *) (((s8 *) (*((void **) (((s8 *) arg1) + 0x20)))) + 4))));
  if (1)
  {
    if (arg1)
    {
      Controller_ResetRumble((sptr) (*((void **) (((s8 *) arg1) + 0x20))));
      Controller_SetRumbleOption((sptr) (*((void **) (((s8 *) arg1) + 0x20))), 1);
      *((s32 *) (((s8 *) (*((void **) (((s8 *) arg1) + 0x1C)))) + 0xBF8)) = 0;
    }
    else
    {
      Controller_ResetRumble((sptr) (*((void **) (((s8 *) arg1) + 0x20))));
      Controller_SetRumbleOption((sptr) (*((void **) (((s8 *) arg1) + 0x20))), 1);
      *((s32 *) (((s8 *) (*((void **) (((s8 *) arg1) + 0x1C)))) + 0xBF8)) = 0;
    }
  }
  return 1;
}
#endif
