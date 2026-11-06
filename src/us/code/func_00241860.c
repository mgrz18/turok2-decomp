#include "common.h"
#include "structs/collision.h"

s32 CCollisionSetup__Construct(s32 *);                   /* extern */
extern s32 D_0042D510;                      /* the CCollisionSetup_t table */
extern CCollisionResults *D_800F1ADC;
extern CCollisionResults D_800F1CE0;

void CCollisionInfo__Reset(void) {
    D_800F1ADC = &D_800F1CE0;
    CCollisionSetup__Construct(&D_0042D510);
}
