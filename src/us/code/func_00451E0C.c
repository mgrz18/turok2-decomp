#include "common.h"
#include "structs/game.h"
#include "structs/menu.h"
#include "globals/game.h"

s32 MenuItem_TextLength_2(MenuItem *item); /* length of the item's text line */
s32 sprintf(char *buf, const char *fmt, ...); /* sprintf */

extern char sBloodLustMenuText_CrosshairsOff[];
extern char sBloodLustMenuText_CrosshairsOn[];

/* Names setting 0x24. */
s32 func_00451E0C(MenuItem *item) {
    switch (gEngineApp_m_Settings.dmCrosshairs) {
    case 0:
        item->text = sBloodLustMenuText_CrosshairsOff;
        break;
    case 1:
        item->text = sBloodLustMenuText_CrosshairsOn;
        break;
    }
    return 0;
}
