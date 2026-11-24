#include "common.h"
#include "globals/pak.h"
#include "globals/menu.h"

/* Only the field read here. */
typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u32 unk10;
} CallbackArg;


void SavePortalMenu_Close_2(s32 arg0, CallbackArg *arg1) {
    if ((sSavePortalDone == 0) && (gSavePortalActive != 0) && (arg1->unk10 < 2U)) {
        sSavePortalDone = 1;
        gCameraPoolHideViews = 1;
    }
}
