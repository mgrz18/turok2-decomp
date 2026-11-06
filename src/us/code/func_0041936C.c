#include "common.h"
#include "structs/menu.h"

extern u8 gEngineApp_m_Settings_teamCrosshairs; /* gEngineApp_m_Settings + 0x2E */
extern char D_800B6164[];
extern char D_800B617C[];

s32 TeamBloodMenu_CrosshairsLabel(MenuItem *a) {
    switch (gEngineApp_m_Settings_teamCrosshairs) {
    case 0:
        a->text = D_800B617C;
        break;
    case 1:
        a->text = D_800B6164;
        break;
    }
    return 0;
}
