#include "common.h"
#include "globals/scene.h"

/* The port: the light index reaches CScene__GetLight (the N64 C left it in $a1 for it) and
 * the light comes back pointer-wide. */
#include "structs/particle.h"
void *CScene__GetLight(CScene *, s32);
void *CDLightSet__Alloc(CDLightSet *, void *);
void *DLight_Spawn(CDLightSet *arg0, s32 arg1) {
    return CDLightSet__Alloc(arg0, CScene__GetLight(&gScene, arg1));
}
