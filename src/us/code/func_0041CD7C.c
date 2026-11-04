#include "common.h"
#include "structs/menu.h"

extern s32 D_8011ACE4;

/* Sets or clears flag 0x01000000 of the item from D_8011ACE4. */
void func_0041CD7C(Menu *arg0) {
    MenuItem *temp_a0;
    MenuItem *temp_v0;

    if (D_8011ACE4 != 0) {
        temp_v0 = arg0->items;
        temp_v0[7].flags = (s32) (temp_v0[7].flags | 0x01000000);
        return;
    }
    temp_a0 = arg0->items;
    temp_a0[7].flags = (s32) (temp_a0[7].flags & 0xFEFFFFFF);
}
