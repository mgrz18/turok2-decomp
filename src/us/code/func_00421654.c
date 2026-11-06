#include "globals/menu_desc.h"
typedef signed int s32;

extern s32 gCameraPoolHideViews;

s32 PakMenu_IsSavePortalMenu(char *arg0) {
    char *p = *(char **) (arg0 + 0x24);

    if (p == (char *)&gSavePortalMenuDescB || p == (char *)&gSavePortalMenuDesc) {
        gCameraPoolHideViews = 0;
        return 1;
    }
    return 0;
}
