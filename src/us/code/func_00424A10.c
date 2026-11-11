#include "common.h"

s32 MenuItem_SelectSubmenu(sptr, sptr, sptr);

s32 Music_RestoreVolume();                            /* extern */

void InventoryMenu_SelectOk(sptr arg0, sptr arg1, sptr arg2) {
    Music_RestoreVolume();
    MenuItem_SelectSubmenu(arg0, arg1, arg2);
}
