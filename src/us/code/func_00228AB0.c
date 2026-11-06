#include "common.h"

s32 CScene__GetLight(void *);                       /* extern */
void CDLightSet__Alloc(s32, s32);                    /* extern */
extern u8 D_800F7078[];

void DLight_Spawn(s32 arg0) {
    CDLightSet__Alloc(arg0, CScene__GetLight(D_800F7078));
}
