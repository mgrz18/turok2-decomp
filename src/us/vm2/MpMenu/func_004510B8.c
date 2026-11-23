#include "common.h"
#include "globals/game.h"
#include "structs/menu.h"
#include "structs/controller.h"

/* The module-2 copy of PlayerSetupMenu_NameLabel (func_00418108.c): the name line of the player
 * setup window shows the name in the settings slot of the window's controller. Round 23: through
 * the members (the m2c body used the N64 offsets MenuItem +0x14 and Menu +0x20). */
s32 PlayerSetupMenu_NameLabel_2(MenuItem *item, Menu *menu) {
    item->text = (char *) gEngineApp_m_Settings.slots[menu->controller->port].m_Name;
    return 0;
}
