#include "common.h"
#include "globals/game.h"
#include "structs/menu.h"
#include "structs/controller.h"

/* The update callback of the player setup window's name line: it shows the name in the settings
 * slot of the window's controller. Round 23: through the members (the m2c body wrote the text at
 * MenuItem +0x14 and read Menu +0x20, the N64 offsets: on the host the text is at 0x18 and
 * Menu.controller at 0x30). */
s32 PlayerSetupMenu_NameLabel(MenuItem *item, Menu *menu) {
    item->text = (char *) gEngineApp_m_Settings.slots[menu->controller->port].m_Name;
    return 0;
}
