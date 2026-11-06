#include "common.h"

/* Only the field written here. */
typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ u8 *data;
} MenuData;

extern u8 D_800B5C60[];
extern u8 D_800B5C6C[];
extern s32 D_8011ACE4;

s32 StatsMenu_HeaderText(MenuData *arg0) {
    if (D_8011ACE4 != 0) {
        arg0->data = D_800B5C6C;
    } else {
        arg0->data = D_800B5C60;
    }
    return 0;
}
