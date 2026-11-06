#include "common.h"
#include "structs/menu.h"
#include "globals/game.h"

extern char D_800B6164[];
extern char D_800B617C[];

s32 BloodLustMenu_CrosshairsLabel(MenuItem *a) {
    switch (gEngineApp_m_Settings.dmCrosshairs) {
    case 0:
        a->text = D_800B617C;
        break;
    case 1:
        a->text = D_800B6164;
        break;
    }
    return 0;
}
