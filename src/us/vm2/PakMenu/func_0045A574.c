#include "common.h"
#include "globals/menu.h"
#include "globals/menu_desc.h"

s32 PakMenu_IsSavePortalMenu_2(char *arg0) {
    char *p = *(char **) (arg0 + 0x24);

    if (p == (char *)&gSavePortalMenuDescB_2 || p == (char *)&gSavePortalMenuDesc_2) {
        gCameraPoolHideViews = 0;
        return 1;
    }
    return 0;
}
