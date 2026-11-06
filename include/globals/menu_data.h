#ifndef GLOBALS_MENU_DATA_H
#define GLOBALS_MENU_DATA_H

/* Tables of the menu module's data (first VM module) other than the
 * MenuDesc descriptors (globals/menu_desc.h), and their module-2 copies. */

#include "structs/menu.h"

extern PlayerSpeedTable sPlayerSpeedTable;   /* MpMenu_StepPlayerSpeed */
extern f32 sPlayerSpeedTable_scale_2[16];    /* module-2 copy's .scale (sPlayerSpeedTable_2 has no symbol) */

#endif
