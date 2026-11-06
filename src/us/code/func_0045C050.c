#include "common.h"
#include "globals/pak.h"
#include "globals/menu.h"

/* The second argument: only its word at 0x10 is read (0 or 1 passes). */
typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u32 kind;
} Arg1;

extern s32 D_801309AC;

void SavePortalMenu_Close_2(s32 arg0, Arg1 *arg1) {
    if ((D_801309AC == 0) && (gSavePortalActive != 0) && (arg1->kind < 2U)) {
        D_801309AC = 1;
        gCameraPoolHideViews = 1;
    }
}
