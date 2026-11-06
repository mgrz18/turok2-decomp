#include "common.h"
#include "structs/menu.h"

extern s32 gEngineApp_m_Settings_match_tag;

/* Sets or clears flag 0x01000000 of the item from gEngineApp_m_Settings_match_tag. */
void StatsMenu_ShowTagsRow(Menu *arg0) {
    MenuItem *temp_a0;
    MenuItem *temp_v0;

    if (gEngineApp_m_Settings_match_tag != 0) {
        temp_v0 = arg0->items;
        temp_v0[7].flags = (s32) (temp_v0[7].flags | 0x01000000);
        return;
    }
    temp_a0 = arg0->items;
    temp_a0[7].flags = (s32) (temp_a0[7].flags & 0xFEFFFFFF);
}
