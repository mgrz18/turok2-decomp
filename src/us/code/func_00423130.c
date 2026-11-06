#include "common.h"

/* Only the field read here. */
typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u32 unk10;
} CallbackArg;

extern s32 gCameraPoolHideViews;
extern s32 gSavePortalActive;
extern s32 D_801309AC;

void SavePortalMenu_Close(s32 arg0, CallbackArg *arg1) {
    if ((D_801309AC == 0) && (gSavePortalActive != 0) && (arg1->unk10 < 2U)) {
        D_801309AC = 1;
        gCameraPoolHideViews = 1;
    }
}
