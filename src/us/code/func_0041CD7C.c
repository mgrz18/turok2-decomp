#include "common.h"

typedef struct {
    /* 0x000 */ u8 pad0[0x120];
    /* 0x120 */ s32 flags;
} MenuItem;

typedef struct {
    /* 0x0 */ u8 pad0[0xC];
    /* 0xC */ MenuItem *item;
} Menu;

extern s32 D_8011ACE4;

/* Sets or clears flag 0x01000000 of the item from D_8011ACE4. */
void func_0041CD7C(Menu *arg0) {
    MenuItem *temp_a0;
    MenuItem *temp_v0;

    if (D_8011ACE4 != 0) {
        temp_v0 = arg0->item;
        temp_v0->flags = (s32) (temp_v0->flags | 0x01000000);
        return;
    }
    temp_a0 = arg0->item;
    temp_a0->flags = (s32) (temp_a0->flags & 0xFEFFFFFF);
}
