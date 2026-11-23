#include "common.h"
#include "structs/menu.h"
#include "globals/game.h"

extern char sBloodLustMenuText_CrosshairsOn[];
extern char sBloodLustMenuText_CrosshairsOff[];

s32 FragTagMenu_CrosshairsLabel(MenuItem *a) {
    switch (gEngineApp_m_Settings.tagCrosshairs) {
    case 0:
        a->text = sBloodLustMenuText_CrosshairsOff;
        break;
    case 1:
        a->text = sBloodLustMenuText_CrosshairsOn;
        break;
    }
    return 0;
}
