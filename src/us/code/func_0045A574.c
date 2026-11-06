#include "common.h"
#include "rw/abu_protos.h"
#include "globals/menu.h"

/* Something whose current state is a handler address at 0x24. */
typedef struct {
    /* 0x00 */ u8 pad0[0x24];
    /* 0x24 */ s32 stateHandler;
} StateHolder;

extern s32 gSavePortalMenuDesc_2;
extern s32 gSavePortalMenuDescB_2;

/* Whether the state handler is one of the two at 0x43BE14 / 0x43BDF0 (and clears gCameraPoolHideViews then). */
s32 PakMenu_IsSavePortalMenu_2(StateHolder *arg0) {
    s32 temp_v1;

    temp_v1 = arg0->stateHandler;
    if ((temp_v1 == (s32) &gSavePortalMenuDescB_2) || (temp_v1 == (s32) &gSavePortalMenuDesc_2)) {
        gCameraPoolHideViews = 0;
        return 1;
    }
    return 0;
}
