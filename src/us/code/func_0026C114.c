#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x59];
    /* 0x59 */ u8 unk59;
} Entry;

Entry *CScene__GetRegionInfo(s32 *, s32);                /* extern */
extern s32 gScene;

u8 func_0026C114(s32 arg0) {
    Entry *temp_v0;

    temp_v0 = CScene__GetRegionInfo(&gScene, arg0);
    if (temp_v0 == NULL) {
        return 0U;
    }
    return temp_v0->unk59;
}
