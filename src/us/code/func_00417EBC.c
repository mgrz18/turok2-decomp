#include "common.h"
#include "globals/game.h"
#include "protos/game.h"
#include "structs/menu.h"
#include "structs/player.h"
#include "structs/controller.h"

void Controller_UnwatchPak(sptr);
void Controller_ResetRumble(ControllerProfile *);

/* The select callback of the rumble notice's last item (a MenuItemDesc select: the item, its
 * window): stop watching the pak, reset and enable the rumble, and clear the player's
 * m_bEnteringDeathmatch. Round 22 de-faked it: Controller_ResetRumble returns void (its
 * definition, func_00285AE0.c); declared s32, the draft needed an if/else with two equal arms
 * to keep the player pointer out of $v1. The N64 stores through the player unconditionally;
 * the port (round 13 b5) keeps a NULL test. */
s32 RumbleMenu_Play(s32 arg0, void *arg1) {
    Menu *menu = (Menu *) arg1;
    gEngineApp_m_Settings.match.tagPhase = 0;
    Controller_UnwatchPak((sptr) menu->controller->port);
    Controller_ResetRumble(menu->controller);
    Controller_SetRumbleOption(menu->controller, 1);
#ifdef TARGET_PORT
    if (menu->player != NULL)
#endif
        menu->player->m_bEnteringDeathmatch = 0;
    return 1;
}
