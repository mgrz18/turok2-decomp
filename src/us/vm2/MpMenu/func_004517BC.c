#include "common.h"
#include "globals/menu_text.h"
#include "structs/game.h"
#include "structs/menu.h"
#include "globals/game.h"

s32 MenuItem_TextLength_2(MenuItem *item); /* length of the item's text line */
extern void sprintf(char *dst, char *fmt, ...);


/* Names setting 0x29. */
s32 FragTagMenu_CrosshairsLabel_2(MenuItem *item) {
    switch (gEngineApp_m_Settings.tagCrosshairs) {
    case 0:
        item->text = sBloodLustMenuText_CrosshairsOff;
        break;
    case 1:
        item->text = sBloodLustMenuText_CrosshairsOn;
        break;
    }
    return 0;
}
