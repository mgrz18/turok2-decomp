#include "common.h"
#include "globals/scene.h"

s32 CScene__GetLight(void *);                       /* extern */
void CDLightSet__Alloc(sptr, s32);                    /* extern */

void DLight_Spawn(s32 arg0) {
    CDLightSet__Alloc(arg0, CScene__GetLight(&gScene));
}
