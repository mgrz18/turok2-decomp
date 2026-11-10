#include "common.h"

s32 MenuItem_SelectSubmenu_2(sptr, sptr, sptr);

void Music_RestoreVolume();                            /* extern */

void InventoryMenu_SelectOk_2(s32 arg0, s32 arg1, s32 arg2) {
    Music_RestoreVolume();
    MenuItem_SelectSubmenu_2(arg0, arg1, arg2);
}
