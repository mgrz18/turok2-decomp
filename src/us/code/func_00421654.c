#include "common.h"
#include "globals/menu.h"

extern char gSavePortalMenuDescB[];
extern char gSavePortalMenuDesc[];

s32 PakMenu_IsSavePortalMenu(char *arg0) {
    char *p = *(char **) (arg0 + 0x24);

    if (p == gSavePortalMenuDescB || p == gSavePortalMenuDesc) {
        gCameraPoolHideViews = 0;
        return 1;
    }
    return 0;
}
