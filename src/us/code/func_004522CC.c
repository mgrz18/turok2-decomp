#include "common.h"
#include "globals/menu_text.h"
#include "structs/game.h"
#include "structs/menu.h"
#include "globals/game.h"

s32 MenuItem_TextLength_2(MenuItem *item); /* length of the item's text line */
s32 sprintf(char *buf, const char *fmt, ...); /* sprintf */


/* Names setting 0x2E. */
s32 func_004522CC(MenuItem *item) {
    switch (gEngineApp_m_Settings.teamCrosshairs) {
    case 0:
        item->text = sBloodLustMenuText_CrosshairsOff;
        break;
    case 1:
        item->text = sBloodLustMenuText_CrosshairsOn;
        break;
    }
    return 0;
}
